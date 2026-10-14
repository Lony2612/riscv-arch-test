"""patches.py — ACT4 本地补丁入口。

由 sitecustomize.py 通过 `import patches` 加载。职责：
  1. 先确保派生头文件已生成（gen_rvtest_header.py）。
  2. 用声明式注册表给若干上游模块打补丁。
  3. 用 MetaPathFinder 在模块首次 import 后自动应用补丁。
"""
import sys
import shutil
import atexit
import tarfile
import inspect
import importlib.abc
from pathlib import Path

from act.build_types import BuildTask, PythonAction

import gen_rvtest_header
gen_rvtest_header.main()


OVERRIDES_DIR = Path(__file__).resolve().parent
INST_LIMIT = 2000000


# ---------------------------------------------------------------------------
# 3) 补丁注册表：用装饰器声明“哪个模块需要被哪个函数打补丁”
# ---------------------------------------------------------------------------
_REGISTRY: dict[str, list] = {}


def patch(target: str):
    """把一个函数注册为 `target` 模块的补丁函数。

    同一个模块可以有多个补丁函数，按注册顺序依次调用。
    """
    def deco(fn):
        _REGISTRY.setdefault(target, []).append(fn)
        return fn
    return deco


def _once(fn):
    """包装补丁函数：**每个补丁函数**对**每个模块**只执行一次（幂等）。"""
    flag = f"_user_patched_{fn.__name__}"     # ← 用函数名区分

    def wrapped(mod):
        if getattr(mod, flag, False):
            return
        fn(mod)
        setattr(mod, flag, True)

    return wrapped

def _finalize_one_config(config_dir: Path,
                         env_dir: Path,
                         patches_dir: Path,
                         config_include_dir: Path) -> None:
    """为单个 config 收集 lib 并打包 <config>.tar.gz。"""
    base_dir = config_dir                          # 例如 work/wk-rv64gc
    lib_dir  = base_dir / "lib"
    cases    = base_dir / "cases"

    # 没 cases 也没 lib，直接跳过
    if not cases.is_dir() and not lib_dir.is_dir():
        return

    lib_dir.mkdir(parents=True, exist_ok=True)

    def _copy(src: Path) -> None:
        if src.is_file():
            shutil.copy2(src, lib_dir / src.name)

    if env_dir.is_dir():
        for h in env_dir.glob("*.h"):
            _copy(h)
    if patches_dir.is_dir():
        for h in patches_dir.glob("*.h"):
            _copy(h)
    _copy(config_include_dir / "rvmodel_macros.h")
    _copy(config_include_dir / "link.ld")
    _copy(base_dir / "rvtest_config.h")

    archive = base_dir / f"{base_dir.name}.tar.gz"
    with tarfile.open(archive, "w:gz") as tar:
        for name in ("cases", "lib"):
            d = base_dir / name
            if d.is_dir():
                tar.add(d, arcname=name)

    print(f"[auto-pack] wrote {archive}")


# ---------------------------------------------------------------------------
# 4) 具体补丁
# ---------------------------------------------------------------------------
def _copy_case_files(cfg_src: Path, s_src: Path, results_src: Path,
                     cfg_dst: Path, s_dst: Path, results_dst: Path,
                     cases_dir: Path) -> None:
    cases_dir.mkdir(parents=True, exist_ok=True)
    for src, dst in ((cfg_src, cfg_dst),
                     (s_src, s_dst),
                     (results_src, results_dst)):
        if src.exists():
            shutil.copy2(src, dst)

def _bind_args(original, args, kwargs) -> dict:
    """把 (args, kwargs) 绑定到 original 的签名上，返回 {参数名: 值}。

    好处：上游改签名（加/删/重排参数）这里自动跟进，不用改索引。
    """
    sig = inspect.signature(original)
    bound = sig.bind_partial(*args, **kwargs)
    bound.apply_defaults()
    return dict(bound.arguments)

def _build_cases_pack_task(test_name: Path, test_metadata, base_dir: Path):
    """把 cfg/.S/.results 统一命名后复制到 base_dir/cases/<group>/。

    - group = test_name.parent.name（如 "Sm"、"SvPMP"、"I"），丢掉 priv/ 或 rv64i/ 前缀
    - 输出文件名：<stem>.cfg / <stem>.S / <stem>.results
      其中 stem = test_name.stem（去掉 .S 扩展名）
    - 只处理 needs_signature 的测试；其它测试返回 None
    - deps=(<build>/<name>.results,) 保证排在 results 任务之后
    """
    if not getattr(test_metadata, "needs_signature", False):
        return None

    group     = test_name.parent.name           # 比 parts[-2] 更明确
    stem      = test_name.stem
    cases_dir = base_dir / "cases" / group

    cfg_src     = (base_dir / "elfs"  / test_name).with_suffix(".cfg")
    s_src       = Path(test_metadata.test_path).resolve()
    results_src = (base_dir / "build" / test_name).with_suffix(".results")

    cfg_dst     = cases_dir / f"{stem}.cfg"
    s_dst       = cases_dir / f"{stem}.S"
    results_dst = cases_dir / f"{stem}.results"
    
    return BuildTask(
        outputs=(cfg_dst, s_dst, results_dst),
        deps=(results_src,),
        extra_inputs=(cfg_src, s_src),
        action=PythonAction(
            fn=_copy_case_files,
            args=(cfg_src, s_src, results_src,
                  cfg_dst, s_dst, results_dst, cases_dir),
        ),
    )


