/*****************************************************************************
* Space-T Co., Ltd. Confidential                                             *
* -------------------------------                                            *
* This file and all its contents are properties of Space-T Co., Ltd.         *
* The information contained herein is confidential and proprietary           *
* and is not to be disclosed outside of Space-T Co., Ltd. except under       *
* a Non-Disclosure Agreement (NDA).                                          *
*****************************************************************************/
.include "csr_bit.h"

.macro SETMEXP EXP_CODE, HANDLER_BEGIN,HANDLER_END
  sd   t0, -24(sp)   #address cann't be changed 
  sd   t1, -32(sp)   #it relates to crt0.s

  la   t1, vector_table
  addi t0,x0,\EXP_CODE
  slli t0,t0,0x3
  add  t1,t1,t0 
  
  la   t0, \HANDLER_BEGIN
  sd   t0, 0(t1) 

  ld   t1, -32(sp)
  ld   t0, -24(sp)
  j    \HANDLER_END 

  ld   a4, -16(sp)
  csrrw x2,mscratch,x2
.endm

.macro SETSEXP EXP_CODE, HANDLER_BEGIN,HANDLER_END
  sd   t0, -24(sp)   #address cann't be changed 
  sd   t1, -32(sp)   #it relates to crt0.s

  la   t1, vector_table_s
  addi t0,x0,\EXP_CODE
  slli t0,t0,0x3
  add  t1,t1,t0 
  
  la   t0, \HANDLER_BEGIN
  sd   t0, 0(t1) 

  ld   t1, -32(sp)
  ld   t0, -24(sp)
  j    \HANDLER_END 

  ld   a4, -16(sp)
  csrrw x2,sscratch,x2
.endm



.macro SETVSEXP EXP_CODE, HANDLER_BEGIN,HANDLER_END
  sd   t0, -24(sp)   #address cann't be changed 
  sd   t1, -32(sp)   #it relates to crt0.s

  la   t1, vector_table_vs
  addi t0,x0,\EXP_CODE
  slli t0,t0,0x3
  add  t1,t1,t0 
  
  la   t0, \HANDLER_BEGIN
  sd   t0, 0(t1) 

  ld   t1, -32(sp)
  ld   t0, -24(sp)
  j    \HANDLER_END 

  ld   a4, -16(sp)
  csrrw x2,sscratch,x2
.endm


.macro SETINT EXP_CODE, HANDLER_BEGIN,HANDLER_END
  sd   t0, -24(sp)   #address cann't be changed
  sd   t1, -32(sp)   #it relates to crt0.s

  la   t1, vector_table + 128  #intterrupt start address
  addi t0,x0,\EXP_CODE
  slli t0,t0,0x3
  add  t1,t1,t0   

  la   t0, \HANDLER_BEGIN
  sd   t0, 0(t1) 

  ld   t1, -32(sp)
  ld   t0, -24(sp)
  j    \HANDLER_END
 
  ld   a4, -16(sp)
  csrrw x2,mscratch,x2
.endm


.macro SETSINT EXP_CODE, HANDLER_BEGIN,HANDLER_END
  sd   t0, -24(sp)   #address cann't be changed
  sd   t1, -32(sp)   #it relates to crt0.s

  la   t1, vector_table_s + 128  #intterrupt start address
  addi t0,x0,\EXP_CODE
  slli t0,t0,0x3
  add  t1,t1,t0   

  la   t0, \HANDLER_BEGIN
  sd   t0, 0(t1) 

  ld   t1, -32(sp)
  ld   t0, -24(sp)
  j    \HANDLER_END
 
  ld   a4, -16(sp)
  csrrw x2,sscratch,x2
.endm



.macro SETVSINT EXP_CODE, HANDLER_BEGIN,HANDLER_END
  sd   t0, -24(sp)   #address cann't be changed
  sd   t1, -32(sp)   #it relates to crt0.s

  la   t1, vector_table_vs + 128  #intterrupt start address
  addi t0,x0,\EXP_CODE
  slli t0,t0,0x3
  add  t1,t1,t0   

  la   t0, \HANDLER_BEGIN
  sd   t0, 0(t1) 

  ld   t1, -32(sp)
  ld   t0, -24(sp)
  j    \HANDLER_END
 
  ld   a4, -16(sp)
  csrrw x2,sscratch,x2
