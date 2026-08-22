.set sval0,  0xbf800000 #-1
.set sval1,  0x00800000 #min positive float normal
.set sval2,  0x37800000 #2^-16
.set sval3,  0x38000000 #2^-15
.set sval4,  0x3b800000 #2^-8
.set sval5,  0x3c000000 #2^-7
.set sval6,  0x3d800000 #2^-4
.set sval7,  0x3e000000 #2^-3
.set sval8,  0x3e800000 #2^-2
.set sval9,  0x3ea00000 #0.3125
.set sval10, 0x3ec00000 #0.375
.set sval11, 0x3ee00000 #0.4375
.set sval12, 0x3f000000 #0.5
.set sval13, 0x3f200000 #0.625
.set sval14, 0x3f400000 #0.75
.set sval15, 0x3f600000 #0.875
.set sval16, 0x3f800000 #1.0
.set sval17, 0x3fa00000 #1.25
.set sval18, 0x3fc00000 #1.5
.set sval19, 0x3fe00000 #1.75
.set sval20, 0x40000000 #2.0
.set sval21, 0x40200000 #2.5
.set sval22, 0x40400000 #3
.set sval23, 0x40800000 #4
.set sval24, 0x41000000 #8
.set sval25, 0x41800000 #16
.set sval26, 0x43000000 #128
.set sval27, 0x43800000 #256
.set sval28, 0x47000000 #2^15
.set sval29, 0x47800000 #2^16

.set dval0,  0xbff0000000000000
.set dval1,  0x0010000000000000 #min positive double normal
.set dval2,  0x3ef0000000000000
.set dval3,  0x3f00000000000000
.set dval4,  0x3f70000000000000
.set dval5,  0x3f80000000000000
.set dval6,  0x3fb0000000000000
.set dval7,  0x3fc0000000000000
.set dval8,  0x3fd0000000000000
.set dval9,  0x3fd4000000000000
.set dval10, 0x3fd8000000000000
.set dval11, 0x3fdc000000000000
.set dval12, 0x3fe0000000000000
.set dval13, 0x3fe4000000000000
.set dval14, 0x3fe8000000000000
.set dval15, 0x3fec000000000000
.set dval16, 0x3ff0000000000000
.set dval17, 0x3ff4000000000000
.set dval18, 0x3ff8000000000000
.set dval19, 0x3ffc000000000000
.set dval20, 0x4000000000000000
.set dval21, 0x4004000000000000
.set dval22, 0x4008000000000000
.set dval23, 0x4010000000000000
.set dval24, 0x4020000000000000
.set dval25, 0x4030000000000000
.set dval26, 0x4060000000000000
.set dval27, 0x4070000000000000
.set dval28, 0x40e0000000000000
.set dval29, 0x40f0000000000000

.set hval0,  0xbc00 #-1
.set hval1,  0x0400 #min positive half normal
.set hval2,  0x0100 #2^-16
.set hval3,  0x0200 #2^-15
.set hval4,  0x1c00 #2^-8
.set hval5,  0x2000 #2^-7
.set hval6,  0x2c00 #2^-4
.set hval7,  0x3000 #2^-3
.set hval8,  0x3400 #2^-2
.set hval9,  0x3500 #0.3125
.set hval10, 0x3600 #0.375
.set hval11, 0x3700 #0.4375
.set hval12, 0x3800 #0.5
.set hval13, 0x3900 #0.625
.set hval14, 0x3a00 #0.75
.set hval15, 0x3b00 #0.875
.set hval16, 0x3c00 #1.0
.set hval17, 0x3d00 #1.25
.set hval18, 0x3e00 #1.5
.set hval19, 0x3f00 #1.75
.set hval20, 0x4000 #2.0
.set hval21, 0x4100 #2.5
.set hval22, 0x4200 #3
.set hval23, 0x4400 #4
.set hval24, 0x4800 #8
.set hval25, 0x4c00 #16
.set hval26, 0x5800 #128
.set hval27, 0x5c00 #256
.set hval28, 0x7800 #2^15
#positive inifity
.set sp_inf, 0x7f800000 
.set dp_inf, 0x7ff0000000000000
.set hp_inf, 0x7c00 
#negetive inifity
.set sn_inf, 0xff800000 
.set dn_inf, 0xfff0000000000000
.set hn_inf, 0xfc00
#canonical NaN (qNaN include cNaN)
.set sp_cNaN, 0x7fc00000 #sval31
.set dp_cNaN, 0x7ff8000000000000 #dval31
.set hp_cNaN, 0x7e00 #hval31
#postive qNaN (exclude quiet bit,other E bit non_zero)
.set sp_qNaN, 0x7fc00908
.set dp_qNaN, 0x7ff800000000000f
.set hp_qNaN, 0x7efc
#postive sNaN
.set sp_sNaN, 0x7f800908 #0111_1111_1000_0000_0000_1001_0000_1000
.set dp_sNaN, 0x7ff000000000000f 
.set hp_sNaN, 0x7cfc
#negetive qNaN
.set sn_qNaN, 0xffc00000 
.set dn_qNaN, 0xfff8000000000000 
.set hn_qNaN, 0xfe00 
#negetive sNaN
.set sn_sNaN, 0xff800908 #0111_1111_1000_0000_0000_1001_0000_1000
.set dn_sNaN, 0xfff000000000000f 
.set hn_sNaN, 0xfcfc
#negetive zero
.set sn_zero, 0x80000000
.set dn_zero, 0x8000000000000000
.set hn_zero, 0x8000

