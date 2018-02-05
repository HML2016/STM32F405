/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : stm32f10x_Timebase.c
* Author             : Jian-guo JIANG
* Date First Issued  : 2008.5.14
* Description        : This module handles time base. 
********************************************************************************
* History:
* 2008.5.14: V1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"

/* Include of other module interface headers ---------------------------------*/
/* Local includes ------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x_Timebase.h"
#include "stm32f10x_encoder.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile u16 hTimebase_display_500us = 0;
static u16 hSpeedMeas_Timebase_500us = SPEED_SAMPLING_TIME;

/*******************************************************************************
* Function Name  : TB_Init
* Description    : TimeBase peripheral initialization. The base time is set to 
*                  500usec and the related interrupt is enabled  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TB_Init(void)
{   
  /* Select AHB clock(HCLK) as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
  /* SysTick interrupt each 500usec with Core clock equal to 72MHz */
  SysTick_SetReload(36000);
  /* Enable SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Enable);

  /* Configure the SysTick handler priority */
  NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 2, 0); 
  /* Enable SysTick interrupt */
  SysTick_ITConfig(ENABLE);
}

/*******************************************************************************
* Function Name  : TB_Set_DisplayDelay_500us
* Description    : Set Delay utilized by main.c module.
* Input          : Time out value
* Output         : None
* Return         : None
*******************************************************************************/
void TB_Set_DisplayDelay_500us(u16 hDelay)
{
  hTimebase_display_500us = hDelay;
}  

/*******************************************************************************
* Function Name  : TB_DisplayDelay_IsElapsed
* Description    : Check if the delay set by TB_Set_DisplayDelay_500us is elapsed.   
* Input          : None
* Output         : True if delay is elapsed, false otherwise 
* Return         : None
*******************************************************************************/
bool TB_DisplayDelay_IsElapsed(void)
{
 if (hTimebase_display_500us == 0)
 {
   return (TRUE);
 }
 else 
 {
   return (FALSE);
 }
} 

/*******************************************************************************
* Function Name  : SysTickHandler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTickHandler(void)
{   
  if (hTimebase_display_500us != 0)  
  {
    hTimebase_display_500us --;
  }
 
  if (hSpeedMeas_Timebase_500us !=0)
  {
    hSpeedMeas_Timebase_500us--;
  }
  else
  {
    hSpeedMeas_Timebase_500us = SPEED_SAMPLING_TIME;
        
    //ENC_Calc_Average_Speed must be called ONLY every SPEED_MEAS_TIMEBASE ms
    ENC_Calc_Average_Speed();    
  }
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
