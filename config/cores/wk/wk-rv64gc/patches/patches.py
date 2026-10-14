import sys
import importlib.abc
from pathlib import Path
from act.parse_test_constraints import TestMetadata
import os


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

# INST_LIMIT = int(os.environ.get("ACT4_REF_MODEL_INST_LIMIT", "10000000"))
INST_LIMIT = 2000000

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

def _patch_ref_model(mod):
    if getattr(mod, "_patched_ref_model_by_user", False):
        return
    original = getattr(mod, "_ref_model_sig_cmd", None)
    if original is None:
        return

    def _ref_model_sig_cmd_with_inst_limit(*args, **kwargs):
        cmd = original(*args, **kwargs)

        if isinstance(cmd, list):
            if not cmd:
                return ["--inst-limit", str(INST_LIMIT)]
            return [cmd[0], "--inst-limit", str(INST_LIMIT), *cmd[1:]]

        if isinstance(cmd, tuple):
            if not cmd:
                return ("--inst-limit", str(INST_LIMIT))
            return (cmd[0], "--inst-limit", str(INST_LIMIT), *cmd[1:])

        # str 兜底：在第一个空格后插入（假设第一段是 sail 可执行文件）
        s = str(cmd)
        idx = s.find(" ")
        if idx == -1:
            return f"{s} --inst-limit {INST_LIMIT}"
        return f"{s[:idx]} --inst-limit {INST_LIMIT}{s[idx:]}"

    mod._ref_model_sig_cmd = _ref_model_sig_cmd_with_inst_limit
    mod._patched_ref_model_by_user = True

# patches/sm_patch.py 或直接加到你的 patch 脚本里

def _patch_sm(mod):
    """把 Sm.py 生成的 mtime 相关 LA 替换为 LI。

    RVMODEL_MTIME_ADDRESS 是 MMIO 地址（一般 > 1GB），
    la 会在 medlow 下报 relocation truncated；li 对任意 64 位地址都有效。
    """
    if getattr(mod, "_patched_sm_by_user", False):
        return

    original = getattr(mod, "_generate_mcsr_cntr_tests", None)
    if original is None:
        return

    def _generate_mcsr_cntr_tests_with_li(test_data):
        lines = original(test_data)
        new_lines = []
        for line in lines:
            # 只替换引用 RVMODEL_MTIME_ADDRESS 的 LA
            if "LA(" in line and "RVMODEL_MTIME_ADDRESS" in line:
                line = line.replace("LA(", "LI(")
            new_lines.append(line)
        return new_lines

    mod._generate_mcsr_cntr_tests = _generate_mcsr_cntr_tests_with_li
    mod._patched_sm_by_user = True

# ---------------------------------------------------------------------------
# Meta path finder: 拦截目标模块加载后自动打补丁
# ---------------------------------------------------------------------------
class _Patcher(importlib.abc.MetaPathFinder):
    _targets = {
        "act.toolchain":                [_patch_toolchain],
        "act.build_plan":               [_patch_build_plan, _patch_ref_model],
        "testgen.priv.extensions.Sm":   [_patch_sm],
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
                    patchers = self._targets[fullname]      # ← 列表

                    class _WrappedLoader(importlib.abc.Loader):
                        def create_module(self, spec):
                            return orig_loader.create_module(spec)

                        def exec_module(self, module):
                            orig_loader.exec_module(module)
                            for p in patchers:              # ← 遍历
                                p(module)

                    spec.loader = _WrappedLoader()
                    return spec
        finally:
            self._busy = False
        return None


sys.meta_path.insert(0, _Patcher())