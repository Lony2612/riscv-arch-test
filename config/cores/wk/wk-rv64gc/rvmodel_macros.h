#ifndef _RVMODEL_MACROS_H
#define _RVMODEL_MACROS_H

#define RVMODEL_DATA_SECTION \
        .pushsection .tohost,"aw",@progbits;                \
        .balign 8; .global tohost; tohost: .dword 0;         \
        .balign 8; .global fromhost; fromhost: .dword 0;     \
        .popsection;

#define STANDARD_SM_SUPPORTED


.set MP_DONE_ADDR,  0x9f0f2000
.set TOTAL_CORE_NUM, 1
.set RERI_INT_SEL, 0

// ##### STARTUP // #####

# Perform boot operations. Can be empty or left undefined unless needed for
# DUT-specific behavior such as turning on a memory controller or
# initializing custom state.

#define RVMODEL_BOOT \
  LA(ra, wk_boot); \
  jalr ra; \
  .global mode_switch; \
  mode_switch:

#ifndef RVTEST_SELFCHECK
  #define WKMODEL_BOOT
#else
  #define WKMODEL_BOOT \
    /* For MP test, all cores do initialization */ \
    /* TOTAL_CORE_NUM is passed from compiler options  */ \
    li x3, TOTAL_CORE_NUM; \
    li x4, 0x1; \
    blt x4, x3, core_init_begin; \
    \
    /* For UP test, only core0 do initialization */ \
    csrr x3,mhartid; \
    beqz x3, core_init_begin; \
    wfi; \
    \
    core_init_begin: \
    /* enable extension */ \
    li  x3, 0x400000; \
    csrs 0x7c0, x3; \
    \
    /* enable unalign */ \
    li x3, 0x8000; \
    csrs 0x7c0,x3; \
    \
    /* mxstatus */ \
    li  x3, 0x638000; \
    csrs 0x7c0, x3; \
    \
    /* msmpr */ \
    csrs 0x7f3, 0x1; \
    \
    /* PART 2: initialize stack pointer & ppn */ \
    csrr x3,  mhartid; \
    li x4,  1; \
    \
    .global cpu_0_sp; \
    cpu_0_sp: \
    \
    /* invalid all memory for IBP, BTB, BHT, DCACHE and ICACHE */ \
    li x3, 0x70013; \
    csrs 0x7c2, x3; /* mcor */ \
    \
    /* #  enable ICACHE,DCACHE,BHT, BTB, IBP, RAS and WA */ \
    li x3, 0x11ff; \
    csrs 0x7c1, x3; /*  mhcr */ \
    \
    .global after_l2en; \
    after_l2en: \
    li  x3, 0x43e14c30c; \
    csrs  0x7c5, x3; /* #mhint */ \
    \
    li x3, 0xe0001419; \
    csrs  0x7c3, x3;   // mccr2
#endif

#ifndef RVTEST_SELFCHECK
  #define RVTEST_POST_MMODE_HOOK   nop
#else
  #define RVTEST_POST_MMODE_HOOK   j rvmodel_halt_pass
#endif

.section .text.wk_boot, "ax", @progbits
.global wk_boot
wk_boot:
    WKMODEL_BOOT
    LA(t0, mode_switch)
    jr t0

.set MB_INTPCBASE   ,0x9ff01000
.set MB_INTTYPEBASE ,0x9ff01008
.set MB_RANDENBASE  ,0x9ff01010
.set MB_OFFSET      ,0x20

.macro MB_ASYN_SET  CORENUM,PC,TYPE,EN
   addi x2,x2, -56
   sd x9,  0(x2) 
   sd x10, 8(x2)
   sd x11, 16(x2)
   sd x12, 24(x2)
   sd x13, 32(x2)

   #cal each core start addr offset
   li   x9,\CORENUM
   li   x10,MB_OFFSET
   li   x11,\CORENUM
  //  mul  x11,x9,x10

   #cal int_pc addr
   li   x12,MB_INTPCBASE
   add  x12,x11,x12
   la   x13,\PC
   sd   x13,0x0(x12)

   #cal int_type addr
   li   x12,MB_INTTYPEBASE
   add  x12,x11,x12
   li   x13,\TYPE
   sd   x13,0x0(x12)

   #cal rand_en
   li   x12,MB_RANDENBASE
   add  x12,x11,x12
   li   x13,\EN
   sd   x13,0x0(x12)

   ld x9,  0(x2)
   ld x10, 8(x2)
   ld x11, 16(x2)
   ld x12, 24(x2)
   ld x13, 32(x2)
   addi x2,x2,56

   fence
.endm

// Custom RVMODEL_BOOT_TO_MMODE overrides default RVTEST_BOOT_TO_MMODE
// if defined.  For most DUTs, the default should work and this macro
// should not be defined.  If no M-mode or CSRs are implemented, define this
// macro as blank to bypass the boot process.  If a nonconforming
// M-mode is implemented, define this macro to set up the necessary
// state in a fashion similar to RVTEST_BOOT_TO_MMODE.
//#define RVMODEL_BOOT_TO_MMODE

// // ##### TERMINATION // #####

// # Terminate test with a pass indication.
// # When the test is run in simulation, this should end the simulation.
#define RVMODEL_HALT_PASS  \
  li x1, 1                ;\
  la t0, tohost           ;\
  write_tohost_pass:      ;\
    sw x1, 0(t0)          ;\
    sw x0, 4(t0)          ;\
  self_loop_pass:         ;\
  __exit:                 ;\
    j self_loop_pass      ;\

