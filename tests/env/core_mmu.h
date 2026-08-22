###########################################################
#            MACRO DEFINITION                             #
###########################################################

.macro MMU_PTW_1G VPN, PPN, FLAG, PBMT
  #backup regs 
  addi	x2, x2, -56
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
   
  # get PPN from satp in x9
  csrr  x9, satp
  li    x10, 0xfffffffffff
  and   x9, x9, x10
  
  # get VPN2 in x10
  li    x10, \VPN
  li    x11, 0x7fc0000
  and   x10,x10,x11
  srli  x10, x10, 18    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN2,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  li    x11, \PPN
  li    x12, \PBMT
  li    x13, \FLAG
  slli  x11, x11, 10
  slli  x12, x12, 61
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x15)
  
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  addi	x2, x2, 56
  # fence 
  fence
  sfence.vma
.endm

.macro MMU_SV39_1G VPN, PPN, FLAGHEAD, FLAGTAIL, ATP #note
  #backup regs 
  addi	x2, x2, -56
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
   
  # get *atp.ppn in x9
  li    x9, \ATP
  srli  x9, x9, 12
  
  # get VPN2 in x10
  li    x10, \VPN
  li    x11, 0x7fc0000
  and   x10,x10,x11
  srli  x10, x10, 18    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN2,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x15)
  
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  addi	x2, x2, 56
  # fence 
  fence
  sfence.vma
.endm

.macro MMU_SV39_1G_VS VPN, PPN, FLAGHEAD, FLAGTAIL, ATP
  #backup regs 
  addi	x2, x2, -56
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
   
  # get *atp.ppn in x9
  li    x9, \ATP
  srli  x9, x9, 12
  
  # get VPN2 in x10
  li    x10, \VPN
  li    x11, 0x7fc0000
  and   x10,x10,x11
  srli  x10, x10, 18    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN2,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  hsv.d x11, 0(x15)
  
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  addi	x2, x2, 56
  # fence 
  fence
  hfence.vvma
.endm


.macro MMU_SV48_512G VPN, PPN, FLAGHEAD, FLAGTAIL, ATP
  #backup regs 
  addi	x2, x2, -56
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
   
  # get *atp.ppn in x9
  li    x9, \ATP
  srli  x9, x9, 12
  
  # get VPN3 in x10
  li    x10, \VPN
  li    x11, 0xff8000000
  and   x10,x10,x11
  srli  x10, x10, 27    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN3,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x15)
  
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  addi	x2, x2, 56
  # fence 
  fence
  sfence.vma
.endm

.macro MMU_SV48_512G_VS VPN, PPN, FLAGHEAD, FLAGTAIL, ATP
  #backup regs 
  addi	x2, x2, -56
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
   
  # get *atp.ppn in x9
  li    x9, \ATP
  srli  x9, x9, 12
  
  # get VPN3 in x10
  li    x10, \VPN
  li    x11, 0xff8000000
  and   x10,x10,x11
  srli  x10, x10, 27    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN3,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  hsv.d x11, 0(x15)
  
  #restore regs
  ld 	x9, 0(x2)
  ld 	x10, 8(x2)
  ld 	x11, 16(x2)
  ld 	x12, 24(x2)
  ld 	x13, 32(x2)
  ld 	x14, 40(x2)
  ld 	x15, 48(x2)
  addi	x2, x2, 56
  # fence 
  fence
  hfence.vvma
.endm

.macro MMU_SV39X4_1G VPN, PPN, FLAGHEAD, FLAGTAIL, ATP
  #backup regs 
  addi	x2, x2, -56
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
   
  # get *atp.ppn in x9
  li    x9, \ATP
  srli  x9, x9, 12
  
  # get VPN2 in x10
  li    x10, \VPN
  li    x11, 0x1ffc0000
  and   x10,x10,x11
  srli  x10, x10, 18    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN2,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x15)
  
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  addi	x2, x2, 56
  # fence 
  fence
  hfence.gvma
.endm

.macro MMU_SV48X4_512G VPN, PPN, FLAGHEAD, FLAGTAIL, ATP
  #backup regs 
  addi	x2, x2, -56
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
   
  # get *atp.ppn in x9
  li    x9, \ATP
  srli  x9, x9, 12
  
  # get VPN3 in x10
  li    x10, \VPN
  li    x11, 0x3ff8000000
  and   x10,x10,x11
  srli  x10, x10, 27    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN3,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x15)
  
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  addi	x2, x2, 56
  # fence 
  fence
  hfence.gvma
.endm


.macro MMU_PTW_2M VPN, PPN, FLAG, PBMT
  #backup regs 
  addi	x2, x2, -88
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
   
  # get PPN from satp in x9
  csrr  x9, satp
  li    x10, 0xfffffffffff
  and   x9, x9, x10
  
  # get VPN2 in x10
  li    x10, \VPN
  li    x11, 0x7fc0000
  and   x10,x10,x11
  srli  x10, x10, 18    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN2,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  # level-2 base addr in x12
  addi  x12, x10, 1
  add   x12, x12, x9
  slli  x14, x12, 10
  li    x13, 0x01
  or    x13, x13, x14
  sd    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN1 in x16
  li    x11, \VPN
  li    x13, 0x3fe00
  and   x16, x11, x13
  srli  x16, x16, 9  
  # level-2 page, entry addr:{pte.ppn,VPN1,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x11, \PPN
  li    x12, \PBMT
  li    x13, \FLAG
  slli  x11, x11, 10
  slli  x12, x12, 61
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x17)
  
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  addi	x2, x2, 88
  # fence 
  fence
  sfence.vma
.endm