.macro ZFA_CLRFFLAG   
  li x1, 0x1f
  csrrc x0, fflags, x1
.endm

.macro ZFA_FFLAG_CHECK FFLAG_VAL  
  li x1, \FFLAG_VAL
  csrrc x31, fflags, x0
  bne x1, x31, TEST_FAIL
.endm

.macro ZFA_FIMM_FREG_S FRD, F_IMM
  li x31, \F_IMM
  fmv.w.x \FRD, x31
.endm

.macro ZFA_FEQCHECK_S FRS1, FSVAL
  ZFA_FFLAG_CHECK 0x0
  ZFA_FIMM_FREG_S f31, \FSVAL
  feq.s x1, \FRS1, f31
  li x31, 0x1
  bne x1, x31, TEST_FAIL
.endm

.macro ZFA_FIMM_FREG_D FRD, F_IMM
  li x31, \F_IMM
  fmv.d.x \FRD, x31
.endm

.macro ZFA_FEQCHECK_D FRS1, FDVAL
  ZFA_FFLAG_CHECK 0x0
  ZFA_FIMM_FREG_D f31, \FDVAL
  feq.d x1, \FRS1, f31
  li x31, 0x1
  bne x1, x31, TEST_FAIL
.endm

.macro ZFA_FIMM_FREG_H FRD, F_IMM
  li x31, \F_IMM
  fmv.h.x \FRD, x31
.endm

.macro ZFA_FEQCHECK_H FRS1, FHVAL
  ZFA_FFLAG_CHECK 0x0
  ZFA_FIMM_FREG_H f31, \FHVAL
  feq.h x1, \FRS1, f31
  li x31, 0x1
  bne x1, x31, TEST_FAIL
.endm

.macro ZFA_MIN_CHECK_S VAL0, VAL1, RES_VAL
  ZFA_FIMM_FREG_S f2, \VAL0
  ZFA_FIMM_FREG_S f3, \VAL1
  li x5, \RES_VAL
  fminm.s f1, f2, f3
  fmv.x.w x4,f1 
  li x6,0x00000000ffffffff
  and x4, x4, x6 
  bne x4, x5, TEST_FAIL
.endm

.macro ZFA_MAX_CHECK_S VAL0, VAL1, RES_VAL
  ZFA_FIMM_FREG_S f2, \VAL0
  ZFA_FIMM_FREG_S f3, \VAL1
  li x5, \RES_VAL
  fmaxm.s f1, f2, f3
  fmv.x.w x4,f1 
  li x6,0x00000000ffffffff
  and x4, x4, x6 
  bne x4, x5, TEST_FAIL
.endm

.macro ZFA_MIN_CHECK_D VAL0, VAL1, RES_VAL
  ZFA_FIMM_FREG_D f2, \VAL0
  ZFA_FIMM_FREG_D f3, \VAL1
  li x5, \RES_VAL
  fminm.d f0,f2,f3
  fmv.x.d x4, f0
  bne x4, x5, TEST_FAIL
.endm

.macro ZFA_MAX_CHECK_D VAL0, VAL1, RES_VAL
  ZFA_FIMM_FREG_D f2, \VAL0
  ZFA_FIMM_FREG_D f3, \VAL1
  li x5, \RES_VAL
  fmaxm.d f0,f2,f3
  fmv.x.d x4, f0
  bne x4, x5, TEST_FAIL