@patch("act.toolchain")
@_once
def _toolchain_inject_include(mod):
    """把 -I<patches> 插到编译器之后、其它 flag 之前；并追加 -mno-relax。"""
    Toolchain = getattr(mod, "Toolchain", None)
    if Toolchain is None:
        return
    original = Toolchain.compile_prefix

    def compile_prefix(self, xlen):
        base = list(original(self, xlen))
        return [base[0], f"-I{OVERRIDES_DIR}", *base[1:], "-mno-relax"]

    Toolchain.compile_prefix = compile_prefix


@patch("act.build_plan")
@_once
def _build_plan_write_cfg(mod):
    """per-test 后处理：
       (a) 写 <base_dir>/elfs/<name>.cfg
       (b) 追加打包任务到 <base_dir>/cases/<group>/
    """
    original = mod.gen_compile_tasks

    def patched(*args, **kwargs):
        tasks = original(*args, **kwargs)

        params = _bind_args(original, args, kwargs)
        test_name     = params["test_name"]
        test_metadata = params["test_metadata"]
        base_dir      = params["base_dir"]
        xlen          = params["xlen"]

        # (a)
        cfg_path = base_dir / "elfs" / test_name.with_suffix(".cfg")
        cfg_path.parent.mkdir(parents=True, exist_ok=True)
        cfg_path.write_text(
            f"XLEN={xlen}\n"
            f"TEST_FLEN={test_metadata.flen}\n"
            f"march={test_metadata.march}\n"
        )

        # (b)
        pack = _build_cases_pack_task(test_name, test_metadata, base_dir)
        if pack is not None:
            tasks.append(pack)

        return tasks

    mod.gen_compile_tasks = patched


@patch("act.build_plan")
@_once
def _ref_model_inst_limit(mod):
    """给参考模型命令行注入 --inst-limit。"""
    original = getattr(mod, "_ref_model_sig_cmd", None)
    if original is None:
        return

    def _ref_model_sig_cmd_with_inst_limit(*args, **kwargs):
        cmd = original(*args, **kwargs)
        opt = ["--inst-limit", str(INST_LIMIT)]
        if isinstance(cmd, list):
            return [*cmd[:1], *opt, *cmd[1:]]
        if isinstance(cmd, tuple):
            return (*cmd[:1], *opt, *cmd[1:])
        # 兜底：字符串形式，在首个空格后插入
        s = str(cmd)
        idx = s.find(" ")
        return f"{s} {' '.join(opt)}" if idx == -1 else f"{s[:idx]} {' '.join(opt)}{s[idx:]}"

    mod._ref_model_sig_cmd = _ref_model_sig_cmd_with_inst_limit


# 需要 LA -> LI 的 MMIO 地址宏（与 gen_rvtest_header.py 保持一致）
_LA_TO_LI_TARGETS = (
    "RVMODEL_MTIME_ADDRESS",
    "RVMODEL_MTIMECMP_ADDRESS",
    "RVMODEL_MSIP_ADDRESS",
)
_LA_TO_LI_RE = __import__("re").compile(
    r"\bLA\(\s*([A-Za-z_]\w*)\s*,\s*("
    + "|".join(_LA_TO_LI_TARGETS)
    + r")\s*\)"
)


@patch("testgen.priv.extensions.Sm")
@_once
def _sm_la_to_li(mod):
    """把 Sm.py 生成的 mtime/mtimecmp/msip 相关 LA 改写为 LI。

    这些 MMIO 地址在 medlow code model 下超出 la 的可达范围，会触发
    'relocation truncated to fit'。LI 对任意 64 位立即数都有效。
    """
    original = getattr(mod, "_generate_mcsr_cntr_tests", None)
    if original is None:
        return

    def _generate_mcsr_cntr_tests_with_li(test_data):
        lines = original(test_data)
        return [_LA_TO_LI_RE.sub(r"LI(\1, \2)", line) for line in lines]

    mod._generate_mcsr_cntr_tests = _generate_mcsr_cntr_tests_with_li


@atexit.register
def _auto_pack_on_exit() -> None:
    """Python 进程退出前，把所有 work/<config>/ 打包。"""
    patches_dir = Path(__file__).resolve().parent            # .../config/cores/wk/wk-rv64gc/patches
    config_include_dir = patches_dir.parent                  # .../config/cores/wk/wk-rv64gc
    act4_root_env = Path.cwd()
    act4_root = Path(act4_root_env).resolve()

    env_dir = act4_root / "tests" / "env"
    work_root = Path.cwd() / "work"

    if not work_root.is_dir():
        return

    for config_dir in sorted(work_root.iterdir()):
        if not config_dir.is_dir():
            continue
        try:
            _finalize_one_config(config_dir, env_dir, patches_dir, config_include_dir)
        except Exception as e:
            print(f"[auto-pack] ERROR for {config_dir}: {e}", file=sys.stderr)
            import traceback
            traceback.print_exc()

# ---------------------------------------------------------------------------
# 5) MetaPathFinder：模块首次 import 后自动应用注册的补丁
# ---------------------------------------------------------------------------
class _Patcher(importlib.abc.MetaPathFinder):
    _busy = False

    def find_spec(self, fullname, path, target=None):
        if fullname not in _REGISTRY or self._busy:
            return None
        self._busy = True
        try:
            for finder in sys.meta_path:
                if finder is self:
                    continue
                spec = finder.find_spec(fullname, path, target)
                if spec is None or spec.loader is None:
                    continue

                orig_loader = spec.loader
                patchers = list(_REGISTRY[fullname])

                class _WrappedLoader(importlib.abc.Loader):
                    def create_module(self, spec):
                        return orig_loader.create_module(spec)

                    def exec_module(self, module):
                        orig_loader.exec_module(module)
                        for p in patchers:
                            p(module)

                spec.loader = _WrappedLoader()
                return spec
        finally:
            self._busy = False
        return None


sys.meta_path.insert(0, _Patcher())