.macro MMU_SV39_2M VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, ATP, PTR1
  #backup regs 
  addi	x2, x2, -88
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
   
  # get *atp.ppn in x9
  li    x9, \ATP
  srli  x9, x9, 12
  
  # get VPN2 in x10
  li    x10, \VPN
  li    x11, 0x7fc0000
  and   x10,x10,x11
  srli  x10, x10, 18    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN2,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page flag=FLAG1TAIL
  # level-2 base addr in x12
  li    x12, \PTR1
  srli  x12, x12, 12
  slli  x14, x12, 10
  li    x18, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x18, x18, 54
  or    x13, x13, x14
  or    x13, x13, x18
  sd    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN1 in x16
  li    x11, \VPN
  li    x13, 0x3fe00
  and   x16, x11, x13
  srli  x16, x16, 9  
  # level-2 page, entry addr:{pte.ppn,VPN1,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x17)
  
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  addi	x2, x2, 88
  # fence 
  fence
  sfence.vma
.endm

.macro MMU_SV39_2M_VS VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, ATP, PTR1
  #backup regs 
  addi	x2, x2, -88
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
   
  # get *atp.ppn in x9
  li    x9, \ATP
  srli  x9, x9, 12
  
  # get VPN2 in x10
  li    x10, \VPN
  li    x11, 0x7fc0000
  and   x10,x10,x11
  srli  x10, x10, 18    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN2,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page flag=FLAG1
  # level-2 base addr in x12
  li    x12, \PTR1
  srli  x12, x12, 12
  slli  x14, x12, 10
  li    x18, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x18, x18, 54
  or    x13, x13, x14
  or    x13, x13, x18
  hsv.d x13, 0(x15)
  
  # cfig level-2 page
  # get VPN1 in x16
  li    x11, \VPN
  li    x13, 0x3fe00
  and   x16, x11, x13
  srli  x16, x16, 9  
  # level-2 page, entry addr:{pte.ppn,VPN1,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  hsv.d x11, 0(x17)
  
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  addi	x2, x2, 88
  # fence 
  fence
  hfence.vvma
.endm

.macro MMU_SV48_1G VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, ATP, PTR1
  #backup regs 
  addi	x2, x2, -88
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
   
  # get *atp.ppn in x9
  li    x9, \ATP
  srli  x9, x9, 12
  
  # get VPN3 in x10
  li    x10, \VPN
  li    x11, 0xff8000000
  and   x10,x10,x11
  srli  x10, x10, 27    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN3,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page flag=FLAG1
  # level-2 base addr in x12
  li    x12, \PTR1
  srli  x12, x12, 12
  slli  x14, x12, 10
  li    x18, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x18, x18, 54
  or    x13, x13, x14
  or    x13, x13, x18
  sd    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN2 in x16
  li    x11, \VPN
  li    x13, 0x7fc0000
  and   x16, x11, x13
  srli  x16, x16, 18  
  # level-2 page, entry addr:{pte.ppn,VPN2,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x17)
  
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  addi	x2, x2, 88
  # fence 
  fence
  sfence.vma
.endm

.macro MMU_SV48_1G_VS VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, ATP, PTR1
  #backup regs 
  addi	x2, x2, -88
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
   
  # get *atp.ppn in x9
  li    x9, \ATP
  srli  x9, x9, 12
  
  # get VPN3 in x10
  li    x10, \VPN
  li    x11, 0xff8000000
  and   x10,x10,x11
  srli  x10, x10, 27    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN3,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page flag=FLAG1
  # level-2 base addr in x12
  li    x12, \PTR1
  srli  x12, x12, 12
  slli  x14, x12, 10
  li    x18, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x18, x18, 54
  or    x13, x13, x14
  or    x13, x13, x18
  hsv.d    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN2 in x16
  li    x11, \VPN
  li    x13, 0x7fc0000
  and   x16, x11, x13
  srli  x16, x16, 18  
  # level-2 page, entry addr:{pte.ppn,VPN2,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  hsv.d    x11, 0(x17)
  
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  addi	x2, x2, 88
  # fence 
  fence
  hfence.vvma
.endm

.macro MMU_SV39X4_2M VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, ATP, PTR1
  #backup regs 
  addi	x2, x2, -88
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
   
  # get *atp.ppn in x9
  li    x9, \ATP
  srli  x9, x9, 12
  
  # get VPN2 in x10
  li    x10, \VPN
  li    x11, 0x1ffc0000
  and   x10,x10,x11
  srli  x10, x10, 18    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN2,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page flag=FLAG1
  # level-2 base addr in x12
  li    x12, \PTR1
  srli  x12, x12, 12
  slli  x14, x12, 10
  li    x18, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x18, x18, 54
  or    x13, x13, x14
  or    x13, x13, x18
  sd    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN1 in x16
  li    x11, \VPN
  li    x13, 0x3fe00
  and   x16, x11, x13
  srli  x16, x16, 9  
  # level-2 page, entry addr:{pte.ppn,VPN1,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x17)
  
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  addi	x2, x2, 88
  # fence 
  fence
  hfence.gvma
.endm

.macro MMU_SV48X4_1G VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, ATP, PTR1
  #backup regs 
  addi	x2, x2, -88
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
   
  # get *atp.ppn in x9
  li    x9, \ATP
  srli  x9, x9, 12
  
  # get VPN3 in x10
  li    x10, \VPN
  li    x11, 0x3ff8000000
  and   x10,x10,x11
  srli  x10, x10, 27    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN3,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page flag=FLAG1
  # level-2 base addr in x12
  li    x12, \PTR1
  srli  x12, x12, 12
  slli  x14, x12, 10
  li    x18, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x18, x18, 54
  or    x13, x13, x14
  or    x13, x13, x18
  sd    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN2 in x16
  li    x11, \VPN
  li    x13, 0x7fc0000
  and   x16, x11, x13
  srli  x16, x16, 18  
  # level-2 page, entry addr:{pte.ppn,VPN2,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x17)
  
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  addi	x2, x2, 88
  # fence 
  fence
  hfence.gvma
