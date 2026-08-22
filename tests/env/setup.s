/*****************************************************************************
* Space-T Co., Ltd. Confidential                                             *
* -------------------------------                                            *
* This file and all its contents are properties of Space-T Co., Ltd.         *
* The information contained herein is confidential and proprietary           *
* and is not to be disclosed outside of Space-T Co., Ltd. except under       *
* a Non-Disclosure Agreement (NDA).                                          *
*****************************************************************************/
#start from __start,
#(1)initialize all registers
#(2)initialize supervisor mode stack pointer
#(3)set VBR register
#(4)construct PMA and PMP 
#(5)construct MMU 
#(6)enable control reg such as EE and MMU
#(9)jump to the main procedure using jmp to main
 .include "core_init.h"
 .set MP_DONE_ADDR, 0x9f0f2000
 .set TOTAL_CORE_NUM, 1
 .set RERI_INT_SEL, 0
	.text
	.global	_start
_start:
  # For MP test, all cores do initialization
  # TOTAL_CORE_NUM is passed from compiler options
  li x3, TOTAL_CORE_NUM
  li x4, 0x1
  blt x4, x3, core_init_begin

  # For UP test, only core0 do initialization
  csrr x3,mhartid
  beqz x3, core_init_begin
  wfi

core_init_begin:


# ml2setup
  li   x3, 0x5010f
  csrw 0x7f0, x3

# for mmu
  li   x3, 0x4000000000000000
  #csrs menvcfg, x3
  .long 0x30a1a073


# enable fpu
  li   x3, 0x6000
  csrs mstatus,x3

# enable vector
  li   x3, 0x200
  csrs mstatus,x3

 #  enable ICACHE,DCACHE,BPE,PRFE
  li x3, 0x10033
  csrs 0x7c0,x3  #msetup
# enable icache way pred
  li x3, 0x200
  csrs 0x7d0, x3 #mperf0



# PART 1: initialize all registers
  li  x1, 0
  li  x2, 0
  li  x3, 0
  li  x4, 0
  li  x5, 0
  li  x6, 0
  li  x7, 0
  li  x8, 0
  li  x9, 0
  li  x10,0
  li  x11,0
  li  x12,0
  li  x13,0
  li  x14,0
  li  x15,0
  li  x16,0
  li  x17,0
  li  x18,0
  li  x19,0
  li  x20,0
  li  x21,0
  li  x22,0
  li  x23,0
  li  x24,0
  li  x25,0
  li  x26,0
  li  x27,0
  li  x28,0
  li  x29,0
  li  x30,0
  li  x31,0

  fmv.d.x f0, x0
  fmv.d.x f1, x0
  fmv.d.x f2, x0
  fmv.d.x f3, x0
  fmv.d.x f4, x0
  fmv.d.x f5, x0
  fmv.d.x f6, x0
  fmv.d.x f7, x0
  fmv.d.x f8, x0
  fmv.d.x f9, x0
  fmv.d.x f10, x0
  fmv.d.x f11, x0
  fmv.d.x f12, x0
  fmv.d.x f13, x0
  fmv.d.x f14, x0
  fmv.d.x f15, x0
  fmv.d.x f16, x0
  fmv.d.x f17, x0
  fmv.d.x f18, x0
  fmv.d.x f19, x0
  fmv.d.x f20, x0
  fmv.d.x f21, x0
  fmv.d.x f22, x0
  fmv.d.x f23, x0
  fmv.d.x f24, x0
  fmv.d.x f25, x0
  fmv.d.x f26, x0
  fmv.d.x f27, x0
  fmv.d.x f28, x0
  fmv.d.x f29, x0
  fmv.d.x f30, x0
  fmv.d.x f31, x0