# Terminate test with a fail indication.
# When the test is run in simulation, this should end the simulation.
#define RVMODEL_HALT_FAIL \
  li x1, 3                ;\
  la t0, tohost           ;\
  write_tohost_fail:      ;\
    sw x1, 0(t0)          ;\
    sw x0, 4(t0)          ;\
  self_loop_fail:         ;\
  __fail:                 ;\
    j self_loop_fail      ;\

// ##### IO // #####
.EQU TB_PRINT_ADDR, 0x9ff00000

# Initialization steps needed prior to writing to the console
# _R1, _R2, and _R3 can be used as temporary registers if needed.
# Do not modify any other registers (or make sure to restore them).
# Can be empty or left undefined if no initialization is needed.
// #define RVMODEL_IO_INIT(_R1, _R2, _R3)
  // uart_init:                ;\
  //   li _R1, UART_LCR         ; /* Load address of UART LCR */    \
  //   li _R2, 3                ; /* 8-bit characters, 1 stop bit, no parity */ \
  //   sb _R2, 0(_R1)           ; \

// # Prints a null-terminated string using a DUT specific mechanism.
// # A pointer to the string is passed in _STR_PTR.
// # _R1, _R2, and _R3 can be used as temporary registers if needed.
// # Do not modify any other registers (or make sure to restore them).
#define RVMODEL_IO_WRITE_STR(_R1, _R2, _R3, _STR_PTR) \
1:                                                   ;    \
  lbu _R1, 0(_STR_PTR)       ;   /* Load byte */          \
  beqz _R1, 2f               ;   /* Exit if null */       \
  li _R2, TB_PRINT_ADDR                              ;    \
  sb _R1, 0(_R2)             ;   /* transmit character */ \
  addi _STR_PTR, _STR_PTR, 1 ;   /* Next char */          \
  j 1b                       ;   /* Loop */               \
2:

// ##### Access Fault // #####

// #define RVMODEL_ACCESS_FAULT_ADDRESS 0x02000000

// ##### Interrupt Latency // #####

#define RVMODEL_INTERRUPT_LATENCY 60

// ##### Machine Timer // #####

// Wally's mtime advances one tick per core clock, and the code between arming
// mtimecmp and reaching the lower privilege mode (three CLINT stores plus
// RVTEST_GOTO_LOWER_MODE) can take well over 100 cycles on a pipelined core
// with caches.  With a delay of 100 the timer interrupt fires while still in
// M-mode with MIE=1, so the trap records MPP=M instead of MPP=U/S.
#define RVMODEL_TIMER_INT_SOON_DELAY 200

#define RVMODEL_ACLINT_BASE_ADDR    0x9fef0000

#define RVMODEL_MTIME_ADDRESS       (RVMODEL_ACLINT_BASE_ADDR + 0x0000BFF8)  /* Address of mtime CSR */
#define RVMODEL_MTIMECMP_ADDRESS    (RVMODEL_ACLINT_BASE_ADDR + 0x00004000) /* Address of mtimecmp CSR */

// ##### Machine Interrupts // #####

#define CLINT_BASE_ADDRESS          0x9fef0000
#define RVMODEL_MSIP_ADDRESS        (CLINT_BASE_ADDRESS + 0x0)


#define PLIC_BASE_ADDRESS    0x9fee8000
#define PLIC_ENABLE_ADDRESS  (PLIC_BASE_ADDRESS + 0x00002000)
#define PLIC_THRESH_ADDRESS  (PLIC_BASE_ADDRESS + 0x00200000)
#define PLIC_CLAIM_ADDRESS   (PLIC_BASE_ADDRESS + 0x00200004)
#define PLIC_SENABLE_ADDRESS (PLIC_BASE_ADDRESS + 0x00002080)   /* For S mode */
#define PLIC_STHRESH_ADDRESS (PLIC_BASE_ADDRESS + 0x00201000)
#define PLIC_SCLAIM_ADDRESS  (PLIC_BASE_ADDRESS + 0x00201004)

#define NS16550_BASE_ADDRESS 0x10000000
#define UART_INT_SRC         10

#define RVMODEL_SET_MEXT_INT(_R1, _R2)          \
  MB_ASYN_SET 0, 5, 0x1, 0x3

#define RVMODEL_CLR_MEXT_INT(_R1, _R2)          \
  MB_ASYN_SET 0, 5, 0x00, 0x1

#define RVMODEL_SET_MSW_INT(_R1, _R2) \
  li _R1, 1; \
  li _R2, RVMODEL_MSIP_ADDRESS; \
  sw _R1, 0(_R2); \
  fence rw, rw; \
  lw _R1, 0(_R2);

#define RVMODEL_CLR_MSW_INT(_R1, _R2) \
  li _R2, RVMODEL_MSIP_ADDRESS; \
  sw zero, 0(_R2); \
  fence rw, rw; \
  lw _R1, 0(_R2); 

// ##### Supervisor Interrupts // #####

#define CVW_SSIP_ADDRESS (CLINT_BASE_ADDRESS + 0xC000)

.set ACLINT_BASE_ADDR, 0x9fef0000
.set ssip0, 0xc000
.set ssip1, 0xc004
.set ssip2, 0xc008
.set ssip3, 0xc00c

#define RVMODEL_SET_SEXT_INT(_R1, _R2)          \
  MB_ASYN_SET 0, 5, 0x8, 0x3

#define RVMODEL_CLR_SEXT_INT(_R1, _R2)          \
  MB_ASYN_SET 0, 5, 0x00, 0x1

#define RVMODEL_SET_SSW_INT(_R1, _R2) \
  li _R1, 1; \
  li _R2, CVW_SSIP_ADDRESS; \
  sw _R1, 0(_R2);

#define RVMODEL_CLR_SSW_INT(_R1, _R2) \
  li _R2, CVW_SSIP_ADDRESS; \
  sw zero, 0(_R2);

#endif // _RVMODEL_MACROS_H
