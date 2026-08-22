.set HpsNaN,    0x7c01
.set HnsNaN,    0xfc01
.set HpqNaN,    0x7e00
.set HnqNaN,    0xfe00
.set HpInf,     0x7c00
.set HnInf,     0xfc00
.set HpNorm,    0x3800    #//0.5
.set HnNorm,    0xb800    #//-0.5
.set HpDeNorm,  0x0001
.set HnDeNorm,  0x8001
.set HpZero,    0x0000
.set HnZero,    0x8000
.set HpLFN,     0x7bff
.set HnLFN,     0xfbff
.set HpMIN,     0x0400
.set HnMIN,     0x8400
.set HpSPECIAL_1,  0x0002    # // denorm+denrom
.set HnSPECIAL_1,  0x8002    # // -denorm + -denorm
.set HpOne,  0x3c00
.set HnOne,  0xbc00
.set HpONE,  0x3c00
.set HnONE,  0xbc00
.set Hp1p1,  0x3e00
.set Hn1p1,  0xbe00
.set HpDeNormMost, 0x03ff    
.set HnDeNormMost, 0x83ff


#//==========================================================
#// single format
#//==========================================================
.set SpsNaN,   0x7f800001 #//0_111'1111'1_000'0000'0000'0000'0000'0001
.set SnsNaN,   0xff800001 #//1_111'1111'1_000'0000'0000'0000'0000'0001
.set SpqNaN,   0x7fc00000 #//0_111'1111'1_100'0000'0000'0000'0000'0000
.set SnqNaN,   0xffc00000 #//1_111'1111'1_100'0000'0000'0000'0000'0000
.set SpInf,    0x7f800000 #//0_111'1111'1_000'0000'0000'0000'0000'0000
.set SnInf,    0xff800000 #//1_111'1111'1_000'0000'0000'0000'0000'0000
.set SpNorm,   0x3f000000 #//0_011'1111'0_000'0000'0000'0000'0000'0000 (+0.5)
.set SnNorm,   0xbf000000 #//1_011'1111'0_000'0000'0000'0000'0000'0000 (-0.5)
.set SpDeNorm, 0x00000001 #//0_000'0000'0_000'0000'0000'0000'0000'0001
.set SnDeNorm, 0x80000001 #//1_000'0000'0_000'0000'0000'0000'0000'0001
.set SpZero,   0x00000000 #//0_000'0000'0_000'0000'0000'0000'0000'0000
.set SnZero,   0x80000000 #//1_000'0000'0_000'0000'0000'0000'0000'0000
.set SpLFN,    0x7f7fffff #//0_111'1111'0_111'1111'1111'1111'1111'1111
.set SnLFN,    0xff7fffff #//1_111'1111'0_111'1111'1111'1111'1111'1111
.set SpMIN,    0x00800000 #//0_000'0000'1_000'0000'0000'0000'0000'0000 
.set SnMIN,    0x80800000 #//1_000'0000'1_000'0000'0000'0000'0000'0000 

.set SpSPECIAL_1,  0x00000002
.set SnSPECIAL_1,  0x80000002
.set SpOne,  0x3f800000
.set SnOne,  0xbf800000
.set SpONE,  0x3f800000
.set SnONE,  0xbf800000
.set Sp1p1,  0x3fc00000
.set Sn1p1,  0xbfc00000

.set SpDeNormMost,   0x007fffff    #//0_000'0000'0111'_ffff...ffff
.set SnDeNormMost,   0x807fffff    #//1_000'0000'0111'_ffff...ffff


