// ******************************************************************************
// * Copyright (C) 2024 SpacemiT. All rights reserved.                          *
// * -------------------------------                                            *
// * This file and all its contents are properties of SpacemiT (Hangzhou)       *
// * Technology Co. Ltd. ("SpacemiT"). The information contained herein is      *
// * confidential and proprietary. Unauthorized use, copying, modification,     *
// * and distribution is strictly forbidden without prior written consent       *
// * from SpacemiT.                                                             *
// ******************************************************************************
// FILE NAME       : c_common.h
// AUTHOR          : zhangzhuo
// ORIGINAL DATE   : 2025-01-05
// DESCRIPTION     : defines common paramaters and functions for c test
// ******************************************************************************
#ifndef __C_COMMON__H__
#define __C_COMMON__H__

const int DWORD_SIZE  = 8;
const int LINE_SIZE   = 64;
const int PAGE_SIZE   = 4096;

const int DWORD_PER_LINE  = LINE_SIZE/DWORD_SIZE;
const int DWORD_PER_PAGE  = PAGE_SIZE/DWORD_SIZE;
const int LINE_PER_PAGE   = PAGE_SIZE/LINE_SIZE;

#endif // __C_COMMON__H__