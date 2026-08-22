.macro RERI_CFG_RAS_INIT,RECORD_NUM,ERROR_TYPE_NUM0,ERROR_TYPE_NUM1,RAS_INT_TYPE0,RAS_INT_TYPE1
  #backup regs
  addi	x2, x2, -48
  sd	x9, 0(x2)
  sd	x11, 8(x2)
  sd	x12, 16(x2)
  sd	x13, 24(x2)
  sd	x14, 32(x2)
  sd	x15, 40(x2)

  #config reri reg by ciu apb
  #apb wr data : (ras_int_type*2^err_type_num + 3) at addr : 94030000 + (record_num+1)*64  
  # ras int type : high = 1,low = 2,platform = 3
  # err type num : ce = 2,ude = 4,uue = 6

  #record num list:
  # 0:l2c sf                   ce,uue
  # 1:l2c tag                  ce,uue
  # 2:l2c data                 ce,ude
  # 3:core0 ifu consume poison uue
  # 4:core0 icache tag         ce
  # 5:core0 icache data        ce
  # 6:core0 jtlb tag           ce
  # 7:core0 jtlb data          ce
  # 8:core0 lsu consume poison uue
  # 9:core0 dcache tag         ce uue
  #10:core0 dcache data        ce ude
  #11~18:core1 ...
  #19~26:core2 ...
  #27~34:core3 ...

  li    x9 ,0x94030000
  li    x13 ,\RECORD_NUM
  addi  x13,x13,1
  slli  x13,x13,6
  add   x9,x13,x9
  li    x15 ,\RAS_INT_TYPE0
  slli  x15,x15,\ERROR_TYPE_NUM0
  li    x14 ,\RAS_INT_TYPE1
  slli  x14,x14,\ERROR_TYPE_NUM1
  add   x14,x15,x14
  li    x11,0x3
  add   x11,x14,x11
  sw    x11, 0x0(x9)
  li    x12,0x1
  sw    x12, 0x8(x9)

  #restore regs
  ld	x9, 0(x2)
  ld	x11, 8(x2)
  ld	x12, 16(x2)
  ld	x13, 24(x2)
  ld	x14, 32(x2)  
  ld	x15, 40(x2)    
  addi	x2, x2,48
.endm

.macro RERI_APB_READ_REG,RECORD_NUM
  #backup regs
  addi	x2, x2, -56
  sd	x9, 0(x2)
  sd	x11, 8(x2)
  sd	x12, 16(x2)
  sd	x13, 24(x2)
  sd	x14, 32(x2)
  sd	x15, 40(x2)
  sd	x16, 48(x2)

  #read reri reg in int server routine
  #apb rd at addr : 94030000 + (record_num+1)*64  

  li    x9 ,0x94030000
  li    x13 ,\RECORD_NUM
  addi  x13,x13,1
  slli  x13,x13,6
  add   x9,x13,x9
  lw    x11, 0(x9)
  lw    x11, 4(x9)
  lw    x12, 8(x9)
  lw    x12, 12(x9)  
  lw    x13, 16(x9)
  lw    x13, 20(x9)  
  lw    x14, 24(x9)
  lw    x14, 28(x9)  
  lw    x15, 32(x9)
  lw    x15, 36(x9)  
  lw    x16, 40(x9)
  lw    x16, 44(x9)

  #restore regs
  ld	x9, 0(x2)
  ld	x11, 8(x2)
  ld	x12, 16(x2)
  ld	x13, 24(x2)
  ld	x14, 32(x2)
  ld	x15, 40(x2)
  ld	x16, 48(x2)   
  addi	x2, x2,56
.endm