.endm

.macro MMU_PTW_4K VPN, PPN, FLAG, PBMT
  #backup regs 
  addi	x2, x2, -88
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
   
  # get PPN from satp in x9
  csrr  x9, satp
  li    x10, 0xfffffffffff
  and   x9, x9, x10
  
  # get VPN2 in x10
  li    x10, \VPN
  li    x11, 0x7fc0000
  and   x10,x10,x11
  srli  x10, x10, 18    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN2,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  # level-2 base addr in x12
  addi  x12, x10, 1
  add   x12, x12, x9
  slli  x14, x12, 10
  li    x13, 0x01
  or    x13, x13, x14
  sd    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN1 in x16
  li    x11, \VPN
  li    x13, 0x3fe00
  and   x16, x11, x13
  srli  x16, x16, 9  
  # level-2 page, entry addr:{pte.ppn,VPN1,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  # level-3 base addr in x18: {satp.ppn + 10'b1000000010 + {vpn2,vpn1}, VPN0, 3'b0}
  li    x11, 0x200
  addi  x11, x11, 2
  add   x11, x11, x9  
  li    x10, \VPN
  srli  x10, x10, 9
  add   x18, x11, x10
  slli  x19, x18, 10 
  li    x13, 0x01
  or    x19, x19, x13
  sd    x19, 0(x17)
  
  # cfig level-3 page
  # get VPN0 in x12
  li    x11, \VPN
  li    x12, 0x1ff
  and   x12, x12, x11
  # get level-3 page addr in x17
  slli  x18, x18, 12
  slli  x12, x12, 3
  add   x17, x18, x12
  # write pte into level-3 page
  li    x11, \PPN
  li    x12, \PBMT
  li    x13, \FLAG
  slli  x11, x11, 10
  slli  x12, x12, 61
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x17)
   
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  addi	x2, x2, 88
  # fence 
  fence
  sfence.vma
.endm

.macro MMU_SV39_4K VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, FLAG2HEAD, FLAG2TAIL, ATP, PTR1, PTR2
  #backup regs 
  addi	x2, x2, -96
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
  sd	x20, 88(x2)
   
  # get PPN from satp in x9
  li    x9, \ATP
  srli   x9, x9, 12
  
  # get VPN2 in x10
  li    x10, \VPN
  li    x11, 0x7fc0000
  and   x10,x10,x11
  srli  x10, x10, 18    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN2,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  # level-2 base addr in x12
  li    x12, \PTR1
  srli   x12, x12, 12
  slli  x14, x12, 10
  li    x20, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x20, x20, 54
  or    x13, x13, x14
  or    x13, x13, x20
  sd    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN1 in x16
  li    x11, \VPN
  li    x13, 0x3fe00
  and   x16, x11, x13
  srli  x16, x16, 9  
  # level-2 page, entry addr:{pte.ppn,VPN1,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x18, \PTR2
  srli  x18, x18, 12
  slli  x19, x18, 10
  li    x20, \FLAG2HEAD
  li    x13, \FLAG2TAIL
  slli  x20, x20, 54
  or    x19, x19, x13
  or    x19, x19, x20
  sd    x19, 0(x17)
  
  # cfig level-3 page
  # get VPN0 in x12
  li    x11, \VPN
  li    x12, 0x1ff
  and   x12, x12, x11
  # get level-3 page addr in x17
  slli  x18, x18, 12
  slli  x12, x12, 3
  add   x17, x18, x12
  # write pte into level-3 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x17)
   
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  ld	x20, 88(x2)
  addi	x2, x2, 96
  # fence 
  fence
  sfence.vma
.endm

.macro MMU_SV39_4K_VS VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, FLAG2HEAD, FLAG2TAIL, ATP, PTR1, PTR2
  #backup regs 
  addi	x2, x2, -96
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
  sd	x20, 88(x2)
   
  # get PPN from satp in x9
  li    x9, \ATP
  srli   x9, x9, 12
  
  # get VPN2 in x10
  li    x10, \VPN
  li    x11, 0x7fc0000
  and   x10,x10,x11
  srli  x10, x10, 18    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN2,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  # level-2 base addr in x12
  li    x12, \PTR1
  srli   x12, x12, 12
  slli  x14, x12, 10
  li    x20, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x20, x20, 54
  or    x13, x13, x14
  or    x13, x13, x20
  hsv.d    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN1 in x16
  li    x11, \VPN
  li    x13, 0x3fe00
  and   x16, x11, x13
  srli  x16, x16, 9  
  # level-2 page, entry addr:{pte.ppn,VPN1,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x18, \PTR2
  srli  x18, x18, 12
  slli  x19, x18, 10
  li    x20, \FLAG2HEAD
  li    x13, \FLAG2TAIL
  slli  x20, x20, 54
  or    x19, x19, x13
  or    x19, x19, x20
  hsv.d    x19, 0(x17)
  
  # cfig level-3 page
  # get VPN0 in x12
  li    x11, \VPN
  li    x12, 0x1ff
  and   x12, x12, x11
  # get level-3 page addr in x17
  slli  x18, x18, 12
  slli  x12, x12, 3
  add   x17, x18, x12
  # write pte into level-3 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  hsv.d    x11, 0(x17)
   
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  ld	x20, 88(x2)
  addi	x2, x2, 96
  # fence 
  fence
  hfence.vvma
.endm

