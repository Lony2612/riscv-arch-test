.set MB_INTPCBASE   ,0x9ff01000
.set MB_INTTYPEBASE ,0x9ff01008
.set MB_RANDENBASE  ,0x9ff01010
.set MB_RESETNBASE  ,0x9ff01018
.set MB_MSTATUS_C0   ,0x9ff01200
.set MB_MSTATUS_C1   ,0x9ff01208
.set MB_MSTATUS_C2   ,0x9ff01210
.set MB_MSTATUS_C3   ,0x9ff01218
.set MB_MSTATUS_C4   ,0x9ff01220
.set MB_MSTATUS_C5   ,0x9ff01228
.set MB_MSTATUS_C6   ,0x9ff01230
.set MB_MSTATUS_C7   ,0x9ff01238
.set MB_CORE_PAD_HALTED,0x9ff01280
.set MB_CORE_PAD_LPMD,0x9ff01288
.set MB_CORE_PAD_NO_OP,0x9ff01290
.set MB_PAD_CPU_L2_REQ,0x9ff01298
.set MB_CPU_PAD_L2_DONE,0x9ff012a0
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
   mul  x11,x9,x10

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

.macro  MB_RESETN CORENUM,RSTN 
   addi x2,x2,-56
   sd   x9,0(x2)
   sd   x10,8(x2)
   sd   x11,16(x2)
   sd   x12,24(x2)

   #cal each core start int_num addr
   li   x9,\CORENUM
   li   x10,MB_OFFSET
   mul  x10,x9,x10
   li   x11,MB_RESETNBASE
   add  x11,x11,x10

   #reset enable 
   li   x12,\RSTN
   sd   x12,0x0(x11)

   ld   x9,0(x2)
   ld   x10,8(x2)
   ld   x11,16(x2)
   ld   x12,24(x2)
   addi x2,x2,56
   
   fence

.endm