.macro RERI_APB_CLR_REG,RECORD_NUM
  #backup regs
  addi	x2, x2, -32
  sd	x9, 0(x2)
  sd	x11, 8(x2)
  sd	x12, 16(x2)
  sd	x13, 24(x2)

  #read reri reg in int server routine
  #apb wr 0 at addr : 94030000 + (record_num+1)*64  

  li    x9 ,0x94030000
  li    x13 ,\RECORD_NUM
  addi  x13,x13,1
  slli  x13,x13,6
  add   x9,x13,x9
  li    x11,0  
  sw    x11, 0(x9) 
  li    x13,3
  slli  x13,x13,16
  sw    x11, 4(x9) 
  li    x12,0
  sw    x12, 8(x9)


  #restore regs
  ld	x9, 0(x2)
  ld	x11, 8(x2)
  ld	x12, 16(x2)
  sd	x13, 24(x2)  
  addi	x2, x2,32
.endm

.macro RERI_CFG_RAS_INIT_FOR_CONTROL,RECORD_NUM,ERROR_TYPE_NUM0,ERROR_TYPE_NUM1,RAS_INT_TYPE0,RAS_INT_TYPE1
  #backup regs
  addi	x2, x2, -48
  sd	x9, 0(x2)
  sd	x11, 8(x2)
  sd	x12, 16(x2)
  sd	x13, 24(x2)
  sd	x14, 32(x2)
  sd	x15, 40(x2)

  #config reri reg by ciu apb
  #apb wr data : (ras_int_type*2^err_type_num + 3) at addr : 94030000 + (record_num+1)*64  
  # ras int type : high = 1,low = 2,platform = 3
  # err type num : ce = 2,ude = 4,uue = 6

  #record num list:
  # 0:l2c sf                   ce,uue
  # 1:l2c tag                  ce,uue
  # 2:l2c data                 ce,ude
  # 3:core0 ifu consume poison uue
  # 4:core0 icache tag         ce
  # 5:core0 icache data        ce
  # 6:core0 jtlb tag           ce
  # 7:core0 jtlb data          ce
  # 8:core0 lsu consume poison uue
  # 9:core0 dcache tag         ce uue
  #10:core0 dcache data        ce ude
  #11~18:core1 ...
  #19~26:core2 ...
  #27~34:core3 ...

  li    x9 ,0x94030000
  li    x13 ,\RECORD_NUM
  addi  x13,x13,1
  slli  x13,x13,6
  add   x9,x13,x9
  li    x15 ,\RAS_INT_TYPE0
  slli  x15,x15,\ERROR_TYPE_NUM0
  li    x14 ,\RAS_INT_TYPE1
  slli  x14,x14,\ERROR_TYPE_NUM1
  add   x14,x15,x14
  li    x11,0x3
  add   x11,x14,x11
  sw    x11, 0x0(x9)
  li    x11,0x30001
  sw    x11, 0x4(x9)
  li    x12,0x1
  sw    x12, 0x8(x9)

  #restore regs
  ld	x9, 0(x2)
  ld	x11, 8(x2)
  ld	x12, 16(x2)
  ld	x13, 24(x2)
  ld	x14, 32(x2)  
  ld	x15, 40(x2)    
  addi	x2, x2,48
.endm

.macro RERI_CFG_RAS_INIT_FOR_STATUS,RECORD_NUM,ERROR_TYPE_NUM0,ERROR_TYPE_NUM1,RAS_INT_TYPE0,RAS_INT_TYPE1
  #backup regs
  addi	x2, x2, -48
  sd	x9, 0(x2)
  sd	x11, 8(x2)
  sd	x12, 16(x2)
  sd	x13, 24(x2)
  sd	x14, 32(x2)
  sd	x15, 40(x2)

  #config reri reg by ciu apb
  #apb wr data : (ras_int_type*2^err_type_num + 3) at addr : 94030000 + (record_num+1)*64  
  # ras int type : high = 1,low = 2,platform = 3
  # err type num : ce = 2,ude = 4,uue = 6

  #record num list:
  # 0:l2c sf                   ce,uue
  # 1:l2c tag                  ce,uue
  # 2:l2c data                 ce,ude
  # 3:core0 ifu consume poison uue
  # 4:core0 icache tag         ce
  # 5:core0 icache data        ce
  # 6:core0 jtlb tag           ce
  # 7:core0 jtlb data          ce
  # 8:core0 lsu consume poison uue
  # 9:core0 dcache tag         ce uue
  #10:core0 dcache data        ce ude
  #11~18:core1 ...
  #19~26:core2 ...
  #27~34:core3 ...

  li    x9 ,0x94030000
  li    x13 ,\RECORD_NUM
  addi  x13,x13,1
  slli  x13,x13,6
  add   x9,x13,x9
  li    x15 ,\RAS_INT_TYPE0
  slli  x15,x15,\ERROR_TYPE_NUM0
  li    x14 ,\RAS_INT_TYPE1
  slli  x14,x14,\ERROR_TYPE_NUM1
  add   x14,x15,x14
  li    x11,0x3
  add   x11,x14,x11
  sw    x11, 0x0(x9)