#//==========================================================
#// double format
#//==========================================================
.set DpsNaN,    0x7ff0000000000001 #//0_111'1111'1111'_0000'0...0'0001
.set DpsNaNH,   0x7ff00000
.set DpsNaNL,   0x00000001
.set DnsNaN,    0xfff0000000000001 #//1_111'1111'1111'_0000'0...0'0001
.set DnsNaNH,   0xfff00000
.set DnsNaNL,   0x00000001
.set DpqNaN,    0x7ff8000000000000 #//0_111'1111'1111'_1000'0...0'0000
.set DpqNaNH,   0x7ff80000
.set DpqNaNL,   0x00000000
.set DnqNaN,    0xfff8000000000000 #//1_111'1111'1111'_1000'0...0'0000
.set DnqNaNH,   0xfff80000
.set DnqNaNL,   0x00000000
.set DpInf,     0x7ff0000000000000 #//0_111'1111'1111'_0000'0...0'0000
.set DpInfH,    0x7ff00000
.set DpInfL,    0x00000000
.set DnInf,     0xfff0000000000000 #//1_111'1111'1111'_0000'0...0'0000
.set DnInfH,    0xfff00000
.set DnInfL,    0x00000000
.set DpNorm,    0x3fe0000000000000 #//0_011'1111'1110'_0000'0...0'0000 (+0.5)
.set DpNormH,   0x3fe00000
.set DpNormL,   0x00000000
.set DnNorm,    0xbfe0000000000000 #//1_011'1111'1110'_0000'0...0'0000 (-0.5)
.set DnNormH,   0xbfe00000
.set DnNormL,   0x00000000
.set DpDeNorm,  0x0000000000000001 #//0_000'0000'0000'_0000'0...0'0001
.set DpDeNormH, 0x00000000
.set DpDeNormL, 0x00000001
.set DnDeNorm,  0x8000000000000001 #//1_000'0000'0000'_0000'0...0'0001
.set DnDeNormH, 0x80000000
.set DnDeNormL, 0x00000001
.set DpZero,    0x0000000000000000 #//0_000'0000'0000'_0000'0...0'0000
.set DpZeroH,   0x00000000
.set DpZeroL,   0x00000000
.set DnZero,    0x8000000000000000 #//1_000'0000'0000'_0000'0...0'0000
.set DnZeroH,   0x80000000
.set DnZeroL,   0x00000000
.set DpLFN,     0x7fefffffffffffff #//0_111'1111'1110'_1111'1...1'1111
.set DpLFNH,    0x7fefffff
.set DpLFNL,    0xffffffff 
.set DnLFN,     0xffefffffffffffff #//1_111'1111'1110'_1111'1...1'1111
.set DnLFNH,    0xffefffff 
.set DnLFNL,    0xffffffff
.set DpMIN,     0x0010000000000000 #//0_000'0000'0001'_0000'0...0'0000
.set DpMINH,    0x00100000
.set DpMINL,    0x00000000
.set DnMIN,     0x8010000000000000
.set DnMINH,    0x80100000
.set DnMINL,    0x00000000

.set DpSPECIAL_1,  0x0000000000000002
.set DnSPECIAL_1,  0x8000000000000002
.set DpOne,  0x3ff0000000000000
.set DnOne,  0xbff0000000000000
.set DpONE,  0x3ff0000000000000
.set DnONE,  0xbff0000000000000
.set Dp1p1,  0x3ff8000000000000
.set Dn1p1,  0xbff8000000000000


#//xuwj modified ,add SNaN,QNaN
.set DpqNaN2,   0x7ff800000000000f    #//0_111'1111'1111'_1000'0000...0'1111
.set DpqNaN3,   0x7ff80000000000f0    #//0_111'1111'1111'_1000'0000...'1111'0000
.set DnqNaN2,   0xfff8000000000f00    #//1_111'1111'1111'_1000'0000...0'1111'0000'0000
.set DnqNaN3,   0xfff800000000f000    #//1_111'1111'1111'_1000'0000...'1111'0000'0000'0000
.set DpsNaN2,   0x7ff00000000f0000    #//0_111'1111'1111'_0000'0...0'0001'0000'0000'0000'0000
.set DpsNaN3,   0x7ff0000000f00000    #//0_111'1111'1111'_0000'0...0'0001'0000'0000'0000'0000'0000
.set DnsNaN2,   0x7ff000000f000000    #//0_111'1111'1111'_0000'0...0'0001'0000'0000'0000'0000'0000'0000
.set DnsNaN3,   0x7ff00000f000000    #//0_111'1111'1111'_0000'0...0'0001'0000'0000'0000'0000'0000'0000'0000
.set DpDeNormMost,   0x000fffffffffffff    #//0_000'0000'0000'_ffff...ffff
.set DnDeNormMost,   0x800fffffffffffff    #//1_000'0000'0000'_ffff...ffff