.if HWCFIG_VECTOR==1
  li x31 ,0x1
  vsetvli x31,x0,e8, m1
  vmv.v.x v0,x0
  vmv.v.x v1,x0
  vmv.v.x v2,x0
  vmv.v.x v3,x0
  vmv.v.x v4,x0
  vmv.v.x v5,x0
  vmv.v.x v6,x0
  vmv.v.x v7,x0
  vmv.v.x v8,x0
  vmv.v.x v9,x0
  vmv.v.x v10,x0
  vmv.v.x v11,x0
  vmv.v.x v12,x0
  vmv.v.x v13,x0
  vmv.v.x v14,x0
  vmv.v.x v15,x0
  vmv.v.x v16,x0
  vmv.v.x v17,x0
  vmv.v.x v18,x0
  vmv.v.x v19,x0
  vmv.v.x v20,x0
  vmv.v.x v21,x0
  vmv.v.x v22,x0
  vmv.v.x v23,x0
  vmv.v.x v24,x0
  vmv.v.x v25,x0
  vmv.v.x v26,x0
  vmv.v.x v27,x0
  vmv.v.x v28,x0
  vmv.v.x v29,x0
  vmv.v.x v30,x0
  vmv.v.x v31,x0
.endif

## PART 2: initialize stack pointer & ppn
  csrr x3,mhartid
  li x4,1

  ENABLE_HYPER

  .global cpu_0_sp	
cpu_0_sp:
  la x2, __stack_top
  li x31,0x1000
  mul x31,x31,x3
  add x2,x31,x2
  csrw mscratch,x2
  csrw sscratch,x2
  csrw vsscratch, x2

# PART 3:initialize mtvec value 

  la    x3,__trap_handler
  csrw  mtvec,x3

  la    x3,__trap_handler_s
  csrw  stvec,x3

.if HWCFIG_HYPERVISOR==1
  ENABLE_HYPER
  la    x3,__trap_handler_vs
  csrw  vstvec,x3
.endif


  # enable mie
  li   x3, 0x8
  csrs mstatus,x3

  #  enable ICACHE,DCACHE,BPE,PRFE
  li x3, 0x10033
  csrs 0x7c0,x3  #msetup
 
  .global reri_init	
reri_init:
  li x3,RERI_INT_SEL
  li x4,0x0
  li x5,0x1
  li x6,0x2
  li x7,0x3
  li x8,0x4
  li x9,0x5
  beq x4, x3, after_l2en
  beq x5, x3, reri_high_int_en
  beq x6, x3, reri_low_int_en
  beq x7, x3, reri_plat_int_en
  beq x8, x3, reri_mix_int_en
  beq x9, x3, reri_mix2_int_en

reri_low_int_en:
  #                      ce uu/de low  low
 RERI_CFG_RAS_INIT 0x0 ,0x2 ,0x6 ,0x2 ,0x2 
 RERI_CFG_RAS_INIT 0x1 ,0x2 ,0x6 ,0x2 ,0x2 
 RERI_CFG_RAS_INIT 0x2 ,0x2 ,0x4 ,0x2 ,0x2 
 RERI_CFG_RAS_INIT 0x3 ,0x0 ,0x6 ,0x0 ,0x2 
 RERI_CFG_RAS_INIT 0x4 ,0x2 ,0x0 ,0x2 ,0x0 
 RERI_CFG_RAS_INIT 0x5 ,0x2 ,0x0 ,0x2 ,0x0 
 RERI_CFG_RAS_INIT 0x6 ,0x2 ,0x0 ,0x2 ,0x0 
 RERI_CFG_RAS_INIT 0x7 ,0x2 ,0x0 ,0x2 ,0x0 
 RERI_CFG_RAS_INIT 0x8 ,0x0 ,0x6 ,0x0 ,0x2 
 RERI_CFG_RAS_INIT 0x9 ,0x2 ,0x6 ,0x2 ,0x2 
 RERI_CFG_RAS_INIT 0xa ,0x2 ,0x4 ,0x2 ,0x2 
 beqz x0,after_l2en