#  li    x11,0x30001
#  sw    x11, 0x4(x9)
  li    x12,0x1
  sw    x12, 0x8(x9)
  li    x12,0xffffffff
  sw    x12, 0x8(x9)
  li    x12,0xffffffff
  sw    x12, 0xc(x9) 
  #restore regs
  ld	x9, 0(x2)
  ld	x11, 8(x2)
  ld	x12, 16(x2)
  ld	x13, 24(x2)
  ld	x14, 32(x2)  
  ld	x15, 40(x2)    
  addi	x2, x2,48
.endm

.macro RERI_GET_RANDOM_VALUE_FROM_SEED ,RERI_CFG_OFFSET

  .set RAND_RAS_INT       , (SEED >> ((RERI_INT_SEL + \RERI_CFG_OFFSET) + \RERI_CFG_OFFSET)) % 3
  .set RAND_RAS_EID       , (SEED >> (RERI_INT_SEL + \RERI_CFG_OFFSET) ) % 4
  .set RAND_RAS_SRDP_OR_SINV  , ((SEED << ((RERI_INT_SEL + \RERI_CFG_OFFSET) - 1)) >> (RERI_INT_SEL + \RERI_CFG_OFFSET) ) % 4
  .set RAND_RAS_EVENT_FLAG    , (SEED >>  ((RERI_INT_SEL + \RERI_CFG_OFFSET) + \RERI_CFG_OFFSET)) % 2
  
  # rand_ras_int = 0 -> low ras int
  # rand_ras_int = 1 -> high ras int
  # rand_ras_int = 2 -> plat ras int

  # eid random_range(0,3)

  # srdp sinv
  # 0/1  0/1  -> (0,3)
  
.endm

