/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : readme.txt
* Author             : MCD Application Team
* Date First Issued  : 05/21/2007
* Description        : This sub-directory contains all the user-modifiable files 
*                      needed to create a new project linked with the STM32F10x  
*                      Firmware Library and working with RealView Microcontroller
*                      Development Kit(RVMDK) software toolchain (Version 3.10 and later).
********************************************************************************
* History:
* 05/21/2007: V0.3
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

Directory contents
===================
- Project.Uv2/.Opt: A pre-configured project file with the provided library structure
                    that produces a debug\release executable image with RVMDK.
              
- stm32f10x_vector.s: This file contains the vector table for STM32F10x, Stack
                       and Heap definition.
                       
- cortexm3_macro.s: Instruction wrappers for special Cortex-M3 instructions. 

      
How to use it
=============
- Open the Project.Uv2 project
- In the build toolbar select the project config:
   - BOOT_FLASH: to load project image into FLASH
   - BOOT_RAM: to load project image into RAM
- Rebuild all files: Project->Rebuild all target files
- Load project image: Debug->Start/Stop Debug Session
- Run program: Debug->Run (F5)
    

******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE******