.endm

.macro MMODE_STATUS_TVAL_CHK
csrr x28,htval
csrr x28,mtval
csrr x28,stval
csrr x28,vstval
csrr x28,mstatus
csrr x28,sstatus
csrr x28,hstatus
csrr x28,vsstatus
.endm



.macro CSR_BIT_CHK , CSR_ADDR, CSR_BIT, IMM
li x9,\CSR_BIT
csrr x11, \CSR_ADDR
and x11,x11,x9
li x10,\IMM
bne x11,x10,TEST_FAIL
#li x10, \CSR_ADDR
.endm

.macro CSR_F_CHK , CSR_ADDR ,IMM
li x7, 0xffffffffffffffff
csrw \CSR_ADDR, x7
csrr x8, \CSR_ADDR
li x9, \IMM
bne x9,x8, TEST_FAIL
.endm

.macro CSR_CHK_HEDELEG
li x7, 0xffffffffffffffff
csrw hedeleg, x7
csrr x8, hedeleg
li x9, 0x00000000b1fe
bne x9,x8, TEST_FAIL
.endm



.macro CHK_ALL_H_INST_ILL_WHEN_H_OFF
hlv.b x6,0(x7)
CHK_ILL_INST
hlv.bu x6,0(x7)
CHK_ILL_INST
hlv.h x6,0(x7)
CHK_ILL_INST
hlv.hu x6,0(x7)
CHK_ILL_INST
hlv.w x6,0(x7)
CHK_ILL_INST
hlv.wu x6,0(x7)
CHK_ILL_INST
hlv.d x6,0(x7)
CHK_ILL_INST
hsv.b x6,0(x7)
CHK_ILL_INST
hsv.h x6,0(x7)
CHK_ILL_INST
hsv.w x6,0(x7)
CHK_ILL_INST
hsv.d x6,0(x7)
CHK_ILL_INST
hlvx.hu x6,0(x7)
CHK_ILL_INST
hlvx.wu x6,0(x7)
CHK_ILL_INST
hfence.gvma x0,x0
CHK_ILL_INST

hfence.vvma x0,x0
CHK_ILL_INST

.endm



.macro CSR_0_CHK
.endm

.macro CSR_5_CHK
.endm


.macro CSR_A_CHK
.endm

.macro CHK_NEW_M_CSR
csrr x12, mtval2
#CHK_ILL_INST
csrw mtval2, x12
#CHK_ILL_INST
csrr x12, mtinst
#CHK_ILL_INST
csrw mtinst, x12
#CHK_ILL_INST
.endm

.macro CHK_S_CSR_WHEN_VUMODE
csrr x13, sstatus
CHK_VIR_INST
csrr x13, stvec
CHK_VIR_INST
csrr x13, sip
CHK_VIR_INST
csrr x13, sie
CHK_VIR_INST
csrr x13, scounteren
CHK_VIR_INST
csrr x13, sscratch
CHK_VIR_INST
csrr x13, sepc
CHK_VIR_INST
csrr x13, scause
CHK_VIR_INST
csrr x13, stval
CHK_VIR_INST
csrr x13, senvcfg
CHK_VIR_INST
csrr x13, satp
CHK_VIR_INST
.endm



.macro ENABLE_HYPER
li x12, 0x80
csrs misa, x12
.endm

.macro DISABLE_HYPER
li x12, 0x80
csrc misa, x12
.endm

.macro CHK_TRAP_VALUE VAL VAL2 TINST
li x31, \VAL2
bne x31, x28, TEST_FAIL
li x31, \VAL
bne x31, x29, TEST_FAIL
li x31, \TINST
bne x31, x30, TEST_FAIL
.endm




.macro CHK_HYPER_BIT_IN_M_CSR_WHEN_H_OFF
csrr x12, mtval2
CHK_ILL_INST
csrw mtval2, x12
CHK_ILL_INST
csrr x12, mtinst
CHK_ILL_INST
csrw mtinst, x12
CHK_ILL_INST
CSR_BIT_SET MSTATUS MSTATUS_MPV
CSR_BIT_SET MSTATUS MSTATUS_GVA
CSR_BIT_CHK MSTATUS MSTATUS_MPV 0x0
CSR_BIT_CHK MSTATUS MSTATUS_GVA 0x0
li x16, 0xffffffffffffffff
li x17,0x2222
csrc mideleg, x16
csrw mip,x16
csrr x18, mip
bne x18, x17, TEST_FAIL
csrc mip,x16
li x17, 0x2aaa
csrw mie,x16
csrr x18, mie
bne x18, x17, TEST_FAIL
csrc mie,x16
li x15,0x4000000000000000
csrc menvcfg,x15
csrs henvcfg,x15
CHK_ILL_INST
.endm