reri_high_int_en:
  #                     ce uu/de high high
 RERI_CFG_RAS_INIT 0x0 ,0x2 ,0x6 ,0x1 ,0x1 
 RERI_CFG_RAS_INIT 0x1 ,0x2 ,0x6 ,0x1 ,0x1 
 RERI_CFG_RAS_INIT 0x2 ,0x2 ,0x4 ,0x1 ,0x1 
 RERI_CFG_RAS_INIT 0x3 ,0x0 ,0x6 ,0x0 ,0x1 
 RERI_CFG_RAS_INIT 0x4 ,0x2 ,0x0 ,0x1 ,0x0 
 RERI_CFG_RAS_INIT 0x5 ,0x2 ,0x0 ,0x1 ,0x0 
 RERI_CFG_RAS_INIT 0x6 ,0x2 ,0x0 ,0x1 ,0x0 
 RERI_CFG_RAS_INIT 0x7 ,0x2 ,0x0 ,0x1 ,0x0 
 RERI_CFG_RAS_INIT 0x8 ,0x0 ,0x6 ,0x0 ,0x1 
 RERI_CFG_RAS_INIT 0x9 ,0x2 ,0x6 ,0x1 ,0x1 
 RERI_CFG_RAS_INIT 0xa ,0x2 ,0x4 ,0x1 ,0x1 
 beqz x0,after_l2en

reri_plat_int_en:
  #                     ce uu/de plat plat
 RERI_CFG_RAS_INIT 0x0 ,0x2 ,0x6 ,0x3 ,0x3 
 RERI_CFG_RAS_INIT 0x1 ,0x2 ,0x6 ,0x3 ,0x3 
 RERI_CFG_RAS_INIT 0x2 ,0x2 ,0x4 ,0x3 ,0x3 
 RERI_CFG_RAS_INIT 0x3 ,0x0 ,0x6 ,0x0 ,0x3 
 RERI_CFG_RAS_INIT 0x4 ,0x2 ,0x0 ,0x3 ,0x0 
 RERI_CFG_RAS_INIT 0x5 ,0x2 ,0x0 ,0x3 ,0x0 
 RERI_CFG_RAS_INIT 0x6 ,0x2 ,0x0 ,0x3 ,0x0 
 RERI_CFG_RAS_INIT 0x7 ,0x2 ,0x0 ,0x3 ,0x0 
 RERI_CFG_RAS_INIT 0x8 ,0x0 ,0x6 ,0x0 ,0x3 
 RERI_CFG_RAS_INIT 0x9 ,0x2 ,0x6 ,0x3 ,0x3 
 RERI_CFG_RAS_INIT 0xa ,0x2 ,0x4 ,0x3 ,0x3 
 beqz x0,after_l2en

reri_mix_int_en:
  #                      ce uu/de low high
 RERI_CFG_RAS_INIT 0x0 ,0x2 ,0x6 ,0x2 ,0x1 
 RERI_CFG_RAS_INIT 0x1 ,0x2 ,0x6 ,0x2 ,0x1 
 RERI_CFG_RAS_INIT 0x2 ,0x2 ,0x4 ,0x2 ,0x1 
 RERI_CFG_RAS_INIT 0x3 ,0x0 ,0x6 ,0x0 ,0x1 
 RERI_CFG_RAS_INIT 0x4 ,0x2 ,0x0 ,0x2 ,0x0 
 RERI_CFG_RAS_INIT 0x5 ,0x2 ,0x0 ,0x2 ,0x0 
 RERI_CFG_RAS_INIT 0x6 ,0x2 ,0x0 ,0x2 ,0x0 
 RERI_CFG_RAS_INIT 0x7 ,0x2 ,0x0 ,0x2 ,0x0 
 RERI_CFG_RAS_INIT 0x8 ,0x0 ,0x6 ,0x0 ,0x1 
 RERI_CFG_RAS_INIT 0x9 ,0x2 ,0x6 ,0x2 ,0x1 
 RERI_CFG_RAS_INIT 0xa ,0x2 ,0x4 ,0x2 ,0x1 
 beqz x0,after_l2en