.macro MMU_SV48_2M VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, FLAG2HEAD, FLAG2TAIL, ATP, PTR1, PTR2
  #backup regs 
  addi	x2, x2, -96
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
  sd	x20, 88(x2)
   
  # get PPN from *atp in x9
  li    x9, \ATP
  srli   x9, x9, 12
  
  # get VPN3 in x10
  li    x10, \VPN
  li    x11, 0xff8000000
  and   x10,x10,x11
  srli  x10, x10, 27    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN3,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  # level-2 base addr in x12
  li    x12, \PTR1
  srli   x12, x12, 12
  slli  x14, x12, 10
  li    x20, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x20, x20, 54
  or    x13, x13, x14
  or    x13, x13, x20
  sd    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN2 in x16
  li    x11, \VPN
  li    x13, 0x7fc0000
  and   x16, x11, x13
  srli  x16, x16, 18  
  # level-2 page, entry addr:{pte.ppn,VPN2,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x18, \PTR2
  srli  x18, x18, 12
  slli  x19, x18, 10 
  li    x20, \FLAG2HEAD
  li    x13, \FLAG2TAIL
  slli  x20, x20, 54
  or    x19, x19, x13
  or    x19, x19, x20
  sd    x19, 0(x17)
  
  # cfig level-3 page
  # get VPN1 in x12
  li    x11, \VPN
  li    x12, 0x3fe00
  and   x12, x12, x11
  srli  x12, x12, 9
  # get level-3 page addr in x17
  slli  x18, x18, 12
  slli  x12, x12, 3
  add   x17, x18, x12
  # write pte into level-3 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x17)
   
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  ld	x20, 88(x2)
  addi	x2, x2, 96
  # fence 
  fence
  sfence.vma
.endm

.macro MMU_SV48_2M_VS VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, FLAG2HEAD, FLAG2TAIL, ATP, PTR1, PTR2
  #backup regs 
  addi	x2, x2, -96
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
  sd	x20, 88(x2)
   
  # get PPN from *atp in x9
  li    x9, \ATP
  srli   x9, x9, 12
  
  # get VPN3 in x10
  li    x10, \VPN
  li    x11, 0xff8000000
  and   x10,x10,x11
  srli  x10, x10, 27    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN3,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  # level-2 base addr in x12
  li    x12, \PTR1
  srli   x12, x12, 12
  slli  x14, x12, 10
  li    x20, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x20, x20, 54
  or    x13, x13, x14
  or    x13, x13, x20
  hsv.d    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN2 in x16
  li    x11, \VPN
  li    x13, 0x7fc0000
  and   x16, x11, x13
  srli  x16, x16, 18  
  # level-2 page, entry addr:{pte.ppn,VPN2,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x18, \PTR2
  srli  x18, x18, 12
  slli  x19, x18, 10 
  li    x20, \FLAG2HEAD
  li    x13, \FLAG2TAIL
  slli  x20, x20, 54
  or    x19, x19, x13
  or    x19, x19, x20
  hsv.d    x19, 0(x17)
  
  # cfig level-3 page
  # get VPN1 in x12
  li    x11, \VPN
  li    x12, 0x3fe00
  and   x12, x12, x11
  srli  x12, x12, 9
  # get level-3 page addr in x17
  slli  x18, x18, 12
  slli  x12, x12, 3
  add   x17, x18, x12
  # write pte into level-3 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  hsv.d    x11, 0(x17)
   
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  ld	x20, 88(x2)
  addi	x2, x2, 96
  # fence 
  fence
  hfence.vvma
.endm


.macro MMU_SV39X4_4K VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, FLAG2HEAD, FLAG2TAIL, ATP, PTR1, PTR2
  #backup regs 
  addi	x2, x2, -96
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
  sd	x20, 88(x2)
   
  # get PPN from satp in x9
  li    x9, \ATP
  srli   x9, x9, 12
  
  # get VPN2 in x10
  li    x10, \VPN
  li    x11, 0x1ffc0000
  and   x10,x10,x11
  srli  x10, x10, 18    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN2,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  # level-2 base addr in x12
  li    x12, \PTR1
  srli   x12, x12, 12
  slli  x14, x12, 10
  li    x20, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x20, x20, 54
  or    x13, x13, x14
  or    x13, x13, x20
  sd    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN1 in x16
  li    x11, \VPN
  li    x13, 0x3fe00
  and   x16, x11, x13
  srli  x16, x16, 9  
  # level-2 page, entry addr:{pte.ppn,VPN1,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x18, \PTR2
  srli  x18, x18, 12
  slli  x19, x18, 10
  li    x20, \FLAG2HEAD
  li    x13, \FLAG2TAIL
  slli  x20, x20, 54
  or    x19, x19, x13
  or    x19, x19, x20
  sd    x19, 0(x17)
  
  # cfig level-3 page
  # get VPN0 in x12
  li    x11, \VPN
  li    x12, 0x1ff
  and   x12, x12, x11
  # get level-3 page addr in x17
  slli  x18, x18, 12
  slli  x12, x12, 3
  add   x17, x18, x12
  # write pte into level-3 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x17)
   
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  ld	x20, 88(x2)
  addi	x2, x2, 96
  # fence 
  fence
  hfence.gvma
.endm