.macro RERI_CFG_RANDOM_INIT_FOR_CONTROL,RECORD_NUM,ERROR_TYPE_NUM0,ERROR_TYPE_NUM1,RERI_CFG_OFFSET

  # rand_ras_int = 0 -> low ras int
  # rand_ras_int = 1 -> high ras int
  # rand_ras_int = 2 -> plat ras int

  # eid random_range(0,3)

  # srdp sinv
  # 0/1  0/1  -> (0,3)

  .set RAND_RAS_INT       , (SEED >> (RERI_INT_SEL + \RERI_CFG_OFFSET) ) % 3
  .set RAND_RAS_EID       , (SEED >> (RERI_INT_SEL + \RERI_CFG_OFFSET) ) % 4
  .set RAND_RAS_SRDP_OR_SINV  , ((SEED << ((RERI_INT_SEL + \RERI_CFG_OFFSET) - 1)) >> (RERI_INT_SEL + \RERI_CFG_OFFSET)) % 4
  
  #backup regs
  addi	x2, x2, -48
  sd	x9, 0(x2)
  sd	x11, 8(x2)
  sd	x12, 16(x2)
  sd	x13, 24(x2)
  sd	x14, 32(x2)
  sd	x15, 40(x2)

  #config reri reg by ciu apb
  #apb wr data : (ras_int_type*2^err_type_num + 3) at addr : 94030000 + (record_num+1)*64  
  # ras int type : high = 1,low = 2,platform = 3
  # err type num : ce = 2,ude = 4,uue = 6

  #record num list:
  # 0:l2c sf                   ce,uue
  # 1:l2c tag                  ce,uue
  # 2:l2c data                 ce,ude
  # 3:core0 ifu consume poison uue
  # 4:core0 icache tag         ce
  # 5:core0 icache data        ce
  # 6:core0 jtlb tag           ce
  # 7:core0 jtlb data          ce
  # 8:core0 lsu consume poison uue
  # 9:core0 dcache tag         ce uue
  #10:core0 dcache data        ce ude
  #11~18:core1 ...
  #19~26:core2 ...
  #27~34:core3 ...

  li    x9  ,0x94030000
  li    x13 ,\RECORD_NUM
  addi  x13 ,x13,1
  slli  x13 ,x13,6
  add   x9  ,x13,x9
  li    x15 ,RAND_RAS_INT + 1
  slli  x15 ,x15,\ERROR_TYPE_NUM0
  srli  x15 ,x15,2
  slli  x15 ,x15,2
  li    x14 ,RAND_RAS_INT + 1
  slli  x14 ,x14,\ERROR_TYPE_NUM1
  srli  x14 ,x14,2
  slli  x14 ,x14,2
  add   x14 ,x15,x14
  li    x11 ,0x3
  add   x11 ,x14,x11
  sw    x11 ,0x0(x9)
  li    x12 ,RAND_RAS_EID
  li    x11 ,RAND_RAS_SRDP_OR_SINV
  slli  x11 ,x11, 16
  add   x11 ,x12,x11 
  sw    x11 , 0x4(x9)
  li    x11 ,0x0
  sw    x11 ,0x8(x9)  

  #restore regs
  ld	x9, 0(x2)
  ld	x11, 8(x2)
  ld	x12, 16(x2)
  ld	x13, 24(x2)
  ld	x14, 32(x2)  
  ld	x15, 40(x2)    
  addi	x2, x2,48
.endm

.macro RERI_CFG_INIT_FOR_STATUS,RECORD_NUM,ERROR_TYPE_NUM0,ERROR_TYPE_NUM1
  #backup regs
  addi	x2, x2, -40
  sd	x9, 0(x2)
  sd	x11, 8(x2)
  sd	x12, 16(x2)
  sd	x13, 24(x2)
  sd	x14, 32(x2)

  li    x9  ,0x94030000
  li    x13 ,\RECORD_NUM
  addi  x13 ,x13,1
  slli  x13 ,x13,6
  add   x9  ,x13,x9
  li    x12 ,0
  li    x14 ,0
  li    x11 , \ERROR_TYPE_NUM0
  beqz  x11 ,3f
  srli  x11 ,x11,1
  li    x12 ,0x1
  sll   x12 ,x12 ,x11
  3:
  li    x11 , \ERROR_TYPE_NUM1
  beqz  x11 ,4f  
  srli  x11 ,x11,1
  li    x14 ,0x1
  sll   x14 ,x14 ,x11
  4:
  add   x12 ,x14 ,x12
  sw    x12 ,0x8(x9)

  #restore regs
  ld	x9, 0(x2)
  ld	x11, 8(x2)
  ld	x12, 16(x2)
  ld	x13, 24(x2)
  ld	x14, 32(x2)  
  addi	x2, x2,40
.endm