reri_mix2_int_en:
  #                      ce uu/de high low
 RERI_CFG_RAS_INIT 0x0 ,0x2 ,0x6 ,0x1 ,0x2 
 RERI_CFG_RAS_INIT 0x1 ,0x2 ,0x6 ,0x1 ,0x2 
 RERI_CFG_RAS_INIT 0x2 ,0x2 ,0x4 ,0x1 ,0x2 
 RERI_CFG_RAS_INIT 0x3 ,0x0 ,0x6 ,0x0 ,0x2 
 RERI_CFG_RAS_INIT 0x4 ,0x2 ,0x0 ,0x1 ,0x0 
 RERI_CFG_RAS_INIT 0x5 ,0x2 ,0x0 ,0x1 ,0x0 
 RERI_CFG_RAS_INIT 0x6 ,0x2 ,0x0 ,0x1 ,0x0 
 RERI_CFG_RAS_INIT 0x7 ,0x2 ,0x0 ,0x1 ,0x0 
 RERI_CFG_RAS_INIT 0x8 ,0x0 ,0x6 ,0x0 ,0x2 
 RERI_CFG_RAS_INIT 0x9 ,0x2 ,0x6 ,0x1 ,0x2 
 RERI_CFG_RAS_INIT 0xa ,0x2 ,0x4 ,0x1 ,0x2 
  
  
  .global after_l2en 
after_l2en:
  

  # mccr2
  li x3, 0xe0000009
  #csrs mccr2,x3 
  .long 0x7c31a073

# PART 4: construct PMA and PMP

# cfig PMA
# init pma to default (pmacfg clr and set pmacfg2 to old sysmap)
# all pmacfg use TOR mode
init_pma:
  # init pmacfg0 clr
  csrw  pmaaddr0, x0
  csrw  pmaaddr1, x0
  csrw  pmaaddr2, x0
  csrw  pmaaddr3, x0
  csrw  pmaaddr4, x0
  csrw  pmaaddr5, x0
  csrw  pmaaddr6, x0
  csrw  pmaaddr7, x0
  csrw  pmacfg0, x0

  # init pmacfg2 to default (old sysmap)
  li    x1, 0   # x1 for pmacfg2
  # set pma8 TOR start_addr = 0
  li    x3, 0x20
  or    x1, x1, x3
  csrw  pmaaddr8, x0
  # 'h00_0000_0000 ~ 'h00_7fff_ffff mem
  li    x3, (0x20 << 8)
  or    x1, x1, x3
  li    x3, 0x80000000 >> 2
  csrw  pmaaddr9, x3
  # 'h00_8000_0000 ~ 'h00_9fff_ffff io
  li    x3, (0x22 << 16)
  or    x1, x1, x3
  li    x3, 0xa0000000 >> 2
  csrw  pmaaddr10, x3
  # 'h00_a000_0000 ~ 'h00_bfff_ffff nc
  li    x3, (0x21 << 24)
  or    x1, x1, x3
  li    x3, 0xc0000000 >> 2
  csrw  pmaaddr11, x3
  # 'h00_c000_0000 ~ 'h3f_ffff_ffff mem
  li    x3, (0x20 << 32)
  or    x1, x1, x3
  li    x3, 0x4000000000 >> 2
  csrw  pmaaddr12, x3
  # 'h40_0000_0000 ~ 'h4f_ffff_ffff nc
  li    x3, (0x21 << 40)
  or    x1, x1, x3
  li    x3, 0x5000000000 >> 2
  csrw  pmaaddr13, x3
  # 'h50_0000_0000 ~ 'h6f_ffff_ffff io
  li    x3, (0x22 << 48)
  or    x1, x1, x3
  li    x3, 0x7000000000 >> 2
  csrw  pmaaddr14, x3
  # 'h70_0000_0000 ~ 'hff_ffff_efff mem
  li    x3, (0x20 << 56)
  or    x1, x1, x3
  li    x3, 0xfffffff000 >> 2
  csrw  pmaaddr15, x3
  # set pmacfg2 and config effect
  csrw  pmacfg2, x1