.macro MMU_SV48X4_2M VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, FLAG2HEAD, FLAG2TAIL, ATP, PTR1, PTR2
  #backup regs 
  addi	x2, x2, -96
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
  sd	x20, 88(x2)
   
  # get PPN from *atp in x9
  li    x9, \ATP
  srli   x9, x9, 12
  
  # get VPN3 in x10
  li    x10, \VPN
  li    x11, 0x3ff8000000
  and   x10,x10,x11
  srli  x10, x10, 27    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN3,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  # level-2 base addr in x12
  li    x12, \PTR1
  srli   x12, x12, 12
  slli  x14, x12, 10
  li    x20, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x20, x20, 54
  or    x13, x13, x14
  or    x13, x13, x20
  sd    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN2 in x16
  li    x11, \VPN
  li    x13, 0x7fc0000
  and   x16, x11, x13
  srli  x16, x16, 18  
  # level-2 page, entry addr:{pte.ppn,VPN2,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x18, \PTR2
  srli  x18, x18, 12
  slli  x19, x18, 10 
  li    x20, \FLAG2HEAD
  li    x13, \FLAG2TAIL
  slli  x20, x20, 54
  or    x19, x19, x13
  or    x19, x19, x20
  sd    x19, 0(x17)
  
  # cfig level-3 page
  # get VPN1 in x12
  li    x11, \VPN
  li    x12, 0x3fe00
  and   x12, x12, x11
  srli  x12, x12, 9
  # get level-3 page addr in x17
  slli  x18, x18, 12
  slli  x12, x12, 3
  add   x17, x18, x12
  # write pte into level-3 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x17)
   
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  ld	x20, 88(x2)
  addi	x2, x2, 96
  # fence 
  fence
  hfence.gvma
.endm




.macro MMU_SV48_4K VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, FLAG2HEAD, FLAG2TAIL, FLAG3HEAD, FLAG3TAIL, ATP, PTR1, PTR2, PTR3
  #backup regs 
  addi	x2, x2, -112
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
  sd    x20, 88(x2)
  sd    x21, 96(x2)
  sd    x22, 104(x2)
   
  # get PPN from *atp in x9
  li    x9, \ATP
  srli   x9, x9, 12
  
  # get VPN3 in x10
  li    x10, \VPN
  li    x11, 0xff8000000
  and   x10,x10,x11
  srli  x10, x10, 27    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN3,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  # level-2 base addr in x12
  li    x12, \PTR1
  srli   x12, x12, 12
  slli  x14, x12, 10
  li    x22, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x22, x22, 54
  or    x13, x13, x14
  or    x13, x13, x22
  sd    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN2 in x16
  li    x11, \VPN
  li    x13, 0x7fc0000
  and   x16, x11, x13
  srli  x16, x16, 18  
  # level-2 page, entry addr:{pte.ppn,VPN2,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x18, \PTR2
  srli  x18, x18, 12
  slli  x19, x18, 10
  li    x22, \FLAG2HEAD
  li    x13, \FLAG2TAIL
  slli  x22, x22, 54
  or    x19, x19, x13
  or    x19, x19, x22
  sd    x19, 0(x17)
 
  # cfig level-3 page
  # get VPN1 in x16
  li    x11, \VPN
  li    x13, 0x3fe00
  and   x16, x11, x13
  srli  x16, x16, 9  
  # level-2 page, entry addr:{pte.ppn,VPN1,3'b0} in x17
  slli  x13, x18, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x20, \PTR3
  srli  x20, x20, 12
  slli  x21, x20, 10 
  li    x22, \FLAG3HEAD
  li    x13, \FLAG3TAIL
  slli  x22, x22, 54
  or    x21, x21, x13
  or    x21, x21, x22
  sd    x21, 0(x17)


  # cfig level-3 page
  # get VPN1 in x12
  li    x11, \VPN
  li    x12, 0x1ff
  and   x12, x12, x11
  # get level-3 page addr in x17
  slli  x20, x20, 12
  slli  x12, x12, 3
  add   x17, x20, x12
  # write pte into level-3 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x17)
   
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  ld    x20, 88(x2)
  ld    x21, 96(x2)
  ld    x22, 104(x2)
  addi	x2, x2, 112
  # fence 
  fence
  sfence.vma
.endm

.macro MMU_SV48_4K_VS VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, FLAG2HEAD, FLAG2TAIL, FLAG3HEAD, FLAG3TAIL, ATP, PTR1, PTR2, PTR3
  #backup regs 
  addi	x2, x2, -112
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
  sd    x20, 88(x2)
  sd    x21, 96(x2)
  sd    x22, 104(x2)
   
  # get PPN from *atp in x9
  li    x9, \ATP
  srli   x9, x9, 12
  
  # get VPN3 in x10
  li    x10, \VPN
  li    x11, 0xff8000000
  and   x10,x10,x11
  srli  x10, x10, 27    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN3,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  # level-2 base addr in x12
  li    x12, \PTR1
  srli   x12, x12, 12
  slli  x14, x12, 10
  li    x22, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x22, x22, 54
  or    x13, x13, x14
  or    x13, x13, x22
  hsv.d    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN2 in x16
  li    x11, \VPN
  li    x13, 0x7fc0000
  and   x16, x11, x13
  srli  x16, x16, 18  
  # level-2 page, entry addr:{pte.ppn,VPN2,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x18, \PTR2
  srli  x18, x18, 12
  slli  x19, x18, 10
  li    x22, \FLAG2HEAD
  li    x13, \FLAG2TAIL
  slli  x22, x22, 54
  or    x19, x19, x13
  or    x19, x19, x22
  hsv.d    x19, 0(x17)
 
  # cfig level-3 page
  # get VPN1 in x16
  li    x11, \VPN
  li    x13, 0x3fe00
  and   x16, x11, x13
  srli  x16, x16, 9  
  # level-2 page, entry addr:{pte.ppn,VPN1,3'b0} in x17
  slli  x13, x18, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x20, \PTR3
  srli  x20, x20, 12
  slli  x21, x20, 10 
  li    x22, \FLAG3HEAD
  li    x13, \FLAG3TAIL
  slli  x22, x22, 54
  or    x21, x21, x13
  or    x21, x21, x22
  hsv.d    x21, 0(x17)


  # cfig level-3 page
  # get VPN1 in x12
  li    x11, \VPN
  li    x12, 0x1ff
  and   x12, x12, x11
  # get level-3 page addr in x17
  slli  x20, x20, 12
  slli  x12, x12, 3
  add   x17, x20, x12
  # write pte into level-3 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  hsv.d    x11, 0(x17)
   
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  ld    x20, 88(x2)
  ld    x21, 96(x2)
  ld    x22, 104(x2)
  addi	x2, x2, 112
  # fence 
  fence
  hfence.vvma