.macro RERI_APB_READ_REG_WITH_CHECK,RECORD_NUM, ERROR_TYPE_NUM0,ERROR_TYPE_NUM1,RERI_CFG_OFFSET

  .set RAS_INT       , (SEED >> (RERI_INT_SEL + \RERI_CFG_OFFSET) ) % 3
  .set RAS_SRDP_OR_SINV  , ((SEED << ((RERI_INT_SEL + \RERI_CFG_OFFSET) - 1)) >> (RERI_INT_SEL + \RERI_CFG_OFFSET)) % 4

  .set RERI_EVENT0_STATUS_BASE, 0x7020001
  .set RERI_EVENT1_STATUS_BASE, 0x5020001
  .set RERI_EVENT2_STATUS_BASE, 0x3020001
  .set RERI_EVENT3_STATUS_BASE, 0x2020401
  .set RERI_EVENT4_STATUS_BASE, 0x5120401
  .set RERI_EVENT5_STATUS_BASE, 0x3120401
  .set RERI_EVENT6_STATUS_BASE, 0xa120601
  .set RERI_EVENT7_STATUS_BASE, 0x9120601
  .set RERI_EVENT8_STATUS_BASE, 0x2020001
  .set RERI_EVENT9_STATUS_BASE, 0x5120001
  .set RERI_EVENT10_STATUS_BASE,0x3120001
  .set RERI_EVENT11_STATUS_BASE, 0x2020401
  .set RERI_EVENT12_STATUS_BASE, 0x5120401
  .set RERI_EVENT13_STATUS_BASE, 0x3120401
  .set RERI_EVENT14_STATUS_BASE, 0xa120601
  .set RERI_EVENT15_STATUS_BASE, 0x9120601
  .set RERI_EVENT16_STATUS_BASE, 0x2020001
  .set RERI_EVENT17_STATUS_BASE, 0x5120001
  .set RERI_EVENT18_STATUS_BASE,0x3120001 
  .set RERI_EVENT19_STATUS_BASE, 0x2020401
  .set RERI_EVENT20_STATUS_BASE, 0x5120401
  .set RERI_EVENT21_STATUS_BASE, 0x3120401
  .set RERI_EVENT22_STATUS_BASE, 0xa120601
  .set RERI_EVENT23_STATUS_BASE, 0x9120601
  .set RERI_EVENT24_STATUS_BASE, 0x2020001
  .set RERI_EVENT25_STATUS_BASE, 0x5120001
  .set RERI_EVENT26_STATUS_BASE,0x3120001
  .set RERI_EVENT27_STATUS_BASE, 0x2020401
  .set RERI_EVENT28_STATUS_BASE, 0x5120401
  .set RERI_EVENT29_STATUS_BASE, 0x3120401
  .set RERI_EVENT30_STATUS_BASE, 0xa120601
  .set RERI_EVENT31_STATUS_BASE, 0x9120601
  .set RERI_EVENT32_STATUS_BASE, 0x2020001
  .set RERI_EVENT33_STATUS_BASE, 0x5120001
  .set RERI_EVENT34_STATUS_BASE,0x3120001
  
  #backup regs
  addi	x2, x2, -56
  sd	x9, 0(x2)
  sd	x11, 8(x2)
  sd	x12, 16(x2)
  sd	x13, 24(x2)
  sd	x14, 32(x2)
  sd	x15, 40(x2)
  sd	x16, 48(x2)

  #read reri reg in int server routine
  #apb rd at addr : 94030000 + (record_num+1)*64  

  1:
  li    x13 ,\RECORD_NUM
  li    x16 ,RERI_EVENT0_STATUS_BASE    
  beqz  x13 ,2f
  addi  x13 ,x13,-1
  li    x16 ,RERI_EVENT1_STATUS_BASE 
  beqz  x13 ,2f
  addi  x13 ,x13,-1  
  li    x16 ,RERI_EVENT2_STATUS_BASE
  beqz  x13 ,2f
  addi  x13 ,x13,-1  
  li    x16 ,RERI_EVENT3_STATUS_BASE 
  beqz  x13 ,2f
  addi  x13 ,x13,-1
  li    x16 ,RERI_EVENT4_STATUS_BASE
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT5_STATUS_BASE 
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT6_STATUS_BASE 
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT7_STATUS_BASE 
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT8_STATUS_BASE 
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT9_STATUS_BASE 
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT10_STATUS_BASE 
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT11_STATUS_BASE 
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT12_STATUS_BASE 
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT13_STATUS_BASE
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT14_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT15_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT16_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT17_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT18_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT19_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT20_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT21_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT22_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT23_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT24_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT25_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT26_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT27_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT28_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT29_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT30_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT31_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT32_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT33_STATUS_BASE   
  beqz  x13 ,2f
  addi  x13 ,x13,-1    
  li    x16 ,RERI_EVENT34_STATUS_BASE   
 
 
  2:
  li    x9 ,0x94030000
  li    x13 ,\RECORD_NUM
  addi  x13,x13,1
  slli  x13,x13,6
  add   x9,x13,x9

  #control value check
  lw    x11, 0(x9)
  lw    x12, 4(x9)
  add   x12, x11, x12
  li    x15 , RAS_INT + 1
  slli  x15 ,x15,\ERROR_TYPE_NUM0
  srli  x15 ,x15,2
  slli  x15 ,x15,2
  li    x14 , RAS_INT + 1
  slli  x14 ,x14,\ERROR_TYPE_NUM1
  srli  x14 ,x14,2
  slli  x14 ,x14,2
  add   x14 ,x15,x14