# cfig PMP
# entry0: addr 0x0_0000_0000~0xff_ffff_ffff, r=1 w=1 x=1 l=0
init_pmp:
  # cfig pmpcfg0
  li   x1, 0x1f 
  csrw pmpcfg0, x1
  # cfig pmpaddr0
  li   x1, 0x1fffffffff
  csrw pmpaddr0, x1
  sfence.vma x0, x0


 #rand int
  #set meie
  li x3, 0x800
  csrs mie, x3
  #set IDNUM
  li x3, 0x99999000
  li x4, 0xffffffff
  sd x4, 0(x3)



  SETINT 11     RAND_ME_INT_BEG RAND_ME_INT_END
.global RAND_ME_INT_BEG
RAND_ME_INT_BEG:
  mret
.global RAND_ME_INT_END
RAND_ME_INT_END:
  nop
  nop


  jal	main

  .global __exit
__exit:
  fence
  addi x10,x0,0x0
  addi x1,x0,0x5a
  jal __finish_loop
  
  nop
  nop
  nop
  nop
  nop
  nop
  .global __fail
__fail:
  fence
  addi x10,x0,0x0
  addi x1,x0,0x2c
  jal __finish_loop
  .global __finish_loop
__finish_loop:
  j __finish_loop

  .global __mp_end
__mp_end:
  fence
  csrr x1, mhartid 
  andi x6, x1, 0x3  # get hart id
  srli x3, x1, 0x2  # get cluster id
  #li  x3, 0x0
  
  li  x4, 0x10
  mul x4, x4, x3
  li  x5, MP_DONE_ADDR
  add x5, x5, x4
  slli x6, x6, 0x2
  add x5, x5, x6
  sw  x5, 0(x5)
  fence

  slli      x3, x1, 3
  la        x1, power_down_cnt
  add       x1, x1, x3
  ld        x6, 0(x1)
power_deadloop:
  bnez      x6, power_deadloop
BRANCHWFI:
  wfi
  j BRANCHWFI


.align 2
.section .text
__trap_handler:
  j __synchronous_exception
  .align 2
  j __asychronous_int
  .align 2
  nop #reserved
  .align 2
  j __asychronous_int
  .align 2
  j __asychronous_int
  .align 2
  j __asychronous_int
  .align 2
  nop #reserved
  .align 2
  j __asychronous_int
  .align 2
  j __asychronous_int
  .align 2
  j __asychronous_int
  .align 2
  nop #reserved
  .align 2
  j __asychronous_int
  j __fail

__synchronous_exception:
  #push

  csrrw x2,mscratch,x2
  sd   x13,-8(sp)
  sd   x14,-16(sp)
  sd   x15,-24(sp)
  csrr x14,mcause
  andi x15,x14,0x7ff    #cause
  srli x14,x14,0x3b   #int
  andi x14,x14,0x10   #mask bit
  add  x14,x14,x15    #{int,cause}

  slli x14,x14,0x3 #offset
  la   x15,vector_table
  add  x15,x14,x15  #target pc
  ld   x14, 0(x15)  #get exception addr
  ld   x13, -8(sp)  #recover x13
  ld   x15, -24(sp) #recover x15
  addi x14,x14,-8
  jr   x14

__asychronous_int:
  sd   x13,-8(sp)
  sd   x14,-16(sp)
  sd   x15,-24(sp)
  csrr x14,mcause
  andi x15,x14,0x7ff    #cause
  srli x14,x14,0x3b   #int
  andi x14,x14,0x10   #mask bit
  add  x14,x14,x15    #{int,cause}

  slli x14,x14,0x3   #offset
  la   x15,vector_table
  add  x15,x14,x15   #target pc
  ld   x14, 0(x15)   #get exception addr
  ld   x13, -8(sp)   #recover x13
  ld   x15, -24(sp)  #recover x15
  addi x14,x14,-8
  jr   x14

