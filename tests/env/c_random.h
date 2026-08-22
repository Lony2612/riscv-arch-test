// ******************************************************************************
// * Copyright (C) 2024 SpacemiT. All rights reserved.                          *
// * -------------------------------                                            *
// * This file and all its contents are properties of SpacemiT (Hangzhou)       *
// * Technology Co. Ltd. ("SpacemiT"). The information contained herein is      *
// * confidential and proprietary. Unauthorized use, copying, modification,     *
// * and distribution is strictly forbidden without prior written consent       *
// * from SpacemiT.                                                             *
// ******************************************************************************
// FILE NAME       : c_random.h
// AUTHOR          : zhangzhuo
// ORIGINAL DATE   : 2024-12-27
// DESCRIPTION     :  provides utils for randomization
// ******************************************************************************
#ifndef __C_RANDOM__H__
#define __C_RANDOM__H__

#include "stdint.h"
#include "c_priv.h"

const uint64_t  MB_RANDOM_ADDR=0x9ff012b0;

typedef   volatile uint32_t * reg_ptr_t;

// get 32b random value from magic box
uint32_t urandom32() {
  reg_ptr_t mb_ptr = (reg_ptr_t)MB_RANDOM_ADDR;
  uint32_t  res;
  res = *mb_ptr;
  asm volatile("fence");
  return res;
}

// get random value between min_val(inclusive) -- max_val(exclusive)
uint32_t urandomRange(uint32_t min_val, uint32_t max_val) {
  uint32_t res;
  uint64_t delta = max_val - min_val;
  if(max_val <= min_val) {
    res = max_val;
  }
  else {
    res = ((urandom32() * delta) >> 32) + min_val;
  }
  return res;
}

// get random value from array
uint64_t randomChoice(uint64_t value[], uint32_t size) {
  uint32_t idx = urandomRange(0, size);
  return value[size];
}

// get random value from weighted array
uint64_t randomWeightedChoice(uint64_t value[], uint32_t weight[], uint32_t size) {
  uint32_t  sum = 0;
  uint32_t  rand_val = 0;
  // get sum and random choice
  for(int i = 0; i < size; i++) {
    sum += weight[i];
  }
  rand_val  = urandomRange(0, sum);

  // calc sum and test rand_val
  sum = 0;
  for(int i = 0; i < size; i++) {
    sum += weight[i];
    if(rand_val < sum)
      return value[i];
  }
  return value[size-1];
}

// returns random priv mode
PrivMode genRandomPrivMode() {
  PrivMode modes[3] = {PRIV_MODE_U, PRIV_MODE_S, PRIV_MODE_M};
  uint32_t idx = urandomRange(0, 3);
  return modes[idx];
}

#endif // __C_RANDOM__H__