#//==========================================================
#//    deformal
#//==========================================================
#// Single
.set SpNmMAX,  0x7f7fffff #//0_111'1111'0_111'1111'1111'1111'1111'1111
.set SpNmMIN,  0x00800000 #//0_000'0000'1_000'0000'0000'0000'0000'0000
.set SnNmMAX,  0xff7fffff #//1_111'1111'0_111'1111'1111'1111'1111'1111
.set SnNmMIN,  0x80800000 #//1_000'0000'1_000'0000'0000'0000'0000'0000
.set SpDmMAX,  0x007fffff #//0_000'0000'0_111'1111'1111'1111'1111'1111
.set SpDmMIN,  0x00000001 #//0_000'0000'0_000'0000'0000'0000'0000'0001
.set SnDmMAX,  0x807fffff #//1_000'0000'0_111'1111'1111'1111'1111'1111
.set SnDmMIN,  0x80000001 #//0_000'0000'0_000'0000'0000'0000'0000'0001
.set SpOne,    0x3f800000
.set SnOne,    0xbf800000
.set SpqNaN1,  0x7fc00001 #//0_111'1111'1_100'0000'0000'0000'0000'0000
.set SnqNaN1,  0xffc00001 #//1_111'1111'1_100'0000'0000'0000'0000'0000
.set SpOne,    0x3f800000
.set SnOne,    0xbf800000
#// Double
.set DpNmMAX,   0x7fefffffffffffff #//0_111'1111'1110'_1111'1...1'1111
.set DpNmMIN,   0x0010000000000000 #//0_000'0000'0001'_0000'0...0'0000
.set DnNmMAX,   0xffefffffffffffff #//1_111'1111'1110'_1111'1...1'1111
.set DnNmMIN,   0x8010000000000000
.set DpDmMAX,   0x000fffffffffffff #//0_000'0000'0000'_1111'1...1'1111
.set DpDmMIN,   0x0000000000000001 #//0_000'0000'0000'_0000'0...0'0001
.set DnDmMAX,   0x800fffffffffffff #//1_000'0000'0000'_1111'1...1'1111
.set DnDmMIN,   0x8000000000000001 #//1_000'0000'0000'_0000'0...0'0001
.set DpOne,     0x3ff0000000000000
.set DnOne,     0xbff0000000000000
.set DpqNaN1,   0x7ff8000000000001 #//0_111'1111'1111'_1000'0...0'0000
.set DnqNaN1,    0xfff8000000000001 #//1_111'1111'1111'_1000'0...0'0000
.set DpOne,     0x3ff0000000000000
.set DnOne,     0xbff0000000000000











.macro FPUMOVH FDESREG, IMME16, IMM_REG
    li \IMM_REG, \IMME16
    li x3, 0xffff0000
    or \IMM_REG,\IMM_REG,x3
    fmv.w.x \FDESREG, \IMM_REG
.endm

.macro FPUMOVS FDESREG, IMME32, IMM_REG
     li \IMM_REG, \IMME32
     fmv.w.x \FDESREG, \IMM_REG
.endm

.macro FPUMOVD FDESREG,IMME64,IMM_REG
      li \IMM_REG, \IMME64
      fmv.d.x \FDESREG, \IMM_REG
.endm

.macro FPUSCHECKS FDESREG, EXPNUM, IMM_REG0, IMM_REG1
      li \IMM_REG0,0xffffffff00000000
      li \IMM_REG1, \EXPNUM
      or \IMM_REG0,\IMM_REG1,\IMM_REG0
      fmv.x.d \IMM_REG1,\FDESREG
.endm

.macro FPUHCHECKH FDESREG, EXPNUM, IMM_REG0, IMM_REG1
    li \IMM_REG0,0xffffffffffff0000
    li \IMM_REG1, \EXPNUM
    or \IMM_REG0,\IMM_REG1,\IMM_REG0
    fmv.x.d \IMM_REG1,\FDESREG
    #bne \IMM_REG0, \IMM_REG1, RESULT_FAIL
.endm

.macro FPUSCHECKD FDESREG,EXPNUML, IMM_REG0, IMM_REG1
  fmv.x.d \IMM_REG0, \FDESREG
  li   \IMM_REG1, \EXPNUML
.endm

.macro FPUEXCHK EXPNUM32, IMM_REG0, IMM_REG1
    li   \IMM_REG0, 0x7f
    csrrs \IMM_REG1,fflags,x0
    csrrc x0,fflags, \IMM_REG0  #//clear flag bits
    and  \IMM_REG0, \IMM_REG0,\IMM_REG1
    li   \IMM_REG1, \EXPNUM32
.endm

#.macro FPUSCHECKH FDESREG, EXPNUM, EXP_FLAG
#  li x3, 0xffffffffffff0000
#  li x4, \EXPNUM
#  or x3, x4,x3
#  fmv.x.d x4,\FDESREG
#  #bne x3, x4, RESULTFAIL
#  FPUEXCHECK \EXP_FLAG
#.endm

