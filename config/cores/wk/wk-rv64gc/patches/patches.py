"""patches.py — ACT4 本地补丁入口。

由 sitecustomize.py 通过 `import patches` 加载。职责：
  1. 先确保派生头文件已生成（gen_rvtest_header.py）。
  2. 用声明式注册表给若干上游模块打补丁。
  3. 用 MetaPathFinder 在模块首次 import 后自动应用补丁。
"""
import os
import sys
import importlib.abc
from pathlib import Path
from act.parse_test_constraints import TestMetadata

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
    """包装补丁函数：每个模块只打一次补丁（幂等）。"""
    def wrapped(mod):
        if getattr(mod, "_user_patched", False):
            return
        fn(mod)
        mod._user_patched = True
    return wrapped


# ---------------------------------------------------------------------------
# 4) 具体补丁
# ---------------------------------------------------------------------------

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
    """在 gen_compile_tasks 之后写一份 .cfg，供外部工具消费。"""
    original = mod.gen_compile_tasks

    def patched_gen_compile_tasks(*args, **kwargs):
        tasks = original(*args, **kwargs)

        def pick(name, index):
            return kwargs.get(name, args[index] if len(args) > index else None)

        test_name = pick("test_name", 0)
        test_metadata = pick("test_metadata", 1)
        base_dir = pick("base_dir", 2)
        xlen = pick("xlen", 4)

        final_cfg = base_dir / "elfs" / test_name.with_suffix(".cfg")
        final_cfg.parent.mkdir(parents=True, exist_ok=True)
        final_cfg.write_text(
            f"XLEN={xlen}\n"
            f"TEST_FLEN={test_metadata.flen}\n"
            f"march={test_metadata.march}\n"
        )
        return tasks

    mod.gen_compile_tasks = patched_gen_compile_tasks


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