.endm

.macro ZFA_MIN_CHECK_H VAL0, VAL1, RES_VAL
  ZFA_FIMM_FREG_H f2, \VAL0
  ZFA_FIMM_FREG_H f3, \VAL1
  li x5, \RES_VAL
  fminm.h f0,f2,f3
  fmv.x.h x4, f0
  li x6,0x000000000000ffff
  and x4, x4, x6 
  bne x4, x5, TEST_FAIL
.endm

.macro ZFA_MAX_CHECK_H VAL0, VAL1, RES_VAL
  ZFA_FIMM_FREG_H f2, \VAL0
  ZFA_FIMM_FREG_H f3, \VAL1
  li x5, \RES_VAL
  fmaxm.h f0,f2,f3
  fmv.x.h x4, f0
  li x6,0x000000000000ffff
  and x4, x4, x6 
  bne x4, x5, TEST_FAIL
.endm

.macro ZFA_FROUND_S  FRD, FRS1, FRS1_VAL, RES_VAL, RMODE  #RM(RNE RTZ RDN RUP RMM)
  ZFA_FIMM_FREG_S \FRS1, \FRS1_VAL
  li x5, \RES_VAL
  fround.s \FRD, \FRS1, \RMODE
  fmv.x.w x4, \FRD
  li x6, 0x00000000ffffffff
  and x4, x4, x6
  bne x4, x5, TEST_FAIL
.endm
.macro ZFA_FROUNDNX_S  FRD, FRS1, FRS1_VAL, RES_VAL, RMODE  #RM(RNE RTZ RDN RUP RMM)
  ZFA_FIMM_FREG_S \FRS1, \FRS1_VAL
  li x5, \RES_VAL
  froundnx.s \FRD, \FRS1, \RMODE
  fmv.x.w x4, \FRD
  li x6, 0x00000000ffffffff
  and x4, x4, x6
  bne x4, x5, TEST_FAIL
.endm

.macro ZFA_FROUND_D  FRD, FRS1, FRS1_VAL, RES_VAL, RMODE  #RM(RNE RTZ RDN RUP RMM)
  ZFA_FIMM_FREG_D \FRS1, \FRS1_VAL
  li x5, \RES_VAL
  fround.d \FRD, \FRS1, \RMODE
  fmv.x.d x4, \FRD
  bne x4, x5, TEST_FAIL
.endm

.macro ZFA_FROUNDNX_D  FRD, FRS1, FRS1_VAL, RES_VAL, RMODE  #RM(RNE RTZ RDN RUP RMM)
  ZFA_FIMM_FREG_D \FRS1, \FRS1_VAL
  li x5, \RES_VAL
  froundnx.d \FRD, \FRS1, \RMODE
  fmv.x.d x4, \FRD
  bne x4, x5, TEST_FAIL
.endm

.macro ZFA_FROUND_H  FRD, FRS1, FRS1_VAL, RES_VAL, RMODE  #RM(RNE RTZ RDN RUP RMM)
  ZFA_FIMM_FREG_H \FRS1, \FRS1_VAL
  li x5, \RES_VAL
  fround.h \FRD, \FRS1, \RMODE
  fmv.x.h x4, \FRD
  li x6, 0x000000000000ffff
  and x4, x4, x6
  bne x4, x5, TEST_FAIL
.endm
.macro ZFA_FROUNDNX_H  FRD, FRS1, FRS1_VAL, RES_VAL, RMODE  #RM(RNE RTZ RDN RUP RMM)
  ZFA_FIMM_FREG_H \FRS1, \FRS1_VAL
  li x5, \RES_VAL
  froundnx.h \FRD, \FRS1, \RMODE
  fmv.x.h x4, \FRD
  li x6, 0x000000000000ffff
  and x4, x4, x6
  bne x4, x5, TEST_FAIL
.endm

.macro ZFA_FLEQCHECK_S FRS1_VAL, FRS2_VAL, RES_VAL
  ZFA_FIMM_FREG_S f20, \FRS1_VAL
  ZFA_FIMM_FREG_S f23, \FRS2_VAL 
  fleq.s x15, f20, f23
  li x8, \RES_VAL
  bne x15, x8, TEST_FAIL
