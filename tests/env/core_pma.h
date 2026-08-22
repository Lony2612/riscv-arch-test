###########################################################
#            CSR DEFINITION                               #
###########################################################

.set pmacfg0   ,0x7de
.set pmacfg2   ,0x7df
.set pmaaddr0  ,0x7e0
.set pmaaddr1  ,0x7e1
.set pmaaddr2  ,0x7e2
.set pmaaddr3  ,0x7e3
.set pmaaddr4  ,0x7e4
.set pmaaddr5  ,0x7e5
.set pmaaddr6  ,0x7e6
.set pmaaddr7  ,0x7e7
.set pmaaddr8  ,0x7e8
.set pmaaddr9  ,0x7e9
.set pmaaddr10 ,0x7ea
.set pmaaddr11 ,0x7eb
.set pmaaddr12 ,0x7ec
.set pmaaddr13 ,0x7ed
.set pmaaddr14 ,0x7ee
.set pmaaddr15 ,0x7ef



###########################################################
#            MACRO DEFINITION                             #
###########################################################

# calc napot pmpaddr/pmaaddr range = [start, start+size)
# example:
#     CALC_NAPOT_SIZE x10 0x80000000 0x20000000
#     CALC_NAPOT_SIZE x10 0x80000000 (0x9fffffff-0x80000000+1)
.macro CALC_NAPOT_SIZE RD START SIZE
      li    \RD, (\START + ((\SIZE - 1) >> 1)) >> 2
.endm

# random a pma space
# size max 4g
.macro RAND_PMA_SPACE
      # rand pma
      .set RAND_PMA_CA_SIZE       , 1 << ((SEED % 20 + 20) % 20 + 12)
      .set RAND_PMA_CA_ADDR_START , (0x100000000 + (SEED & 0xffffffff)) >> ((SEED % 20 + 20) % 20 + 12) << ((SEED % 20 + 20) % 20 + 12)
      .set RAND_PMA_NC_SIZE       , 1 << (((SEED >> 5) % 20 + 20) % 20 + 12)
      .set RAND_PMA_NC_ADDR_START , (0x200000000 + (SEED & 0xffffffff)) >> (((SEED >> 5) % 20 + 20) % 20 + 12) << (((SEED >> 5) % 20 + 20) % 20 + 12) 
      .set RAND_PMA_IO_SIZE       , 1 << (((SEED >> 10) % 20 + 20) % 20 + 12)
      .set RAND_PMA_IO_ADDR_START , (0x300000000 + (SEED & 0xffffffff)) >> (((SEED >> 10) % 20 + 20) % 20 + 12) << (((SEED >> 10) % 20 + 20) % 20 + 12)
      # set pma 
      CALC_NAPOT_SIZE x1, RAND_PMA_CA_ADDR_START, RAND_PMA_CA_SIZE
      csrw  pmaaddr0, x1
      CALC_NAPOT_SIZE x1, RAND_PMA_NC_ADDR_START, RAND_PMA_NC_SIZE
      csrw  pmaaddr1, x1
      CALC_NAPOT_SIZE x1, RAND_PMA_IO_ADDR_START, RAND_PMA_IO_SIZE
      csrw  pmaaddr2, x1
      li    x1, 0xffffff
      csrc  pmacfg0, x1
      li    x1, 0x626160
      csrs  pmacfg0, x1
      sfence.vma x0, x0
.endm
