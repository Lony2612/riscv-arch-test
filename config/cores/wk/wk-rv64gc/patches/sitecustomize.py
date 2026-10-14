import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
if _here not in sys.path:
    sys.path.insert(0, _here)

_repo = os.environ.get("ACT4_REPO_ROOT")
if _repo:
    _repo_abs = os.path.abspath(_repo)
    if _repo_abs not in sys.path:
        sys.path.insert(0, _repo_abs)

# ---------------------------------------------------------------------------
# 导入 patches 模块，注册 _Patcher 等
#
# 语义区分：
#   - `import patches` 自身找不到 patches 模块 → 说明当前不在项目目录下，正常跳过；
#   - patches 内部的依赖（act / gen_rvtest_header / ...）导入失败 → 说明补丁已
#     残缺，应显式报错而不是静默吞掉。
# ---------------------------------------------------------------------------
try:
    import patches  # noqa: F401
except ImportError as e:
    if getattr(e, "name", None) == "patches":
        # patches 不在 sys.path 上：静默跳过
        pass
    else:
        sys.stderr.write(f"sitecustomize: patches import failed: {e}\n")
        raise