.macro FPUSCHECKH FDESREG, EXPNUM, IMM_REG0, IMM_REG1
    li \IMM_REG0,0xffffffffffff0000
    li \IMM_REG1, \EXPNUM
    or \IMM_REG0,\IMM_REG1,\IMM_REG0
    fmv.x.d \IMM_REG1,\FDESREG
    bne \IMM_REG0, \IMM_REG1, TEST_FAIL
.endm

.macro FPUEXCHECK EXPNUM32   #//xuwj modified, add #bne instruction in macro,use x3x4 default
  #//to see if fccee is enabled
     li   x3, 0x1f
     csrrs x4,fflags,x0
     csrrc x0,fflags, x3  #//clear flag bits
     and  x3, x3,x4
     li   x4, \EXPNUM32
    #bne x3, x4,EXCEPTION_FAIL
.endm

.macro FPUSCHECKH_RESULT FDESREG, EXPNUM
  li x3, 0xffffffffffff0000
  li x4, \EXPNUM
  or x3, x4,x3
  fmv.x.d x4,\FDESREG
  #bne x3, x4, RESULTFAIL
.endm




.macro  FPURMCH IMME2 , IMM_REG0
   li x18, 0xf8ffffff
   csrrs \IMM_REG0, fflags, x0
   and \IMM_REG0, \IMM_REG0,x18
   li x18,\IMME2
   slli x18, x18,24
   or x18, x18,\IMM_REG0
   csrrw x0, fflags,x18
.endm

.macro  FPUQNANCH IMME2, IMM_REG0   ##//imm_reg0 is a tmp reg 
    csrrs  x18, fflags,x0
    li   \IMM_REG0, 0xff7fffff
    and x18,\IMM_REG0,x18
    li  \IMM_REG0, \IMME2
    slli  \IMM_REG0,\IMM_REG0, 23             #//left shift 23  bits
    or    \IMM_REG0, \IMM_REG0,x18            #//set the current QNAN
    csrrw  x0, fflags,\IMM_REG0        #//put in relative vcr
.endm

#//.macro ADDS_H FDESREG0, FDESREG1, RESULT, EXPNUM32
#//    FPUMOVH f12, HpZero  , x3#//PUT CASE TOP
#//    fadd.h f12, \FDESREG0 , \FDESREG1
#//    FPUSCHECKH f12, \RESULT, \EXPNUM32
#//.endm





.macro ADDS_VR FDESREG0, FDESREG1, RESULT, EXPNUM32
   FPUMOVS f12, SpZero  , x3#//PUT CASE TOP
   fadd.s f12, \FDESREG0 , \FDESREG1
   #//CHECK
   FPUSCHECKS f12, \RESULT, x3,x4
   #bne x3, x4, RESULTFAIL
   FPUEXCHK \EXPNUM32 , x3, x4  #//close self check
   #bne x3,x4, FLAGCFAIL
.endm

.macro SUBS_SC IMM1, IMM2, IMM3, EXPNUM32
       FPUMOVS f1 , \IMM1, x3
       FPUMOVS f2 , \IMM2, x3
       fsub.s f12, f1 , f2
        #//CHECK
       FPUSCHECKS f12,\IMM3,x3,x4
       #bne x3,x4, RESULTFAIL
       FPUEXCHK \EXPNUM32 , x4, x3
       #bne x3,x4,  FLAGCFAIL
.endm

.macro SUBD_SC SOURCE1, SOURCE2, RESULT,EXPNUM32
      FPUMOVD f1 , \SOURCE1 ,x3
      FPUMOVD f2 , \SOURCE2, x3
      fsub.d f12, f1 , f2
      FPUSCHECKD f12, \RESULT, x3, x4
      #bne x3,x4, RESULTFAIL
      FPUEXCHK \EXPNUM32 , x3, x4
      #bne x3,x4, FLAGCFAIL
.endm

.macro CHECK_FFLAG  FVAL
  li x4, \FVAL
  li x5, 0x1f
  csrrc x31, fflags, x5
  bne   x31, x4, TEST_FAIL
 
.endm

.macro FIMM_FREG_S FRD, F_IMM
  li x31, \F_IMM
  fmv.w.x \FRD, x31
.endm

.macro FIMM_FREG_D FRD, F_IMM
  li x31, \F_IMM
  fmv.d.x \FRD, x31
.endm

.macro FIMM_FREG_H FRD, F_IMM
  li x31, \F_IMM
  fmv.h.x \FRD, x31
.endm
