.macro VLE32_TASK DVREG,BASE
#backup regs
addi x2, x2, -24
sd x15, 0x0(x2)
sd x16, 0x8(x2)
sd x17, 0x10(x2)

#backup vtype vl vstart
csrr	x15,vtype
csrr	x16,vl
csrr	x17,vstart

addi	x2, x2, -24
sd	x15,0x0(x2)
sd	x16,0x8(x2)
sd	x17,0x10(x2)

#modify sew
li   x17,0x7
and  x15,x15,x17 #vlmul
li   x17,0x2 #vsew =32
slli x17,x17,0x3
or   x17,x17,x15
vsetvl x0,x16,x17



  #get element num
  csrr x16,vtype
  li   x15,0x3
  and  x16,x16,x15 #get vlmul
  li   x15,0x1
  sll  x16,x15,x16 #get LMUL value 1/2/4/8
  slli x16,x16,0x3 #LMUL*(VLEN/SEW) = ELEMENT NUM
  
  #init data
  mv  x17,\BASE
#init_data:  
1:  
  lwu x15,0(x17)
  vslide1down.vx \DVREG,\DVREG,x15
  addi x17,x17,0x4
  addi x16,x16,-1
  bnez x16, 1b

#restore vtype vl vstart
#vtype
ld	x15,0x0(x2)
#vl
ld	x16,0x8(x2)
#start
ld	x17,0x10(x2)
addi	x2, x2, 24

vsetvl	x0,x16,x15
csrw	vstart,x17

#restore regs
ld x15, 0x0(x2)
ld x16, 0x8(x2)
ld x17, 0x10(x2)
addi x2, x2, 24  
.endm

.macro VLE16_TASK DVREG,BASE
#backup regs
addi x2, x2, -24
sd x15, 0x0(x2)
sd x16, 0x8(x2)
sd x17, 0x10(x2)

#backup vtype vl vstart
csrr	x15,vtype
csrr	x16,vl
csrr	x17,vstart

addi	x2, x2, -24
sd	x15,0x0(x2)
sd	x16,0x8(x2)
sd	x17,0x10(x2)

#modify sew
li   x17,0x7
and  x15,x15,x17 #vlmul
li   x17,0x1     #vsew = 16
slli x17,x17,0x3
or   x17,x17,x15
vsetvl x0,x16,x17


  #get element num
  csrr x16,vtype
  li   x15,0x3
  and  x16,x16,x15 #get vlmul
  li   x15,0x1
  sll  x16,x15,x16 #get LMUL value 1/2/4/8
  slli x16,x16,0x4 #LMUL*(VLEN/SEW) = ELEMENT NUM
  
  #init data
  mv  x17,\BASE
#init_data:  
1:  
  lhu x15,0(x17)
  vslide1down.vx \DVREG,\DVREG,x15
  addi x17,x17,0x2
  addi x16,x16,-1
  bnez x16, 1b
 
#restore vtype vl vstart
#vtype
ld	x15,0x0(x2)
#vl
ld	x16,0x8(x2)
#start
ld	x17,0x10(x2)
addi	x2, x2, 24

vsetvl	x0,x16,x15
csrw	vstart,x17
  
#restore regs
ld x15, 0x0(x2)
ld x16, 0x8(x2)
ld x17, 0x10(x2)
addi x2, x2, 24  
.endm

.macro VLE8_TASK DVREG,BASE
#backup regs
addi x2, x2, -24
sd x15, 0x0(x2)
sd x16, 0x8(x2)
sd x17, 0x10(x2)
#backup vtype vl vstart
csrr	x15,vtype
csrr	x16,vl
csrr	x17,vstart

addi	x2, x2, -24
sd	x15,0x0(x2)
sd	x16,0x8(x2)
sd	x17,0x10(x2)

#modify sew
li   x17,0x7
and  x15,x15,x17 #vlmul
li   x17,0x0 #vsew = 8 
slli x17,x17,0x3
or   x17,x17,x15
vsetvl x0,x16,x17
  
  #get element num
  csrr x16,vtype
  li   x15,0x3
  and  x16,x16,x15 #get vlmul
  li   x15,0x1
  sll  x16,x15,x16 #get LMUL value 1/2/4/8
  slli x16,x16,0x5 #LMUL*(VLEN/SEW) = ELEMENT NUM
  
  #init data
  mv  x17,\BASE
#init_data:  
1:  
  lbu x15,0(x17)
  vslide1down.vx \DVREG,\DVREG,x15
  addi x17,x17,0x1
  addi x16,x16,-1
  bnez x16, 1b
 
#restore vtype vl vstart
#vtype
ld	x15,0x0(x2)
#vl
ld	x16,0x8(x2)
#start
ld	x17,0x10(x2)
addi	x2, x2, 24

vsetvl	x0,x16,x15
csrw	vstart,x17

#restore regs
ld x15, 0x0(x2)
ld x16, 0x8(x2)
ld x17, 0x10(x2)
addi x2, x2, 24  
.endm


.macro VSE32_TASK SVREG,BASE
#backup regs
addi x2, x2, -24
sd x15, 0x0(x2)
sd x16, 0x8(x2)
sd x17, 0x10(x2)

  #get element num
  csrr x16,vtype
  li   x15,0x3
  and  x16,x16,x15 #get vlmul
  li   x15,0x1
  sll  x16,x15,x16 #get LMUL value 1/2/4/8
  slli x16,x16,0x3 #LMUL*(VLEN/SEW) = ELEMENT NUM
  
  #st data
  mv  x17,\BASE
