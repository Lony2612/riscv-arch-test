// ******************************************************************************
// * Copyright (C) 2024 SpacemiT. All rights reserved.                          *
// * -------------------------------                                            *
// * This file and all its contents are properties of SpacemiT (Hangzhou)       *
// * Technology Co. Ltd. ("SpacemiT"). The information contained herein is      *
// * confidential and proprietary. Unauthorized use, copying, modification,     *
// * and distribution is strictly forbidden without prior written consent       *
// * from SpacemiT.                                                             *
// ******************************************************************************
// FILE NAME       : c_mmu.h
// AUTHOR          : zhangzhuo
// ORIGINAL DATE   : 2024-12-31
// DESCRIPTION     :
// ******************************************************************************
#ifndef __C_MMU__H__
#define __C_MMU__H__

#include "stdint.h"

const uint8_t   PTE_V = 0b00000001;
const uint8_t   PTE_R = 0b00000010;
const uint8_t   PTE_W = 0b00000100;
const uint8_t   PTE_X = 0b00001000;
const uint8_t   PTE_U = 0b00010000;
const uint8_t   PTE_G = 0b00100000;
const uint8_t   PTE_A = 0b01000000;
const uint8_t   PTE_D = 0b10000000;

const uint8_t   PTE_XWR = PTE_X | PTE_W | PTE_R;
const uint8_t   PTE_AD  = PTE_A | PTE_D;

const uint64_t  SATP_PPN_BITS   = 44;
const uint64_t  SATP_PPN_MASK   = (1ul << SATP_PPN_BITS) - 1;
const uint64_t  SATP_ASID_BITS  = 16;
const uint64_t  SATP_ASID_MASK  = (1ul << SATP_ASID_BITS) - 1;
const uint64_t  SATP_MODE_BITS  = 4;
const uint64_t  SATP_MODE_MASK  = (1ul << SATP_MODE_BITS) - 1;
const uint64_t  PARTIAL_PN_BITS = 9;
const uint64_t  PARTIAL_PN_MASK = (1ul << PARTIAL_PN_BITS) - 1;

const uint64_t  SV39_NEG_MASK = 0xFFFFFFC000000000ul;

typedef enum {
  BARE  = 0,
  SV39  = 8,
  SV48  = 9
} SatpMode;

typedef enum {
  PAGESIZE_4K = 12,
  PAGESIZE_2M = 21,
  PAGESIZE_1G = 30,
  PAGESIZE_512G = 39
} PageSize;

void fenceAndSfence() {
  asm volatile (
    "fence\n"
    "sfence.vma"
  );
}

uint64_t readSatp() {
  uint64_t  value;
  asm volatile (
    "csrr %0, satp"
    : "=r" (value)
  );
  return value;
}

void writeSatp(uint64_t value) {
  asm volatile (
    "csrw satp, %0"
    :: "r"(value)
  );
}

void setSatp(SatpMode mode, uint16_t asid, uint64_t ppn) {
  uint64_t value = 0;
  value += mode;
  value <<= 16;
  value += asid;
  value <<= 44;
  value += (ppn & SATP_PPN_MASK);
  writeSatp(value);
}

// FLAG: G, U, X, W, R, V
void setPage(SatpMode mode, uint64_t va, uint64_t pa, uint8_t flag, PageSize size, uint64_t pageTablePtrs[]) {
  // mask out pa
  pa >>= 12;
  switch (size) {
    case PAGESIZE_4K: break;
    case PAGESIZE_2M: pa = ((pa >> PARTIAL_PN_BITS) << PARTIAL_PN_BITS); break;
    case PAGESIZE_1G: pa = ((pa >> 2*PARTIAL_PN_BITS) << 2*PARTIAL_PN_BITS); break;
    case PAGESIZE_512G: pa = ((pa >> 3*PARTIAL_PN_BITS) << 3*PARTIAL_PN_BITS); break;
  }
  // break down VPN
  uint64_t  vpn[4];
  va >>= 12;
  for(int i = 0; i < 4; i++) {
    vpn[i] = va & PARTIAL_PN_MASK;
    va >>= PARTIAL_PN_BITS;
  }
  // make iter num
  int iterNum = 0;
  int vpnLevel = 0;
  switch(mode) {
    case SV39:
      switch(size) {
        case PAGESIZE_4K: iterNum = 3; break;
        case PAGESIZE_2M: iterNum = 2; break;
        case PAGESIZE_1G: iterNum = 1; break;
      }
      vpnLevel = 2;
      break;
    case SV48:
      switch(size) {
        case PAGESIZE_4K:  iterNum = 4; break;
        case PAGESIZE_2M:  iterNum = 3; break;
        case PAGESIZE_1G:  iterNum = 2; break;
        case PAGESIZE_512G:iterNum = 1; break;
      }
      vpnLevel = 2;
      break;
  }
  // make ptes
  uint64_t* pageTablePtr;
  uint64_t  pte;
  for(int i = 0; i < iterNum; i++) {
    uint64_t  pte = 0;
    pageTablePtr = (uint64_t *) pageTablePtrs[i];
    // last, make leaf page
    if(i == iterNum-1) {
      pte = ((pa & SATP_PPN_MASK) << 10) | (flag & 0x3F) | PTE_AD | PTE_V;
    }
    // not last
    else {
      uint64_t  next_pt = pageTablePtrs[i + 1];
      pte = ((next_pt >> 12) << 10) | PTE_V | (flag & 0x20);
    }
    pageTablePtr[vpn[vpnLevel]] = pte;
    vpnLevel --;
  }
  fenceAndSfence();
}

#endif // __C_MMU__H__