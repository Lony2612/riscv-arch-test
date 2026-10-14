import os
import sys
import subprocess

_here = os.path.dirname(os.path.abspath(__file__))
if _here not in sys.path:
    sys.path.insert(0, _here)

_repo = os.environ.get("ACT4_REPO_ROOT")
if _repo and _repo not in sys.path:
    sys.path.insert(0, _repo)

# ---- 生成 patches/rvtest_failure_code.h，带递归保护 ----
_GEN_GUARD = "ACT4_FAILURE_CODE_GEN_DONE"

if not os.environ.get(_GEN_GUARD):
    _gen = os.path.join(_here, "gen_rvtest_header.py")
    if os.path.isfile(_gen):
        env = os.environ.copy()
        env[_GEN_GUARD] = "1"        # 让子进程不要再跑这一段
        subprocess.run(
            [sys.executable, _gen],
            check=True,
            cwd=_here,
            env=env,
        )

# ---------------------------------------------------------------------------
# 原有逻辑：导入 patches 模块，注册 _Patcher 等
# ---------------------------------------------------------------------------
try:
    import patches  # noqa: F401
except ImportError:
    pass