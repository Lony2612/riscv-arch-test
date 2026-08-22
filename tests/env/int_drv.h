/*****************************************************************************
* Space-T Co., Ltd. Confidential                                             *
* -------------------------------                                            *
* This file and all its contents are properties of Space-T Co., Ltd.         *
* The information contained herein is confidential and proprietary           *
* and is not to be disclosed outside of Space-T Co., Ltd. except under       *
* a Non-Disclosure Agreement (NDA).                                          *
*****************************************************************************/
.set IDNUMBASE    ,0x99999000
.set IDPCOFFSET   ,0x400
.set IDCNTOFFSET  ,0x800
.set IDTYPEOFFSET ,0xc00

.macro DRVINT_SET INDEX, PC, CNT, TYPE
  addi x2, x2, -56
  sd   x9, 0(x2)
  sd   x10, 8(x2)
  sd   x11, 16(x2)
  sd   x12, 24(x2)
  
  li   x9, IDNUMBASE
  #calc common index offset
  li   x10, \INDEX
  slli x10, x10, 3
  add  x10, x9, x10

  #calc IDPC_ADDR
  li   x11, IDPCOFFSET
  add  x12, x10, x11
  la   x11, \PC
  sd   x11, 0(x12)
  
  #calc IDCNT_ADDR
  li   x11, IDCNTOFFSET
  add  x12, x10, x11
  li   x11, \CNT
  sd   x11, 0(x12)

  #calc IDTYPE_ADDR
  li   x11, IDTYPEOFFSET
  add  x12, x10, x11
  li   x11, \TYPE
  sd   x11, 0(x12)

  #restore regs
  ld   x9, 0(x2)
  ld   x10, 8(x2)
  ld   x11, 16(x2)
  ld   x12, 24(x2)
  addi x2, x2, 56
  # fence 
  fence
.endm

.macro DRVINT_ENABLE NUM
  addi x2, x2, -56
  sd   x9, 0(x2)
  sd   x10, 8(x2)
  
  li   x9, IDNUMBASE
  li   x10, \NUM
  sd   x10,0(x9)

  #restore regs
  ld   x9, 0(x2)
  ld   x10, 8(x2)
  addi x2, x2, 56
  # fence 
  fence
.endm
