/**
  ******************************************************************************
  * @file    I2C/EEPROM/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    30-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "usart.h"

#define USARTx_IRQHANDLER   USART1_IRQHandler

extern uint16_t CAN1_ID;
extern uint8_t CAN1_DATA0,CAN1_DATA1,CAN1_DATA2,CAN1_DATA3,CAN1_DATA4,CAN1_DATA5,CAN1_DATA6,CAN1_DATA7;

extern uint16_t CAN2_ID;
extern uint8_t CAN2_DATA0,CAN2_DATA1,CAN2_DATA2,CAN2_DATA3,CAN2_DATA4,CAN2_DATA5,CAN2_DATA6,CAN2_DATA7;

extern __IO uint8_t Can1Flag,Can2Flag;
/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup IOToggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/
void USARTx_IRQHANDLER(void)
{
  
}


/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
  CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
  CAN1_ID=RxMessage.StdId;
  CAN1_DATA0=RxMessage.Data[0];
  CAN1_DATA1=RxMessage.Data[1];
  CAN1_DATA2=RxMessage.Data[2];
  CAN1_DATA3=RxMessage.Data[3];
  CAN1_DATA4=RxMessage.Data[4];
  CAN1_DATA5=RxMessage.Data[5];
  CAN1_DATA6=RxMessage.Data[6];
  CAN1_DATA7=RxMessage.Data[7];
  CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
  Can1Flag = ENABLE;
}

/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  */
void CAN2_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
  CAN_Receive(CAN2,CAN_FIFO0, &RxMessage);
  CAN2_ID=RxMessage.StdId;
  CAN2_DATA0=RxMessage.Data[0];
  CAN2_DATA1=RxMessage.Data[1];
  CAN2_DATA2=RxMessage.Data[2];
  CAN2_DATA3=RxMessage.Data[3];
  CAN2_DATA4=RxMessage.Data[4];
  CAN2_DATA5=RxMessage.Data[5];
  CAN2_DATA6=RxMessage.Data[6];
  CAN2_DATA7=RxMessage.Data[7];
  CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);
  Can2Flag = ENABLE;
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
