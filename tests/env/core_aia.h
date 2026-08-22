.macro CHECK_ILL_NUM EXPECT, ADDER, ACTUAL
  addi \EXPECT, \EXPECT, \ADDER
  bne  \EXPECT, \ACTUAL, TEST_FAIL
.endm

.macro CHECK_MISELECT_INDEX INDEXREG, INDEX, EXPECT_ILL, ILL_ADDER, ACTUAL_ILL, EXPECT_VILL, VILL_ADDER, ACTUAL_VILL

  li    \INDEXREG, \INDEX
  csrw  miselect, \INDEXREG
  csrr  \INDEXREG, miselect
  CHECK_ILL_NUM \EXPECT_ILL, \ILL_ADDER, \ACTUAL_ILL
  CHECK_ILL_NUM \EXPECT_VILL, \VILL_ADDER, \ACTUAL_VILL

.endm

.macro CHECK_MIREG_IMM64 IMMREG, IMM64, EXPECT_ILL, ILL_ADDER, ACTUAL_ILL, EXPECT_VILL, VILL_ADDER, ACTUAL_VILL
 
  li    \IMMREG, \IMM64
  csrw  mireg, \IMMREG
  csrr  \IMMREG, mireg
  CHECK_ILL_NUM \EXPECT_ILL, \ILL_ADDER, \ACTUAL_ILL
  CHECK_ILL_NUM \EXPECT_VILL, \VILL_ADDER, \ACTUAL_VILL

.endm

.macro CHECK_SISELECT_INDEX INDEXREG, INDEX, EXPECT_ILL, ILL_ADDER, ACTUAL_ILL, EXPECT_VILL, VILL_ADDER, ACTUAL_VILL

  li    \INDEXREG, \INDEX
  csrw  siselect, \INDEXREG
  csrr  \INDEXREG, siselect
  CHECK_ILL_NUM \EXPECT_ILL, \ILL_ADDER, \ACTUAL_ILL
  CHECK_ILL_NUM \EXPECT_VILL, \VILL_ADDER, \ACTUAL_VILL

.endm

.macro CHECK_SIREG_IMM64 IMMREG, IMM64, EXPECT_ILL, ILL_ADDER, ACTUAL_ILL, EXPECT_VILL, VILL_ADDER, ACTUAL_VILL
 
  li    \IMMREG, \IMM64
  csrw  sireg, \IMMREG
  csrr  \IMMREG, sireg
  CHECK_ILL_NUM \EXPECT_ILL, \ILL_ADDER, \ACTUAL_ILL
  CHECK_ILL_NUM \EXPECT_VILL, \VILL_ADDER, \ACTUAL_VILL

.endm

.macro CHECK_VSISELECT_INDEX INDEXREG, INDEX, EXPECT_ILL, ILL_ADDER, ACTUAL_ILL, EXPECT_VILL, VILL_ADDER, ACTUAL_VILL

  li    \INDEXREG, \INDEX
  csrw  vsiselect, \INDEXREG
  csrr  \INDEXREG, vsiselect
  CHECK_ILL_NUM \EXPECT_ILL, \ILL_ADDER, \ACTUAL_ILL
  CHECK_ILL_NUM \EXPECT_VILL, \VILL_ADDER, \ACTUAL_VILL

.endm

.macro CHECK_VSIREG_IMM64 IMMREG, IMM64, EXPECT_ILL, ILL_ADDER, ACTUAL_ILL, EXPECT_VILL, VILL_ADDER, ACTUAL_VILL
 
  li    \IMMREG, \IMM64
  csrw  vsireg, \IMMREG
  csrr  \IMMREG, vsireg
  CHECK_ILL_NUM \EXPECT_ILL, \ILL_ADDER, \ACTUAL_ILL
  CHECK_ILL_NUM \EXPECT_VILL, \VILL_ADDER, \ACTUAL_VILL

.endm

.set   ACLINT_BASE_OFFSET,          0x9fef0000
.set   APLIC_BASE_OFFSET,           0x9fee8000
.set   DOMAINCFG_OFFSET,            0x0000
.set   SOURCECFG1_OFFSET,           0x0004
.set   MMSIADDRCFG_OFFSET,          0x1bc0
.set   MMSIADDRCFGH_OFFSET,         0x1bc4
.set   SMSIADDRCFG_OFFSET,          0x1bc8
.set   SMSIADDRCFGH_OFFSET,         0x1bcc
.set   SETIP0_OFFSET,               0x1c00
.set   SETIPNUM_OFFSET,             0x1cdc
.set   IN_CLRIP0_OFFSET,            0x1d00
.set   CLRIPNUM_OFFSET,             0x1ddc
.set   SETIE0_OFFSET,               0x1e00
.set   SETIENUM_OFFSET,             0x1edc
.set   CLRIE0_OFFSET,               0x1f00
.set   CLRIENUM_OFFSET,             0x1fdc
.set   SETIPNUM_LE_OFFSET,          0x2000
.set   SETIPNUM_BE_OFFSET,          0x2004
.set   GENMSI_OFFSET,               0x3000
.set   TARGET1_OFFSET,              0x3004

.macro MSIADDRCFG_INIT MPPN, SPPN, HHXW, LHXW, HHXS, MLHXS, SLHXS

  li x1, APLIC_BASE_OFFSET
  li x3, DOMAINCFG_OFFSET
  #Mdomain base
  add x1, x1, x3
  li x3, MMSIADDRCFG_OFFSET
  #Mdomain mmsiaddrcfg
  add x3, x1, x3
  li x4, \MPPN
  li x5, 0xffffffff
  and x5, x5, x4
  sw x5, 0(x3)
  #Mdomain mmsiaddrcfgh
  srli x5, x4, 32
  li x4, \LHXW
  slli x4, x4, 12
  add x5, x5, x4
  li x4, \HHXW
  slli x4, x4, 16
  add x5, x5, x4
  li x4, \MLHXS
  slli x4, x4, 20
  add x5, x5, x4
  li x4, \HHXS
  slli x4, x4, 24
  add x5, x5, x4
  addi x3, x3, 4
  sw x5, 0(x3)

  #Mdomain smsiaddrcfg
  addi x3, x3, 4
  li x4, \SPPN
  li x5, 0xffffffff
  and x5, x5, x4
  sw x5, 0(x3)
  #Mdomain smsiaddrcfgh
  srli x5, x4, 32
  li x4, \SLHXS
  slli x4, x4, 20
  add x5, x5, x4
  addi x3, x3, 4
  sw x5, 0(x3)
  
.endm