.global vector_table
 .align  10
  vector_table:   #totally 256 entries
  .rept   128
  .long   __fail
  .long  0
  .endr

  .global __dummy
__dummy:  
  
  .data
  nop
  nop
  nop

.align 2
.section .text
__trap_handler_s:
  j __synchronous_exception_s
  .align 2
  j __asychronous_int
  .align 2
  nop #reserved
  .align 2
  j __asychronous_int
  .align 2
  j __asychronous_int
  .align 2
  j __asychronous_int
  .align 2
  nop #reserved
  .align 2
  j __asychronous_int
  .align 2
  j __asychronous_int
  .align 2
  j __asychronous_int
  .align 2
  nop #reserved
  .align 2
  j __asychronous_int
  j __fail

__synchronous_exception_s:
  #push
  csrrw x2,sscratch,x2
  sd   x13,-8(sp)
  sd   x14,-16(sp)
  sd   x15,-24(sp)
  csrr x14,scause
  andi x15,x14,0x7ff    #cause
  srli x14,x14,0x3b   #int
  andi x14,x14,0x10   #mask bit
  add  x14,x14,x15    #{int,cause}

  slli x14,x14,0x3 #offset
  la   x15,vector_table_s
  add  x15,x14,x15  #target pc
  ld   x14, 0(x15)  #get exception addr
  ld   x13, -8(sp)  #recover x13
  ld   x15, -24(sp) #recover x15
  addi x14,x14,-4
  #csrrw x2,sscratch,x2
  jr   x14

.global vector_table_s
 .align  10
  vector_table_s:   #totally 256 entries
  .rept   128
  .long   __fail
  .long   0
  .endr


.align 2
.section .text
__trap_handler_vs:
  j __synchronous_exception_vs
  .align 2
  j __asychronous_int
  .align 2
  nop #reserved
  .align 2
  j __asychronous_int
  .align 2
  j __asychronous_int
  .align 2
  j __asychronous_int
  .align 2
  nop #reserved
  .align 2
  j __asychronous_int
  .align 2
  j __asychronous_int
  .align 2
  j __asychronous_int
  .align 2
  nop #reserved
  .align 2
  j __asychronous_int
  j __fail

__synchronous_exception_vs:
  #push
  csrrw x2,sscratch,x2
  sd   x13,-8(sp)
  sd   x14,-16(sp)
  sd   x15,-24(sp)
  csrr x14,scause
  andi x15,x14,0x7ff    #cause
  srli x14,x14,0x3b   #int
  andi x14,x14,0x10   #mask bit
  add  x14,x14,x15    #{int,cause}

  slli x14,x14,0x3 #offset
  la   x15,vector_table_vs
  add  x15,x14,x15  #target pc
  ld   x14, 0(x15)  #get exception addr
  ld   x13, -8(sp)  #recover x13
  ld   x15, -24(sp) #recover x15
  addi x14,x14,-8
  jr   x14

.global vector_table_vs
 .align  10
  vector_table_vs:   #totally 256 entries
  .rept   128
  .long   __fail
  .long   0
  .endr



.align 4
.global power_up_resume_pc_stack
.global power_down_cnt

power_up_resume_pc_stack:
  .rept  32
  .long  0
  .endr
power_down_cnt:
   .dword 0x0000000000000000
   .dword 0x0000000000000000
   .dword 0x0000000000000000
   .dword 0x0000000000000000
   .dword 0x0000000000000000
   .dword 0x0000000000000000
   .dword 0x0000000000000000
   .dword 0x0000000000000000


.section .tohost, "aw", @progbits
.global  tohost
.align 8
tohost: .dword 0
.global  fromhost
.align 8
fromhost: .dword 0
