import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
if _here not in sys.path:
    sys.path.insert(0, _here)

_repo = os.environ.get("ACT4_REPO_ROOT")
if _repo and _repo not in sys.path:
    sys.path.insert(0, _repo)

# ---------------------------------------------------------------------------
# 原有逻辑：导入 patches 模块，注册 _Patcher 等
# ---------------------------------------------------------------------------
try:
    import patches  # noqa: F401
except ImportError:
    pass