#init_mem:  
1:
  vmv.x.s x15,\SVREG
  sw   x15,0(x17)
  vslide1down.vx \SVREG,\SVREG,x0
  addi x17,x17,0x4
  addi x16,x16,-1
  bnez x16,1b
 
#restore regs
ld x15, 0x0(x2)
ld x16, 0x8(x2)
ld x17, 0x10(x2)
addi x2, x2, 24  
.endm

.macro VSE16_TASK SVREG,BASE
#backup regs
addi x2, x2, -24
sd x15, 0x0(x2)
sd x16, 0x8(x2)
sd x17, 0x10(x2)

  #get element num
  csrr x16,vtype
  li   x15,0x3
  and  x16,x16,x15 #get vlmul
  li   x15,0x1
  sll  x16,x15,x16 #get LMUL value 1/2/4/8
  slli x16,x16,0x4 #LMUL*(VLEN/SEW) = ELEMENT NUM
  
  #st data
  mv  x17,\BASE
#init_mem:  
1:
  vmv.x.s x15,\SVREG
  sh   x15,0(x17)
  vslide1down.vx \SVREG,\SVREG,x0
  addi x17,x17,0x2
  addi x16,x16,-1
  bnez x16,1b
 
#restore regs
ld x15, 0x0(x2)
ld x16, 0x8(x2)
ld x17, 0x10(x2)
addi x2, x2, 24  
.endm

.macro VSE8_TASK SVREG,BASE
#backup regs
addi x2, x2, -24
sd x15, 0x0(x2)
sd x16, 0x8(x2)
sd x17, 0x10(x2)

  #get element num
  csrr x16,vtype
  li   x15,0x3
  and  x16,x16,x15 #get vlmul
  li   x15,0x1
  sll  x16,x15,x16 #get LMUL value 1/2/4/8
  slli x16,x16,0x5 #LMUL*(VLEN/SEW) = ELEMENT NUM
  
  #st data
  mv  x17,\BASE
#init_mem:  
1:
  vmv.x.s x15,\SVREG
  sb   x15,0(x17)
  vslide1down.vx \SVREG,\SVREG,x0
  addi x17,x17,0x1
  addi x16,x16,-1
  bnez x16,1b
 
#restore regs
ld x15, 0x0(x2)
ld x16, 0x8(x2)
ld x17, 0x10(x2)
addi x2, x2, 24  
.endm

.macro COMPARE_WITH_IMM  SREG,IMM
#backup regs
addi x2, x2, -24
sd \SREG, 0x0(x2)
sd   x15, 0x8(x2)
sd   x16, 0x10(x2)

li x15,\IMM
ld   x16, 0x0(x2)
bne  x15, x16,TEST_FAIL



#restore regs
ld x16, 0x10(x2)
ld x15, 0x8(x2)
ld \SREG, 0x0(x2)
addi x2, x2, 24
.endm


.macro INIT_VREG_IMM256 VREG, d64, c64, b64, a64
#backup regs 
addi x2, x2, -24
sd x15, 0x0(x2)
sd x16, 0x8(x2)
sd x17, 0x10(x2)

#backup vtype vl vstart
csrr	x15,vtype
csrr	x16,vl
csrr	x17,vstart

addi	x2, x2, -24
sd	x15,0x0(x2)
sd	x16,0x8(x2)
sd	x17,0x10(x2)

#modify vtype vl to lmul 1 sew 64b vl 1 
#reset vstart to 0
#li	x16,0x1
vsetvli	x16,x0,e32,m1

#updt 1st 64 bit 
li x16, 0x20
li		x15, \a64
vslide1down.vx	\VREG,\VREG,x15
srl x15,x15,x16
vslide1down.vx	\VREG,\VREG,x15
#updt 2nd 64 bit 
li		x15, \b64
vslide1down.vx	\VREG,\VREG,x15
srl x15,x15,x16
vslide1down.vx	\VREG,\VREG,x15
#updt 3th 64 bit
li		x15, \c64
vslide1down.vx	\VREG,\VREG,x15
srl x15,x15,x16
vslide1down.vx	\VREG,\VREG,x15
#updt 4th 64 bit
li		x15, \d64
vslide1down.vx	\VREG,\VREG,x15
srl x15,x15,x16
vslide1down.vx	\VREG,\VREG,x15

#restore vtype vl vstart
#vtype
ld	x15,0x0(x2)
#vl
ld	x16,0x8(x2)
#vstart
ld	x17,0x10(x2)
addi	x2, x2, 24

vsetvl	x0,x16,x15
csrw	vstart,x17

#restore regs
ld x15, 0x0(x2)
ld x16, 0x8(x2)
ld x17, 0x10(x2)
addi x2, x2, 24
.endm

.macro TESTFLOAT_FVAL2VREG VREG, FVAL
  li x31, \FVAL
  vmv.v.x \VREG, x31
.endm

.macro TESTFLOAT_CHECK_VREG CMP_VD, CMP_REG, EXP_VAL 
   csrr	x15,vtype
   
   vsetvli x31, x0, e64, m1
   vmv.v.x v16, x0
   vmseq.vv v16, \CMP_VD, \CMP_REG
   vmv.x.s x10, v16
   li x31, \EXP_VAL
   bne x10, x31, TEST_FAIL

   vsetvl	x5,x0,x15
.endm

