/* Copyright 2018 Google LLC
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http:*www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
MEMORY
{
MEM1(RWX)  : ORIGIN = 0x00000000,  LENGTH = 0x00200000
MEM2(RWX)  : ORIGIN = 0x00200000,  LENGTH = 0x00200000
}
__stack_top = 0x000ee000 ;

ENTRY(rvtest_entry_point)

SECTIONS
{
  .text :
  {
    *(.text*)
  } >MEM1

  .rodata :  {*(.rodata* )
              *(.srodata)
              *(.srodata.*)
              *(.srodata.cst4*)
    *(.srodata.cst8*)
  } >MEM1

  .data : 
  { 
    *(.data) 
    *(.sdata*)
  } >MEM2

  .bss :
  {
    *(.bss) *.(COMMON) *(.sbss)
  } >MEM2
  end = .;
}