.macro CHK_HYPER_CSR_READ_WHEN_VUMODE
csrr x15, henvcfg
CHK_VIR_INST

csrr x14, hstatus
CHK_VIR_INST

csrr x14, hideleg
CHK_VIR_INST

csrr x14, hedeleg
CHK_VIR_INST

csrr x14, hvip
CHK_VIR_INST

csrr x14, hip
CHK_VIR_INST

csrr x14, hie
CHK_VIR_INST

csrr x14, hgeie
CHK_VIR_INST

csrr x14, hgeip
CHK_VIR_INST

csrr x14, hcounteren
CHK_VIR_INST

csrr x14, htimedelta
CHK_VIR_INST

csrr x14, htval
CHK_VIR_INST

csrr x14, htinst
CHK_VIR_INST

csrr x14, hgatp
CHK_VIR_INST

csrr x14, vsstatus
CHK_VIR_INST

csrr x14, vsip
CHK_VIR_INST

csrr x14, vsie
CHK_VIR_INST

csrr x14, vstvec
CHK_VIR_INST

csrr x14, vsscratch
CHK_VIR_INST

csrr x14, vsepc
CHK_ILL_INST

csrr x14, vscause
CHK_VIR_INST

csrr x14, vstval
CHK_VIR_INST

csrr x14, vsatp
CHK_VIR_INST
.endm

.macro CHK_CUR_MODE, PM, V

.endm

.macro CHK_HS_MODE
.endm

.macro CHK_HYPER_CSR_WRITE_WHEN_H_OFF
li  x15, 0x1
csrw  hstatus, x15
CHK_ILL_INST

csrw  hideleg, x15
CHK_ILL_INST

csrw  hedeleg, x15
CHK_ILL_INST

csrw  hvip, x15
CHK_ILL_INST

csrw  hip, x15
CHK_ILL_INST

csrw  hie, x15
CHK_ILL_INST

csrw  hgeie, x15
CHK_ILL_INST

csrw  hgeip, x15
CHK_ILL_INST

csrw  henvcfg, x15
CHK_ILL_INST

csrw  hcounteren, x15
CHK_ILL_INST

csrw  htimedelta, x15
CHK_ILL_INST

csrw  htval, x15
CHK_ILL_INST

csrw  htinst, x15
CHK_ILL_INST

csrw  hgatp, x15
CHK_ILL_INST

csrw  vsstatus, x15
CHK_ILL_INST

csrw  vsip, x15
CHK_ILL_INST

csrw  vsie, x15
CHK_ILL_INST

csrw  vstvec, x15
CHK_ILL_INST

csrw  vsscratch, x15
CHK_ILL_INST

csrw  vsepc, x15
CHK_ILL_INST

csrw  vscause, x15
CHK_ILL_INST

csrw  vstval, x15
CHK_ILL_INST

csrw  vsatp, x15
CHK_ILL_INST
.endm

.macro CHK_HYPER_CSR_WRITE_WHEN_VUMODE
li  x15, 0x1
csrw  hstatus, x15
CHK_VIR_INST

csrw  hideleg, x15
CHK_VIR_INST

csrw  hedeleg, x15
CHK_VIR_INST

csrw  hvip, x15
CHK_VIR_INST

csrw  hip, x15
CHK_VIR_INST

csrw  hie, x15
CHK_VIR_INST

csrw  hgeie, x15
CHK_VIR_INST

csrw  hgeip, x15
CHK_VIR_INST

csrw  henvcfg, x15
CHK_VIR_INST

csrw  hcounteren, x15
CHK_VIR_INST

csrw  htimedelta, x15
CHK_VIR_INST

csrw  htval, x15
CHK_VIR_INST

csrw  htinst, x15
CHK_VIR_INST

csrw  hgatp, x15
CHK_VIR_INST

