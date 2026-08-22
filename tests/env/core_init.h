/*****************************************************************************
* Space-T Co., Ltd. Confidential                                             *
* -------------------------------                                            *
* This file and all its contents are properties of Space-T Co., Ltd.         *
* The information contained herein is confidential and proprietary           *
* and is not to be disclosed outside of Space-T Co., Ltd. except under       *
* a Non-Disclosure Agreement (NDA).                                          *
*****************************************************************************/
/*.include "core_hwcfig.h"*/
.include "core_lsu.s"
.include "core_cp0.s"
.include "core_vector.h"
.include "core_mmu.h"
.include "core_fpu.h"
.include "core_aia.h"
.include "int_drv.h"
.include "magic_box.h"
.include "core_power.h"
.include "core_reri.s"
.include "core_pma.h"
.include "core_zfa.h"



.macro EXIT
  la   x1, __exit
  jr   x1
.endm

.macro FAIL
  la   x1, __fail 
  jr   x1
.endm  

