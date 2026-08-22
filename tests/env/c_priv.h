// ******************************************************************************
// * Copyright (C) 2024 SpacemiT. All rights reserved.                          *
// * -------------------------------                                            *
// * This file and all its contents are properties of SpacemiT (Hangzhou)       *
// * Technology Co. Ltd. ("SpacemiT"). The information contained herein is      *
// * confidential and proprietary. Unauthorized use, copying, modification,     *
// * and distribution is strictly forbidden without prior written consent       *
// * from SpacemiT.                                                             *
// ******************************************************************************
// FILE NAME       : c_priv.h
// AUTHOR          : zhangzhuo
// ORIGINAL DATE   : 2024-12-31
// DESCRIPTION     : provide priviledge utils for c cases
// ******************************************************************************
#ifndef __C_PRIV__H__
#define __C_PRIV__H__

#include "stdint.h"

typedef enum {
  PRIV_MODE_U = 0b00,
  PRIV_MODE_S = 0b01,
  PRIV_MODE_M = 0b11
} PrivMode;

void mModeToSMode() {
  uint64_t  pc;
  // get cur pc
  asm volatile(
    "auipc %0, 0"
    : "=r" (pc)
  );
  pc += 26;
  // set mepc and mpriv
  asm volatile(
    "csrc mstatus, %0\n"  // clear mie/sie
    "csrw mepc, %1\n" // write mepc
    "csrc mstatus, %2\n"  // clear higher bit of mpp
    "mret\n"
    "nop" // mret will ret to this nop
    :: "r" (0xa), "r"(pc), "r"(0x1000)
  );
  return;
}

void mModeToUMode() {
  uint64_t  pc;
  // get cur pc
  asm volatile(
    "auipc %0, 0"
    : "=r" (pc)
  );
  pc += 30;
  // set mepc and mpriv
  asm volatile(
    "csrc mstatus, %0\n"  // clear mie/sie
    "csrw mepc, %1\n" // write mepc
    "csrc mstatus, %2\n"  // clear both bits of mpp
    "mret\n"
    "nop" // mret will ret to this nop
    :: "r" (0xa), "r"(pc), "r"(0x1800)
  );
  return;
}

/*
uint64_t readCsr(uint32_t csrIndex) {
  uint64_t  value;
  asm volatile (
    "csrr %0, %1"
    : "=r" (value)
    : "r" (csrIndex)
  );
  return value;
}

void writeCsr(uint32_t csrIndex, uint64_t value) {
  asm volatile (
    "csrw %0, %1"
    :: "r" (csrIndex), "r"(value)
  );
}
*/

#endif // __C_PRIV__H__