.endm
.macro ZFA_FLEQCHECK_D FRS1_VAL, FRS2_VAL, RES_VAL
  ZFA_FIMM_FREG_D f20, \FRS1_VAL
  ZFA_FIMM_FREG_D f23, \FRS2_VAL 
  fleq.d x15, f20, f23
  li x8, \RES_VAL
  bne x15, x8, TEST_FAIL
.endm
.macro ZFA_FLEQCHECK_H FRS1_VAL, FRS2_VAL, RES_VAL
  ZFA_FIMM_FREG_H f20, \FRS1_VAL
  ZFA_FIMM_FREG_H f23, \FRS2_VAL 
  fleq.h x15, f20, f23
  li x8, \RES_VAL
  bne x15, x8, TEST_FAIL
.endm
.macro ZFA_FLTQCHECK_S FRS1_VAL, FRS2_VAL, RES_VAL
  ZFA_FIMM_FREG_S f20, \FRS1_VAL
  ZFA_FIMM_FREG_S f23, \FRS2_VAL 
  fltq.s x15, f20, f23
  li x8, \RES_VAL
  bne x15, x8, TEST_FAIL
.endm
.macro ZFA_FLTQCHECK_D FRS1_VAL, FRS2_VAL, RES_VAL
  ZFA_FIMM_FREG_D f20, \FRS1_VAL
  ZFA_FIMM_FREG_D f23, \FRS2_VAL 
  fltq.d x15, f20, f23
  li x8, \RES_VAL
  bne x15, x8, TEST_FAIL
.endm
.macro ZFA_FLTQCHECK_H FRS1_VAL, FRS2_VAL, RES_VAL
  ZFA_FIMM_FREG_H f20, \FRS1_VAL
  ZFA_FIMM_FREG_H f23, \FRS2_VAL 
  fltq.h x15, f20, f23
  li x8, \RES_VAL
  bne x15, x8, TEST_FAIL
.endm
.macro ZFA_FCVTMOD_CHECK XRD, CVT_VAL, RES_VAL
  ZFA_FIMM_FREG_D f30, \CVT_VAL
  fcvtmod.w.d \XRD,f30,rtz 
  li x31, \RES_VAL
  bne x31, \XRD, TEST_FAIL
.endm

  .set bfp_cNaN, 0x7fc0 
  .set bfp_qNaN, 0x7fc1 # 0 1111_1111 100_0001  
  .set bfn_qNaN, 0xffc1 # 1 1111_1111 100_0001
  .set bfp_sNaN, 0x7f81 # 0 1111_1111 000_0001
  .set bfn_sNaN, 0xff81 # 1 1111_1111 000_0001
  .set bfp_inf,  0x7f80 # 0 1111_1111 000_0000
  .set bfn_inf,  0xff80 # 1 1111_1111 000_0000
  .set bfn_zero, 0x8000 

  .set bfval0,  0xbf80  #-1  # 1 0111_1111 000_0000
  .set bfval1,  0x0080  #min p nor 0 0000_0001 000_0000
  .set bfval2,  0x3780  #0 0110_1111 000_0000
  .set bfval3,  0x3800  #0 0111_0000 000_0000
  .set bfval4,  0x3b80  
  .set bfval5,  0x3c00
  .set bfval6,  0x3d80
  .set bfval7,  0x3e00
  .set bfval8,  0x3e80
  .set bfval9,  0x3ea0
  .set bfval10, 0x3ec0
  .set bfval11, 0x3ee0
  .set bfval12, 0x3f00
  .set bfval13, 0x3f20
  .set bfval14, 0x3f40
  .set bfval15, 0x3f60
  .set bfval16, 0x3f80
  .set bfval17, 0x3fa0
  .set bfval18, 0x3fc0
  .set bfval19, 0x3fe0
  .set bfval20, 0x4000
  .set bfval21, 0x4020
  .set bfval22, 0x4040
  .set bfval23, 0x4080
  .set bfval24, 0x4100
  .set bfval25, 0x4180
  .set bfval26, 0x4300
  .set bfval27, 0x4380
  .set bfval28, 0x4700
  .set bfval29, 0x4780

  .macro ZFA_FEQCHECK_BF FREG, BFVAL
     fmv.x.h x4, \FREG
     li x5, \BFVAL
     li x6, 0xffff
     and x4, x4, x6
     bne x4, x5, TEST_FAIL
  .endm