.endm

.macro MMU_SV48X4_4K VPN, PPN, FLAGHEAD, FLAGTAIL, FLAG1HEAD, FLAG1TAIL, FLAG2HEAD, FLAG2TAIL, FLAG3HEAD, FLAG3TAIL, ATP, PTR1, PTR2, PTR3
  #backup regs 
  addi	x2, x2, -112
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd	x11, 16(x2)
  sd	x12, 24(x2)
  sd	x13, 32(x2)
  sd	x14, 40(x2)
  sd	x15, 48(x2)
  sd	x16, 56(x2)
  sd	x17, 64(x2)
  sd	x18, 72(x2)
  sd	x19, 80(x2)
  sd    x20, 88(x2)
  sd    x21, 96(x2)
  sd    x22, 104(x2)
   
  # get PPN from *atp in x9
  li    x9, \ATP
  srli   x9, x9, 12
  
  # get VPN3 in x10
  li    x10, \VPN
  li    x11, 0x3ff8000000
  and   x10,x10,x11
  srli  x10, x10, 27    
   
  # cfig first-level page
  # level-1 page, entry addr:{ppn,VPN3,3'b0} in x15
  slli  x14, x9, 12
  slli  x11, x10, 3
  add   x15, x14, x11
  # write pte into level-1 page
  # level-2 base addr in x12
  li    x12, \PTR1
  srli   x12, x12, 12
  slli  x14, x12, 10
  li    x22, \FLAG1HEAD
  li    x13, \FLAG1TAIL
  slli  x22, x22, 54
  or    x13, x13, x14
  or    x13, x13, x22
  sd    x13, 0(x15)
  
  # cfig level-2 page
  # get VPN2 in x16
  li    x11, \VPN
  li    x13, 0x7fc0000
  and   x16, x11, x13
  srli  x16, x16, 18  
  # level-2 page, entry addr:{pte.ppn,VPN2,3'b0} in x17
  slli  x13, x12, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x18, \PTR2
  srli  x18, x18, 12
  slli  x19, x18, 10
  li    x22, \FLAG2HEAD
  li    x13, \FLAG2TAIL
  slli  x22, x22, 54
  or    x19, x19, x13
  or    x19, x19, x22
  sd    x19, 0(x17)
 
  # cfig level-3 page
  # get VPN1 in x16
  li    x11, \VPN
  li    x13, 0x3fe00
  and   x16, x11, x13
  srli  x16, x16, 9  
  # level-2 page, entry addr:{pte.ppn,VPN1,3'b0} in x17
  slli  x13, x18, 12
  slli  x17, x16, 3
  add   x17, x17, x13
  # write pte into level-2 page
  li    x20, \PTR3
  srli  x20, x20, 12
  slli  x21, x20, 10 
  li    x22, \FLAG3HEAD
  li    x13, \FLAG3TAIL
  slli  x22, x22, 54
  or    x21, x21, x13
  or    x21, x21, x22
  sd    x21, 0(x17)


  # cfig level-3 page
  # get VPN1 in x12
  li    x11, \VPN
  li    x12, 0x1ff
  and   x12, x12, x11
  # get level-3 page addr in x17
  slli  x20, x20, 12
  slli  x12, x12, 3
  add   x17, x20, x12
  # write pte into level-3 page
  li    x11, \PPN
  li    x12, \FLAGHEAD
  li    x13, \FLAGTAIL
  slli  x11, x11, 10
  slli  x12, x12, 54
  or    x11, x11, x12
  or    x11, x11, x13
  sd    x11, 0(x17)
   
  #restore regs
  ld	x9, 0(x2)
  ld	x10, 8(x2)
  ld	x11, 16(x2)
  ld	x12, 24(x2)
  ld	x13, 32(x2)
  ld	x14, 40(x2)
  ld	x15, 48(x2)
  ld	x16, 56(x2)
  ld	x17, 64(x2)
  ld	x18, 72(x2)
  ld	x19, 80(x2)
  ld    x20, 88(x2)
  ld    x21, 96(x2)
  ld    x22, 104(x2)
  addi	x2, x2, 112
  # fence 
  fence
  hfence.gvma
.endm

.macro MXSTATUS_THEADISAEE IMM
  #write cskyisaee
  li    x9,0x400000
  #csrc  mxstatus,x9
  csrc  0x7c0,x9
  li    x10, \IMM
  slli  x10,x10,22
  csrs  0x7c0,x10
.endm

.macro MMU_SATP_CFG MODE, ASID, PPN
  #backup regs
  addi	x2, x2, -24
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd    x11, 16(x2)

  li    x9,  \MODE
  slli  x9,x9,60
  li    x10, \ASID
  slli  x10, x10, 44
  li    x11, \PPN
  srli  x11, x11, 12

  or    x11,x11,x10
  or    x11,x11,x9
  csrw	satp, x11

  #restore regs
  ld    x11, 16(x2)
  ld	x10, 8(x2)
  ld	x9, 0(x2)
  addi	x2, x2, 24
.endm

.macro MMU_VSATP_CFG MODE, ASID, PPN
  #backup regs
  addi	x2, x2, -24
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd    x11, 16(x2)

  li    x9,  \MODE
  slli  x9,x9,60
  li    x10, \ASID
  slli  x10, x10, 44
  li    x11, \PPN
  srli  x11, x11, 12

  or    x11,x11,x10
  or    x11,x11,x9
  csrw	vsatp, x11

  #restore regs
  ld    x11, 16(x2)
  ld	x10, 8(x2)
  ld	x9, 0(x2)
  addi	x2, x2, 24
