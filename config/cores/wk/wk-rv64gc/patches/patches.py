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
from gen_rvtest_header import LA_TO_LI_RE, MMIO_ADDR_MACROS  # noqa: F401  (共享常量)

# 生成派生头文件；失败时立即中止，避免后续静默使用残缺头
if gen_rvtest_header.main() != 0:
    raise RuntimeError("gen_rvtest_header failed")


HERE = Path(__file__).resolve().parent
OVERRIDES_DIR = HERE / "override_header"
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
    """包装补丁函数：**每个补丁函数**对**每个模块**只执行一次（幂等）。

    flag 使用 module + qualname，避免不同模块 / 不同作用域下同名函数冲突。
    """
    flag = f"_user_patched_{fn.__module__}_{fn.__qualname__}".replace(".", "_")

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
        if not base:
            return [f"-I{OVERRIDES_DIR}", "-mno-relax"]
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

        flen  = getattr(test_metadata, "flen",  None)
        march = getattr(test_metadata, "march", None)
        if flen is None or march is None:
            sys.stderr.write(
                f"[patches] warning: test_metadata missing flen/march "
                f"(flen={flen!r}, march={march!r}) for {test_name}\n"
            )

        # (a)
        cfg_path = base_dir / "elfs" / test_name.with_suffix(".cfg")
        cfg_path.parent.mkdir(parents=True, exist_ok=True)
        cfg_path.write_text(
            f"XLEN={xlen}\n"
            f"TEST_FLEN={flen if flen is not None else 0}\n"
            f"march={march if march is not None else ''}\n"
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
        if " " not in s:
            return f"{s} {' '.join(opt)}"
        head, tail = s.split(" ", 1)
        return f"{head} {' '.join(opt)} {tail}"

    mod._ref_model_sig_cmd = _ref_model_sig_cmd_with_inst_limit


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
        return [LA_TO_LI_RE.sub(r"LI(\1, \2)", line) for line in lines]

    mod._generate_mcsr_cntr_tests = _generate_mcsr_cntr_tests_with_li


@atexit.register
def _auto_pack_on_exit() -> None:
    """Python 进程退出前，把所有 work/<config>/ 打包。"""
    try:
        patches_dir        = OVERRIDES_DIR
        config_include_dir = patches_dir.parent

        env_dir   = Path.cwd().resolve() / "tests" / "env"
        work_root = Path.cwd() / "work"

        if not work_root.is_dir():
            return

        for config_dir in sorted(work_root.iterdir()):
            if not config_dir.is_dir():
                continue
            try:
                _finalize_one_config(
                    config_dir, env_dir, patches_dir, config_include_dir
                )
            except Exception as e:
                print(f"[auto-pack] ERROR for {config_dir}: {e}", file=sys.stderr)
                import traceback
                traceback.print_exc()
    except Exception as e:
        print(f"[auto-pack] outer ERROR: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()


# ---------------------------------------------------------------------------
# 5) MetaPathFinder：模块首次 import 后自动应用注册的补丁
# ---------------------------------------------------------------------------
class _Patcher(importlib.abc.MetaPathFinder):
    # 正在包装 loader 的模块名；防止 find_spec 内部重入
    _installing: set[str] = set()

    def find_spec(self, fullname, path, target=None):
        if fullname not in _REGISTRY or fullname in self._installing:
            return None

        self._installing.add(fullname)
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
                        fn = getattr(orig_loader, "create_module", None)
                        return fn(spec) if fn is not None else None

                    def exec_module(self, module):
                        orig_loader.exec_module(module)
                        for p in patchers:
                            p(module)

                spec.loader = _WrappedLoader()
                return spec
        finally:
            self._installing.discard(fullname)
        return None


sys.meta_path.insert(0, _Patcher())