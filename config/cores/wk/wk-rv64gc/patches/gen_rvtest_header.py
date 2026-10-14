#!/usr/bin/env python3

import re
import sys
from pathlib import Path

def inject_rvtest_failure_code(src_path: Path, dst_path: Path) -> int:
    ENTRY_RE = re.compile(r"^(\s*)(failedtest_\w*?x\d+_x\d+:)\s*$")
    INJECT = "        j rvmodel_halt_fail"

    if not src_path.exists():
        print(f"ERROR: source not found: {src_path}", file=sys.stderr)
        return 1

    src_text = src_path.read_text()
    lines = src_text.splitlines(keepends=True)

    out = []
    injected = 0
    skip_next = False  # 用于避免重复注入（幂等性）

    for i, line in enumerate(lines):
        out.append(line)

        m = ENTRY_RE.match(line.rstrip("\n"))
        if not m:
            continue

        indent, label = m.group(1), m.group(2)

        # 检查下一行是否已经是 j rvmodel_halt_fail（幂等）
        nxt = lines[i + 1].strip() if i + 1 < len(lines) else ""
        if nxt == "j rvmodel_halt_fail":
            continue

        # 用标签的缩进 + 4 空格作为注入行的缩进
        inject_line = f"{INJECT}\n"
        out.append(inject_line)
        injected += 1

    dst_path.write_text("".join(out))
    return 0

def inject_rvtest_setup(src_path: Path, dst_path: Path) -> int:
    TARGET_ADDRS = r"(?:RVMODEL_MTIME_ADDRESS|RVMODEL_MTIMECMP_ADDRESS|RVMODEL_MSIP_ADDRESS)"
    LA_RE = re.compile(
        rf"\bLA\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*,\s*{TARGET_ADDRS}\s*\)"
    )

    if not src_path.exists():
        print(f"ERROR: source not found: {src_path}", file=sys.stderr)
        return 1

    src_text = src_path.read_text()

    def repl(m: "re.Match[str]") -> str:
        reg = m.group(1)
        # 从原始匹配文本里提取地址宏名，保留原样大小写
        addr = m.group(0).rsplit(",", 1)[1].strip().rstrip(")")
        return f"LI({reg}, {addr})"

    new_text, n = LA_RE.subn(repl, src_text)

    dst_path.write_text(new_text)
    return 0

def inject_riscv_arch_test(src_path: Path, dst_path: Path) -> int:
    if not src_path.exists():
        print(f"ERROR: source not found: {src_path}", file=sys.stderr)
        return 1

    dst_path.write_text(src_path.read_text())
    return 0

def main() -> int:
    # ---------- 路径配置 ----------
    ACT4_ROOT = Path("/home/lony/proj/riscv-arch-test-act4")
    SRC = ACT4_ROOT / "tests/env/rvtest_failure_code.h"
    DST = Path(__file__).resolve().parent / "rvtest_failure_code.h"
    inject_rvtest_failure_code(SRC, DST)

    SRC = ACT4_ROOT / "tests/env/rvtest_setup.h"
    DST = Path(__file__).resolve().parent / "rvtest_setup.h"
    inject_rvtest_setup(SRC, DST)

    SRC = ACT4_ROOT / "tests/env/riscv_arch_test.h"
    DST = Path(__file__).resolve().parent / "riscv_arch_test.h"
    inject_riscv_arch_test(SRC, DST)

    return 0

if __name__ == "__main__":
    sys.exit(main())