.endm

# note vmid width 14 bit
.macro MMU_HGATP_CFG MODE, VMID, PPN
  #backup regs
  addi	x2, x2, -24
  sd	x9, 0(x2)
  sd	x10, 8(x2)
  sd    x11, 16(x2)

  li    x9,  \MODE
  slli  x9,x9,60
  li    x10, \VMID
  slli  x10, x10, 44
  li    x11, \PPN
  srli  x11, x11, 12

  or    x11,x11,x10
  or    x11,x11,x9
  csrw	hgatp, x11

  #restore regs
  ld    x11, 16(x2)
  ld	x10, 8(x2)
  ld	x9, 0(x2)
  addi	x2, x2, 24
.endm

.macro MMU_SATP_MODE MODE
  #backup regs
  addi	x2, x2, -16
  sd	x9, 0(x2)
  sd	x10, 8(x2)

  csrr  x9,satp
  li    x10,0xfffffffffffffff
  and   x9,x9,x10
  li    x10,\MODE
  slli  x10,x10,60
  or    x9,x9,x10
  csrw	satp, x9

  #restore regs
  ld	x10, 8(x2)
  ld	x9, 0(x2)
  addi	x2, x2, 16
  sfence.vma
.endm

.macro MMU_EN
  #backup regs
  addi	x2, x2, -16
  sd	x9, 0(x2)
  sd	x10, 8(x2)

  csrr  x9,satp
  li    x10,0xfffffffffffffff
  and   x9,x9,x10
  li    x10, 8
  slli  x10,x10,60
  or    x9,x9,x10
  csrw	satp, x9

  #restore regs
  ld	x10, 8(x2)
  ld	x9, 0(x2)
  addi	x2, x2, 16
  sfence.vma
.endm

.macro MMU_SATP_PPN PPN
  #backup regs
  addi	x2, x2, -16
  sd	x9,  0(x2)
  sd	x10, 8(x2)

  #write PPN
  csrr  x9, satp
  li	x10, 0xfffffffff0000000
  and   x9,x9,x10
  li    x10, \PPN
  or    x9,x9,x10
  csrw	satp,x9

  #restore regs
  ld	x10, 8(x2)
  ld	x9,  0(x2)
  addi	x2, x2, 8
  sfence.vma
.endm

.macro MMU_SATP_ASID ASID
  #backup regs
  addi	x2, x2, -16
  sd	x9,  0(x2)
  sd	x10, 8(x2)

  #write ASID
  csrr  x9, satp
  li	x10, 0xf0000fffffffffff
  and   x9,x9,x10
  li    x10, \ASID
  slli  x10,x10,44

  or    x9,x9,x10
  csrw	satp,x9

  #restore regs
  ld	x10, 8(x2)
  ld	x9,  0(x2)
  addi	x2, x2, 8
  sfence.vma
.endm

.macro MMU_VSATP_MODE MODE
  #backup regs
  addi	x2, x2, -16
  sd	x9, 0(x2)
  sd	x10, 8(x2)

  csrr  x9,vsatp
  li    x10,0xfffffffffffffff
  and   x9,x9,x10
  li    x10,\MODE
  slli  x10,x10,60
  or    x9,x9,x10
  csrw	vsatp, x9

  #restore regs
  ld	x10, 8(x2)
  ld	x9, 0(x2)
  addi	x2, x2, 16
  hfence.vvma
.endm

.macro MMU_VSATP_PPN PPN
  #backup regs
  addi	x2, x2, -16
  sd	x9,  0(x2)
  sd	x10, 8(x2)

  #write PPN
  csrr  x9, vsatp
  li	x10, 0xfffffffff0000000
  and   x9,x9,x10
  li    x10, \PPN
  srli  x10, x10, 12
  or    x9,x9,x10
  csrw	vsatp,x9

  #restore regs
  ld	x10, 8(x2)
  ld	x9,  0(x2)
  addi	x2, x2, 8
  hfence.vvma
.endm

.macro MMU_VSATP_ASID ASID
  #backup regs
  addi	x2, x2, -16
  sd	x9,  0(x2)
  sd	x10, 8(x2)

  #write ASID
  csrr  x9, vsatp
  li	x10, 0xf0000fffffffffff
  and   x9,x9,x10
  li    x10, \ASID
  slli  x10,x10,44

  or    x9,x9,x10
  csrw	vsatp,x9

  #restore regs
  ld	x10, 8(x2)
  ld	x9,  0(x2)
  addi	x2, x2, 8
  hfence.vvma
.endm


.macro MMU_HGATP_PPN PPN
  #backup regs
  addi	x2, x2, -16
  sd	x9,  0(x2)
  sd	x10, 8(x2)

  #write PPN
  csrr  x9, hgatp
  li	x10, 0xfffff00000000000
  and   x9,x9,x10
  li    x10, \PPN
  srli  x10, x10, 12
  or    x9,x9,x10
  csrw	hgatp,x9

  #restore regs
  ld	x10, 8(x2)
  ld	x9,  0(x2)
  addi	x2, x2, 8
  hfence.gvma
.endm



.macro MMU_HGATP_VMID VMID
  #backup regs
  addi	x2, x2, -16
  sd	x9,  0(x2)
  sd	x10, 8(x2)

  #write VMID
  csrr  x9, hgatp
  li	x10, 0xfc000fffffffffff
  and   x9,x9,x10
  li    x10, \VMID
  slli  x10,x10,44

  or    x9,x9,x10
  csrw	hgatp,x9

  #restore regs
  ld	x10, 8(x2)
  ld	x9,  0(x2)
  addi	x2, x2, 8
  hfence.gvma