csrw  vsstatus, x15
CHK_VIR_INST

csrw  vsip, x15
CHK_VIR_INST

csrw  vsie, x15
CHK_VIR_INST

csrw  vstvec, x15
CHK_VIR_INST

csrw  vsscratch, x15
CHK_VIR_INST

csrw  vsepc, x15
CHK_VIR_INST

csrw  vscause, x15
CHK_VIR_INST

csrw  vstval, x15
CHK_VIR_INST

csrw  vsatp, x15
CHK_VIR_INST
.endm


.macro CHK_VIR_INST, IMM
li x21, \IMM
bne x20, x21, TEST_FAIL
li x20, 0x0
.endm


.macro CHK_ALL_H_INST_ILL_WHEN_VUMODE
hlv.b x6,0(x7)
CHK_VIR_INST
hlv.bu x6,0(x7)
CHK_VIR_INST
hlv.h x6,0(x7)
CHK_VIR_INST
hlv.hu x6,0(x7)
CHK_VIR_INST
hlv.w x6,0(x7)
CHK_VIR_INST
hlv.wu x6,0(x7)
CHK_VIR_INST
hlv.d x6,0(x7)
CHK_VIR_INST
hsv.b x6,0(x7)
CHK_VIR_INST
hsv.h x6,0(x7)
CHK_VIR_INST
hsv.w x6,0(x7)
CHK_VIR_INST
hsv.d x6,0(x7)
CHK_VIR_INST
hlvx.hu x6,0(x7)
CHK_VIR_INST
hlvx.wu x6,0(x7)
CHK_VIR_INST
hfence.gvma x0,x0
CHK_VIR_INST

hfence.vvma x0,x0
CHK_VIR_INST

.endm




.macro CHK_ILL_INST , IMM#check reg value
li x21, \IMM

bne x20, x21, TEST_FAIL
li x20, 0x0
.endm

.macro CSR_BIT_SET , CSR_ADDR, CSR_BIT
li x9,\CSR_BIT
#li x10, \CSR_ADDR
csrs \CSR_ADDR ,x9
.endm

.macro CSR_BIT_CLR , CSR_ADDR, CSR_BIT
li x9,\CSR_BIT
#li x10, \CSR_ADDR
csrc \CSR_ADDR ,x9
.endm

.macro CHK_WRITE_VALUE,IMM,IMM2,result1,result2
li x11, \IMM
li x18, \result1
mul x11, x11,x18
li x12,\IMM2
li x18, \result2
mul x12, x12,x18
bne x11,x13,TEST_FAIL
.endm


.macro CSR_VALUE_CHK2,CSR_ADDR,EXP_TYPE,WrData0,WrData1,WrData2
  #backup regs
  addi	x2, x2, -72
  sd	x9, 0(x2)
  sd	x20, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x18, 40(x2)
  sd	x21, 48(x2)
  sd	x22, 56(x2)
  sd	x23, 64(x2)

  #get excepted value from input
  li x20, 0x0
  li x9,0x0
  li x11,\WrData1
  li x12,\WrData2
  li x13,0x0 

  # write csr with wrdata1
  csrw \CSR_ADDR ,x9
  li x18, \EXP_TYPE
  andi x21, x18, 0x1
  li x23,0x333 # write value in expt handler; can change 
  mul x21,x21,x23
  bne x20, x21, TEST_FAIL #first check if ill inst
  li x20, 0x0
  andi x22, x18, 0x2
  li x23,0x444 # write value in expt handler; can change
  mul x22,x22,x23
  bne x20, x22, TEST_FAIL # then check if virtual inst 
  li x20, 0x0
  csrr x13,\CSR_ADDR # read csr 
  li x18, \EXP_TYPE
  andi x21, x18, 0x1
  li x23,0x333
  mul x21,x21,x23
  bne x20, x21, TEST_FAIL# chk ill/vir expt 
  li x20, 0x0
  andi x22, x18, 0x2
  li x23,0x444
  mul x22,x22,x23
  bne x20, x22, TEST_FAIL
  li x20, 0x0
  addi x18,x18,0x3# get expected vaule for self check 
  andi x18,x18,0x2# if ill/vir expected all 0
  srli x18,x18,0x1# if no expect ; expected wrdata
  mul x11, x11,x18
  mul x12, x12,x18
  bne x11,x13,TEST_FAIL
  li x13,0x0# repeat the same way in wrdata2
  li x9,0xffffffffffffffff
  csrw \CSR_ADDR ,x9
  csrr x13,\CSR_ADDR
  bne x12,x13, TEST_FAIL

