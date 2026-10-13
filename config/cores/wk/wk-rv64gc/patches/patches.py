import sys
import importlib.abc
from pathlib import Path
from act.parse_test_constraints import TestMetadata


# ---------------------------------------------------------------------------
# 你的 override 目录（可用环境变量覆盖）
# 这是存放自定义 rvtest_setup.h / rvmodel_macros.h 的目录
# ---------------------------------------------------------------------------
# OVERRIDES_DIR = os.environ.get(
#     "ACT4_OVERRIDES_DIR",
#     str(Path(__file__).resolve().parent / "overrides" / "wk-rv64gc"),
# )
# os.makedirs(OVERRIDES_DIR, exist_ok=True)
OVERRIDES_DIR = "/home/lony/proj/riscv-arch-test-act4/config/cores/wk/wk-rv64gc/patches"

# ---------------------------------------------------------------------------
# 补丁 1: Toolchain.compile_prefix 追加 -mno-relax
# ---------------------------------------------------------------------------
def _patch_toolchain(mod):
    if getattr(mod, "_patched_by_user", False):
        return
    Toolchain = getattr(mod, "Toolchain", None)
    if Toolchain is None:
        return
    original = Toolchain.compile_prefix

    def compile_prefix_with_norelax(self, xlen):
        original_prefix = list(original(self, xlen))
        return [
            original_prefix[0],
            f"-I{OVERRIDES_DIR}",
            *original_prefix[1:], 
            "-mno-relax"
            ]

    Toolchain.compile_prefix = compile_prefix_with_norelax
    mod._patched_by_user = True


# ---------------------------------------------------------------------------
# 补丁 2: act.build_plan.gen_compile_tasks 增加写 .cfg 的副作用
# ---------------------------------------------------------------------------
def _patch_build_plan(mod):
    if getattr(mod, "_patched_by_user", False):
        return
    original = mod.gen_compile_tasks

    def patched_gen_compile_tasks(*args, **kwargs):
        # 1) 先调用上游原函数，拿到 tasks 列表
        tasks = original(*args, **kwargs)

        # 2) 复现原来那段“写 .cfg”的副作用
        #    签名顺序：
        #    test_name(0), test_metadata(1), base_dir(2), env_dir(3),
        #    xlen(4), config(5), toolchain(6), ...
        def pick(name, index):
            return kwargs.get(name, args[index] if len(args) > index else None)

        test_name:Path              = pick("test_name",     0)
        test_metadata:TestMetadata  = pick("test_metadata", 1)
        base_dir:Path               = pick("base_dir",      2)
        xlen:int                    = pick("xlen",          4)

        import os
        final_cfg = base_dir / "elfs" / test_name.with_suffix(".cfg")
        os.makedirs(os.path.dirname(os.path.abspath(final_cfg)), exist_ok=True)
        with open(str(final_cfg), "w") as cfg_file:
            cfg_file.write(f"XLEN={xlen}\n")
            cfg_file.write(f"TEST_FLEN={test_metadata.flen}\n")
            cfg_file.write(f"march={test_metadata.march}\n")

        return tasks

    mod.gen_compile_tasks = patched_gen_compile_tasks
    mod._patched_by_user = True


# ---------------------------------------------------------------------------
# Meta path finder: 拦截目标模块加载后自动打补丁
# ---------------------------------------------------------------------------
class _Patcher(importlib.abc.MetaPathFinder):
    _targets = {
        "act.toolchain":   _patch_toolchain,
        "act.build_plan":  _patch_build_plan,
    }
    _busy = False

    def find_spec(self, fullname, path, target=None):
        if fullname not in self._targets or self._busy:
            return None
        self._busy = True
        try:
            for finder in sys.meta_path:
                if finder is self:
                    continue
                spec = finder.find_spec(fullname, path, target)
                if spec is not None and spec.loader is not None:
                    orig_loader = spec.loader
                    patcher = self._targets[fullname]

                    class _WrappedLoader(importlib.abc.Loader):
                        def create_module(self, spec):
                            return orig_loader.create_module(spec)

                        def exec_module(self, module):
                            orig_loader.exec_module(module)
                            patcher(module)

                    spec.loader = _WrappedLoader()
                    return spec
        finally:
            self._busy = False
        return None


sys.meta_path.insert(0, _Patcher())