.endm

.macro MMU_HGATP_MODE MODE
  #backup regs
  addi	x2, x2, -16
  sd	x9,  0(x2)
  sd	x10, 8(x2)

  #write MODE
  csrr  x9, hgatp
  li	x10, 0xfffffffffffffff
  and   x9,x9,x10
  li    x10, \MODE
  slli  x10,x10,60

  or    x9,x9,x10
  csrw	hgatp,x9

  #restore regs
  ld	x10, 8(x2)
  ld	x9,  0(x2)
  addi	x2, x2, 8
  hfence.gvma
.endm




.macro MMODETOSMODE  SROUTINE
  #disable sie/mie
  li   x29, 0x2
  csrc mstatus,x29
  li   x29, 0x8
  csrc mstatus,x29
  #write mepc
  la x1,\SROUTINE
  csrw mepc,x1
  li x1,0x800
  #csrrs x3,mstatus,x1
  csrs mstatus,x1
  li x1,0x1000
  #csrrc x3,mstatus,x1
  csrc mstatus,x1
  mret
.endm

.macro MMODETOVSMODE  VSROUTINE
  #disable sie/mie
  li   x29, 0x2
  csrc mstatus,x29
  li   x29, 0x8
  csrc mstatus,x29
  #write mepc
  la x1,\VSROUTINE
  csrw mepc,x1
  li x1,0x8000000800
  #csrrs x3,mstatus,x1
  csrs mstatus,x1
  li x1,0x1000
  #csrrc x3,mstatus,x1
  csrc mstatus,x1
  mret
.endm



.macro MMODETOUMODE  UROUTINE
  #disable sie/mie
  li   x29, 0x2
  csrc mstatus,x29
  li   x29, 0x8
  csrc mstatus,x29
  #write mepc
  la x1,\UROUTINE
  csrw mepc,x1
  li x1,0x1800
  #csrrc x3,mstatus,x1
  csrc mstatus,x1
  mret
.endm

.macro MMODETOVUMODE  VUROUTINE
  #disable sie/mie
  li   x29, 0x2
  csrc mstatus,x29
  li   x29, 0x8
  csrc mstatus,x29
  #write mepc
  la x1,\VUROUTINE
  csrw mepc,x1
  li x1,0x8000000800
  csrs mstatus,x1
  li x1,0x1800
  #csrrc x3,mstatus,x1
  csrc mstatus,x1
  mret
.endm


.macro HSMODETOVSMODE  VSROUTINE
  #disable sie/mie
  li   x29, 0x2
  csrc sstatus,x29
  li   x29, 0x8
  csrc sstatus,x29
  #write mepc
  la x1,\VSROUTINE
  csrw sepc,x1
  li x1,0x0000000100
  csrs sstatus,x1
  #li x1,0x1000
  #csrrc x3,mstatus,x1
  #csrc sstatus,x1
  li x1,0x0080
  csrs hstatus,x1
  sret
.endm


.macro HSMODETOVUMODE  VUROUTINE
  #disable sie/mie
  li   x29, 0x2
  csrc sstatus,x29
  li   x29, 0x8
  csrc sstatus,x29
  #write mepc
  la x1,\VUROUTINE
  csrw sepc,x1
  li x1,0x0000000100
  csrc sstatus,x1
  #li x1,0x1000
  #csrrc x3,mstatus,x1
  #csrc sstatus,x1
  li x1,0x0080
  csrs hstatus,x1
  sret
.endm

.macro VSMODETOVUMODE  VUROUTINE
  #disable sie/mie
  li   x29, 0x2
  csrc sstatus,x29
  li   x29, 0x8
  csrc sstatus,x29
  #write mepc
  la x1,\VUROUTINE
  csrw sepc,x1
  li x1,0x0000000100
  csrc sstatus,x1
  #li x1,0x1000
  #csrrc x3,mstatus,x1
  #csrc sstatus,x1
#li x1,0x0080
#csrs hstatus,x1
  sret
.endm


.macro MXSTATUS_MAEE IMM
  #write maee
  li    x9,0x200000
  #csrc  mxstatus,x9
  csrc  0x7c0,x9
  li    x10, \IMM
  slli  x10,x10,21
  csrs  0x7c0,x10
.endm

.macro SMODETOUMODE  UROUTINE
  #disable sie/mie
  li   x29, 0x2
  csrc sstatus,x29
  li   x29, 0x8
  csrc sstatus,x29
  #write mepc
  la x1,\UROUTINE
  csrw sepc,x1
  li x1,0x0000000100
  csrc sstatus,x1
  #li x1,0x1000
  #csrrc x3,mstatus,x1
  #csrc sstatus,x1
  #li x1,0x0080
  #csrs hstatus,x1
  sret
.endm

.macro EN_HYPERVISOR
  # ensure misa.H is on
  li    x9,  0x80
  csrs  misa,  x9
.endm

.macro PMP_NO_LIMIT 
  # cfig PMP
  # entry0: addr 0x0_0000_0000~0xff_ffff_ffff, r=1 w=1 x=1 l=0
  # cfig pmpcfg0
  li   x1, 0x1f 
  csrw pmpcfg0, x1
  # cfig pmpaddr0
  li   x1, 0x1fffffffff
  csrw pmpaddr0, x1
  sfence.vma
.endm


.macro EN_PBMT
  # set menvcfg.pbmte = 1
  li    x9,  0x4000000000000000
  csrs  menvcfg,  x9
.endm

.macro EN_HYPER_PBMT
  # set henvcfg.pbmte = 1
  li    x9,  0x4000000000000000
  csrs  henvcfg,  x9
.endm



