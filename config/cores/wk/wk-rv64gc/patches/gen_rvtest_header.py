#!/usr/bin/env python3

import re
import shutil
import sys
from pathlib import Path

# ---------------------------------------------------------------------------
# 共享常量：供 patches.py 导入，避免宏列表 / 正则重复
# ---------------------------------------------------------------------------
MMIO_ADDR_MACROS = (
    "RVMODEL_MTIME_ADDRESS",
    "RVMODEL_MTIMECMP_ADDRESS",
    "RVMODEL_MSIP_ADDRESS",
)

# LA(reg, <MMIO 宏>) -> 需要被 LI(reg, <MMIO 宏>) 替换
# group(1) = 目标寄存器，group(2) = 地址宏名
LA_TO_LI_RE = re.compile(
    r"\bLA\(\s*([A-Za-z_]\w*)\s*,\s*("
    + "|".join(MMIO_ADDR_MACROS)
    + r")\s*\)"
)


def inject_rvtest_failure_code(src_path: Path, dst_path: Path) -> int:
    ENTRY_RE = re.compile(r"^(\s*)(failedtest_\w*?x\d+_x\d+:)\s*$")

    if not src_path.exists():
        print(f"ERROR: source not found: {src_path}", file=sys.stderr)
        return 1

    lines = src_path.read_text().splitlines(keepends=True)

    out = []
    injected = 0

    for i, line in enumerate(lines):
        out.append(line)

        m = ENTRY_RE.match(line.rstrip("\r\n"))
        if not m:
            continue

        indent = m.group(1)

        # 幂等：向下扫描第一个有效行（跳过空行与注释）判断是否已注入
        already = False
        for nxt in lines[i + 1:]:
            stripped = nxt.strip()
            if not stripped or stripped.startswith("#") or stripped.startswith("//"):
                continue
            if stripped == "j rvmodel_halt_fail":
                already = True
            break

        if already:
            continue

        # 用标签缩进 + 4 空格注入，与注释保持一致
        out.append(f"{indent}    j rvmodel_halt_fail\n")
        injected += 1

    dst_path.write_text("".join(out))
    return 0


def inject_rvtest_setup(src_path: Path, dst_path: Path) -> int:
    HOOK_RE = re.compile(
        r"("
        r"cleanup_epilogs:[^\n]*\n"
        r"[ \t]*#ifdef STANDARD_SM_SUPPORTED[^\n]*\n"
        r"([ \t]*)RVTEST_TSBI_GOTO_MMODE[^\n]*\n"
        r")",
        re.MULTILINE,
    )

    if not src_path.exists():
        print(f"ERROR: source not found: {src_path}", file=sys.stderr)
        return 1

    src_text = src_path.read_text()

    # 1) LA(reg, MMIO) -> LI(reg, MMIO)
    def repl_la(m: "re.Match[str]") -> str:
        reg = m.group(1)
        addr = m.group(2)
        return f"LI({reg}, {addr})"

    new_text = LA_TO_LI_RE.sub(repl_la, src_text)

    # 2) 在 RVTEST_TSBI_GOTO_MMODE 之后插入 RVTEST_POST_MMODE_HOOK
    if "RVTEST_POST_MMODE_HOOK" not in new_text:
        def repl_hook(m: "re.Match[str]") -> str:
            block = m.group(1)
            indent = m.group(2)
            return f"{block}{indent}RVTEST_POST_MMODE_HOOK\n"

        new_text = HOOK_RE.sub(repl_hook, new_text, count=1)

    dst_path.write_text(new_text)
    return 0


def inject_riscv_arch_test(src_path: Path, dst_path: Path) -> int:
    if not src_path.exists():
        print(f"ERROR: source not found: {src_path}", file=sys.stderr)
        return 1
    shutil.copyfile(src_path, dst_path)
    return 0


def main() -> int:
    act4_root = Path.cwd()
    out_dir = Path(__file__).resolve().parent

    steps = (
        (act4_root / "tests/env/rvtest_failure_code.h",
         out_dir / "override_header/rvtest_failure_code.h",
         inject_rvtest_failure_code),
        (act4_root / "tests/env/rvtest_setup.h",
         out_dir / "override_header/rvtest_setup.h",
         inject_rvtest_setup),
        (act4_root / "tests/env/riscv_arch_test.h",
         out_dir / "override_header/riscv_arch_test.h",
         inject_riscv_arch_test),
    )

    for src, dst, fn in steps:
        rc = fn(src, dst)
        if rc != 0:
            return rc

    return 0


if __name__ == "__main__":
    sys.exit(main())