#  li    x11 , RAS_SRDP_OR_SINV
#  slli  x11 ,x11, 48
#  add   x14 ,x11,x14
  addi  x14 ,x14,3  
  bne   x14 ,x12,TEST_FAIL

  #status value check
  lw    x12, 8(x9)
  lw    x13, 12(x9) 
  add   x13, x12, x13
  li    x12 ,0
  li    x14 ,0  
  li    x11 , \ERROR_TYPE_NUM0
  beqz  x11 ,5f  
  srli  x11 ,x11,1
  li    x12 ,0x1
  sll   x12 ,x12 ,x11
  5:
  li    x11 , \ERROR_TYPE_NUM1
  beqz  x11 ,6f    
  srli  x11 ,x11,1
  li    x14 ,0x1
  sll   x14 ,x14 ,x11
  6:  
  add   x14 ,x12 ,x14
  add   x14 ,x16 ,x14
  bne   x14 ,x13,TEST_FAIL

  #restore regs
  ld	x9, 0(x2)
  ld	x11, 8(x2)
  ld	x12, 16(x2)
  ld	x13, 24(x2)
  ld	x14, 32(x2)
  ld	x15, 40(x2)
  ld	x16, 48(x2)   
  addi	x2, x2,56
.endm

.macro EID_CHECK_FROM_CONTROL,RECORD_NUM
  #backup regs
  addi	x2, x2, -40
  sd	x9, 0(x2)
  sd	x11, 8(x2)
  sd	x12, 16(x2)
  sd	x13, 24(x2)
  sd	x14, 32(x2)

  li    x9  ,0x94030000
  li    x13 ,\RECORD_NUM
  addi  x13 ,x13,1
  slli  x13 ,x13,6
  add   x9  ,x13,x9
  lw    x12 ,0x4(x9)
  bnez  x12 ,TEST_FAIL 

  #restore regs
  ld	x9, 0(x2)
  ld	x11, 8(x2)
  ld	x12, 16(x2)
  ld	x13, 24(x2)
  ld	x14, 32(x2)  
  addi	x2, x2,40
.endm

.macro SAVE_RERI_REG_RESET_VALUE,RECORD_ADDR_OFFSET,RERI_SAVE_ADDR
  #backup regs
  addi	x2, x2, -40
  sd	x9, 0(x2)
  sd	x11, 8(x2)
  sd	x12, 16(x2)
  sd	x13, 24(x2)
  sd	x14, 32(x2)
 
  li    x9  ,0x94030000
  li    x13 ,\RECORD_ADDR_OFFSET
  add   x9  ,x13,x9
  lw    x12 ,0(x9)
  la    x14 ,\RERI_SAVE_ADDR
  add   x14  ,x13,x14  
  sw    x12 ,0(x14)
  lw    x12 ,4(x9)
  addi  x14, x14, 4  
  sw    x12 ,0(x14)
  lw    x12 ,8(x9)
  addi  x14, x14, 4  
  sw    x12 ,0(x14)
  lw    x12 ,12(x9)
  addi  x14, x14, 4  
  sw    x12 ,0(x14)

  #restore regs
  ld	x9, 0(x2)
  ld	x11, 8(x2)
  ld	x12, 16(x2)
  ld	x13, 24(x2)
  ld	x14, 32(x2)  
  addi	x2, x2,40

