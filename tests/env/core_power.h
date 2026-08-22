
# MMODE_ENTER_LOW_POWER_STATE
# Macro used to let core enter into low power state

# Parameter: "L2_FLUSH" is used to control whether to 
# flush L2 if it's the last acitve core of the cpu 
# cluster. Value 1 means L2 flush is needed.

.macro MMODE_ENTER_LOW_POWER_STATE  L2_FLUSH
  # disable interrupts
  li x3, 0xa
  csrc mstatus, x3
  li x3, 0
  csrw mie, x3

  # disable prefetcher
  lui x3, 0x70
  csrc 0x7f0, x3   #ML2SETUP.iprf/tprf
  li x3, 0x20
  csrc 0x7c0, x3   #MSETUP.prfe
  li x3, 0x193
  csrs 0x7d1, x3   #

  # clean & inv dcache
  li x3, 0x3
  csrw 0x7c2, x3

  fence

  # disable dcache
  li x3, 0x1
  csrc 0x7c0, x3

  # disable smpen
  csrr x3, mhartid
  andi x3, x3, 0x3
  li   x4, 0x1
  sll  x3, x4, x3
  csrc 0x7f0, x3

  fence

  # check whether L2 needs power down
  li   x3, \L2_FLUSH
  beq  x3, x0, 2f

  # flush L2 only if it's the last active core in the cluster
  li x3, MB_CORE_PAD_LPMD
  ld x4, 0(x3)
  li x3, 0x1
  beq x4, x3, 1f 
  li x3, 0x2
  beq x4, x3, 1f 
  li x3, 0x4
  beq x4, x3, 1f 
  li x3, 0x8
  beq x4, x3, 1f 
  j  2f
1:
  csrr x3, mhartid
  srli x3, x3, 2
  li   x4, 0x1
  sll  x4, x4, x3
  li x3, MB_PAD_CPU_L2_REQ
  sd x4, 0(x3)

  fence

2:
  # enter low power state
  wfi

.endm


