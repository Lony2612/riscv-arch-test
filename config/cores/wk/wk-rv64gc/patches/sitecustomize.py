import os, sys

_here = os.path.dirname(os.path.abspath(__file__))
if _here not in sys.path:
    sys.path.insert(0, _here)

_repo = os.environ.get("ACT4_REPO_ROOT")
if _repo and _repo not in sys.path:
    sys.path.insert(0, _repo)

try:
    import patches  # noqa: F401
except ImportError:
    pass