.endm

.macro CHECK_RERI_REG_RESET_VALUE,RECORD_ADDR_OFFSET,RERI_SAVE_ADDR
  #backup regs
  addi	x2, x2, -40
  sd	x9, 0(x2)
  sd	x11, 8(x2)
  sd	x12, 16(x2)
  sd	x13, 24(x2)
  sd	x14, 32(x2)
 
  li    x9  ,0x94030000
  li    x13 ,\RECORD_ADDR_OFFSET
#  addi  x13 ,x13,1
#  slli  x13 ,x13,6
  add   x9  ,x13,x9
  lw    x12 ,0(x9)
  la    x14 ,\RERI_SAVE_ADDR
  add   x14  ,x13,x14
  lw    x15 ,0(x14)
  bne   x15 ,x12,TEST_FAIL
  lw    x12 ,4(x9)
  addi  x14, x14, 4  
  lw    x15 ,0(x14)
  bne   x15 ,x12,TEST_FAIL  
  lw    x12 ,8(x9)
  addi  x14, x14, 4  
  lw    x15 ,0(x14)
  bne   x15 ,x12,TEST_FAIL  
  lw    x12 ,12(x9)
  addi  x14, x14, 4  
  lw    x15 ,0(x14)
  bne   x15 ,x12,TEST_FAIL

  #restore regs
  ld	x9, 0(x2)
  ld	x11, 8(x2)
  ld	x12, 16(x2)
  ld	x13, 24(x2)
  ld	x14, 32(x2)  
  addi	x2, x2,40

.endm


.macro CHECK_RERI_ERR_HEAD_REG_VALUE,RECORD_NUM
  .set VENDOR_ID, 0x710
  .set IMP_ID, 0x0
  .set CLUSTER_ID, 0x0  
  .set VERSION, 0x1
  .set ERR_RECS, 0x23

  .set SUMMARY_EN, 0x1

  #backup regs
  addi	x2, x2, -48
  sd	x9, 0(x2)
  sd	x11, 8(x2)
  sd	x12, 16(x2)
  sd	x13, 24(x2)
  sd	x14, 32(x2)
  sd	x15, 40(x2)
 
  li    x9  ,0x94030000
  #vendor and imp id check
  lw    x12 ,0(x9)
  li    x13 ,VENDOR_ID
  bne   x13 ,x12,TEST_FAIL
  lw    x12 ,4(x9)
  li    x13 ,IMP_ID
  bne   x13 ,x12,TEST_FAIL
  #bank info check  
  lw    x12 ,8(x9)
  li    x13 ,ERR_RECS
  slli  x13 ,x13,16 
  li    x14 ,CLUSTER_ID
  add   x13 ,x14 ,x13
  bne   x13 ,x12,TEST_FAIL  
  lw    x12 ,12(x9)
  li    x13 ,VERSION
  slli  x13 ,x13,24
  bne   x13 ,x12,TEST_FAIL  
  #valid summary check  
  lw    x12 ,16(x9)
  li    x13 ,SUMMARY_EN
  li    x15 ,\RECORD_NUM
  addi  x15 ,x15,1
  sll   x13 ,x13,x15
  addi  x13 ,x13,1  
  bne   x13 ,x12,TEST_FAIL
  lw    x12 ,20(x9)
  li    x13 ,0x0  
  bne   x13 ,x12,TEST_FAIL

  #restore regs
  ld	x9, 0(x2)
  ld	x11, 8(x2)
  ld	x12, 16(x2)
  ld	x13, 24(x2)
  ld	x14, 32(x2)  
  ld	x15, 40(x2)    
  addi	x2, x2,48
.endm
