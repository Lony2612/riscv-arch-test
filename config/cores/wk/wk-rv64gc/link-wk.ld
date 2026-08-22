/* ================================================================
   用户可修改的配置（匹配您的 DUT 内存映射）
   ================================================================ */
IRAM_ORIGIN = 0x00000000;      /* 指令存储器基址 */
IRAM_LENGTH = 0x00200000;      /* 指令存储器大小（2MB） */
DRAM_ORIGIN = 0x00200000;      /* 数据存储器基址 */
DRAM_LENGTH = 0x00200000;      /* 数据存储器大小（2MB） */

TEST_BASE = IRAM_ORIGIN;       /* 入口点位于 IRAM 起始 */
NUM_HARTS = 1;                 /* 核心数，按需修改 */

/* 栈大小（通常 128KB 足够） */
STACK_SIZE = 0x20000;

/* ================================================================
   以下为固定部分，一般无需改动
   ================================================================ */
OUTPUT_ARCH("riscv")
ENTRY(rvtest_entry_point)

MEMORY
{
  iram (rwx) : ORIGIN = IRAM_ORIGIN, LENGTH = IRAM_LENGTH   /* 指令存储器，可读可写可执行 */
  dram (rw) : ORIGIN = DRAM_ORIGIN, LENGTH = DRAM_LENGTH   /* 数据存储器，读写 */
}

PROVIDE(__stack_size = STACK_SIZE);
PROVIDE(__num_harts = NUM_HARTS);

SECTIONS
{
  /* 检查入口点是否落在 IRAM 内 */
  ASSERT(TEST_BASE >= ORIGIN(iram) && TEST_BASE < ORIGIN(iram) + LENGTH(iram),
         "TEST_BASE is outside IRAM; adjust IRAM_ORIGIN/IRAM_LENGTH")

  /* ---------- 代码段（放 IRAM） ---------- */
  .text.init   TEST_BASE : { *(.text.init) } > iram
  .text.rvtest . : { *(.text.rvtest) *(.text.rvtest.*) } > iram

  . = ALIGN(0x4000);
  .rodata . : { *(.rodata) *(.rodata.*) *(.srodata) *(.srodata.*) } > iram

  /* 模型代码（Sail 辅助函数等）也放入 IRAM */
  . = ALIGN(0x1000);
  .text.rvmodel . : {
    *(.text.rvmodel) *(.text.rvmodel.*)
    *(.text) *(.text.*)        /* 其余通用代码 */
  } > iram

  /* 检查 IRAM 是否溢出 */
  . = ALIGN(0x1000);
  __iram_end = .;
  ASSERT(__iram_end <= ORIGIN(iram) + LENGTH(iram),
         "IRAM overflow; increase IRAM_LENGTH or reduce code size")

  /* ---------- 数据段（放 DRAM） ---------- */
  . = ALIGN(0x1000);
  .data : { *(.data) *(.data.*) *(.sdata) *(.sdata.*) } > dram

  . = ALIGN(16);
  .bss . : {
    __bss_start = .;
    *(.sbss) *(.sbss.*) *(.bss) *(.bss.*) *(COMMON)
    . = ALIGN(16);
    __bss_end = .;
  } > dram

  /* 栈空间（在 DRAM 顶部） */
  . = ALIGN(16);
  __stack_bottom = .;
  . += __stack_size * __num_harts;
  __stack_top = .;

  /* 检查 DRAM 是否溢出 */
  . = ALIGN(0x1000);
  __dram_end = .;
  ASSERT(__dram_end <= ORIGIN(dram) + LENGTH(dram),
         "DRAM overflow; increase DRAM_LENGTH or reduce data/stack size")

  /* 保留 _end 符号（供 ACT 框架使用，指向 DRAM 结束） */
  _end = __dram_end;
}