#restore regs
  ld	x9, 0(x2)
  ld	x20, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x18, 40(x2)
  ld	x21, 48(x2)
  ld	x22, 56(x2)
  ld	x23, 64(x2)
  addi	x2, x2,72


.endm


#.macro CSR_VALUE_CHK,CSR_ADDR,CSR_0,CSR_F
##li x14, 0x2
##li x15, \EXPT
##bne x14 ,x15, ill_inst_check
#li x9,0x0
#li x11,\CSR_0
#li x12,\CSR_F
#li x13,0x0 
##li x10, \CSR_ADDR
#csrw \CSR_ADDR ,x9
##CHK_ILL_INST \ILL
##CHK_VIR_INST \VIR
#csrr x13,\CSR_ADDR
##CHK_ILL_INST \ILL
##CHK_VIR_INST \VIR
##CHK_WRITE_VALUE \WV1 \WV2 \CSR_0 \CSR_F 
#bne x11, x13 , TEST_FAIL
#li x13,0x0
#li x9,0xffffffffffffffff
#csrw \CSR_ADDR ,x9
#csrr x13,\CSR_ADDR
#bne x12,x13, TEST_FAIL
#.endm


.macro CSR_VALUE_CHK1,CSR_ADDR,CSR_0,CSR_F, ILL, VIR, WV1, WV2
#li x14, 0x2
#li x15, \EXPT
#bne x14 ,x15, ill_inst_check
li x9,0x0
li x11,\CSR_0
li x12,\CSR_F
li x13,0x0 
#li x10, \CSR_ADDR
csrw \CSR_ADDR ,x9
CHK_ILL_INST \ILL
CHK_VIR_INST \VIR
csrr x13,\CSR_ADDR
CHK_ILL_INST \ILL
CHK_VIR_INST \VIR
CHK_WRITE_VALUE \WV1 \WV2 \CSR_0 \CSR_F 
bne x11, x13 , TEST_FAIL
li x13,0x0
li x9,0xffffffffffffffff
csrw \CSR_ADDR ,x9
csrr x13,\CSR_ADDR
bne x12,x13, TEST_FAIL
.endm

.macro CSR_RESET_VALUE_CHK

csrr x6, hedeleg
bne x6,x0, TEST_FAIL

csrr x6, hideleg
bne x6,x0, TEST_FAIL

csrr x6, hvip
bne x6,x0, TEST_FAIL

csrr x6, hip
bne x6,x0, TEST_FAIL

csrr x6, hie
bne x6,x0, TEST_FAIL

csrr x6, hgeie
bne x6,x0, TEST_FAIL

csrr x6, hgeip
bne x6,x0, TEST_FAIL

#csrr x6, henvcfg
#bne x6,x0, TEST_FAIL

csrr x6, hcounteren
bne x6,x0, TEST_FAIL

csrr x6, htimedelta
bne x6,x0, TEST_FAIL


csrr x6, htval
bne x6,x0, TEST_FAIL

csrr x6, htinst
bne x6,x0, TEST_FAIL

csrr x6, hgatp
bne x6,x0, TEST_FAIL

csrr x6, vsip
bne x6,x0, TEST_FAIL

csrr x6, vsie
bne x6,x0, TEST_FAIL

#csrr x6, vstvec
#bne x6,x0, TEST_FAIL

csrr x6, vsscratch
bne x6,x0, TEST_FAIL

csrr x6, vsepc
bne x6,x0, TEST_FAIL

csrr x6, vscause
bne x6,x0, TEST_FAIL

csrr x6, vstval
bne x6,x0, TEST_FAIL

csrr x6, vsatp
bne x6,x0, TEST_FAIL
.endm



.set PLICBASE ,0xffffffe000000000
.set PLICBASE_M  ,0x6000000000
.set INTPRIO ,0x0
.set INTPEND ,0x1000
.set INTIE ,0x2000
.set INTIE_HART ,0x80
.set INTTH ,0x200000
.set INTCLAIM ,0x200004
.set INTHC_HART ,0x1000
