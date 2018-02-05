/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Main program body
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
#include "stm32f4xx.h"
#include "usart.h"
#include "main.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
CanTxMsg TxMessage;
uint8_t KeyNumber = 0x0;

/* Private variables ---------------------------------------------------------*/
uint16_t CAN_ID;
uint8_t CAN_DATA0,CAN_DATA1,CAN_DATA2,CAN_DATA3,CAN_DATA4,CAN_DATA5,CAN_DATA6,CAN_DATA7;
uint8_t CanFlag,Display;

void Delay(void);
void GPIO_Configuration(void);
void CAN_Config(void);
void CAN2_Config(void);
void NVIC_Config(void);
void USB_LP_CAN1_RX0_IRQHandler(void);
void CanWriteData(uint16_t ID);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

  USART_Configuration();
  printf("\r\n****************************************************************\r\n");
  printf("CAN-Bus Test \r\n");
  printf("CAN-Bus Speed 100kHz \r\n");

  CAN_Config();//CAN_Config(); 
  NVIC_Config();
  GPIO_Configuration();
    /* Infinite loop */
  while (1)
	{
	  if( CanFlag == ENABLE )
	  {
      CanFlag = DISABLE;
      printf("CAN Receive Data \r\n");
      printf("CAN ID %x \r\n",CAN_ID);
      printf("CAN_DATA0 %x \r\n",CAN_DATA0);
      printf("CAN_DATA1 %x \r\n",CAN_DATA1);
      printf("CAN_DATA2 %x \r\n",CAN_DATA2);
      printf("CAN_DATA3 %x \r\n",CAN_DATA3);
      printf("CAN_DATA4 %x \r\n",CAN_DATA4);
      printf("CAN_DATA5 %x \r\n",CAN_DATA5);
      printf("CAN_DATA6 %x \r\n",CAN_DATA6);
      printf("CAN_DATA7 %x \r\n",CAN_DATA7);
	  }

    CanWriteData(0xA5A5);

    if( Display ) 
    { 
      /*====LED-ON=======*/
      GPIO_SetBits(GPIOB , GPIO_Pin_0);
      GPIO_SetBits(GPIOB , GPIO_Pin_1);
      GPIO_SetBits(GPIOB , GPIO_Pin_2);
      GPIO_SetBits(GPIOB , GPIO_Pin_3);
    }
    else 
    { 
      /*====LED-OFF=======*/ 
      GPIO_ResetBits(GPIOB , GPIO_Pin_0);
      GPIO_ResetBits(GPIOB , GPIO_Pin_1);
      GPIO_ResetBits(GPIOB , GPIO_Pin_2);
      GPIO_ResetBits(GPIOB , GPIO_Pin_3);
    }
    Display = ~Display;   	
    Delay();  /* delay 200ms */  
  }
}

/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */
void CAN_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(Open_CAN_GPIO_CLK, ENABLE);
  RCC_APB1PeriphClockCmd(Open_CAN_CLK, ENABLE);
  /* Connect CAN pins to AF9 */
  GPIO_PinAFConfig(Open_CAN_GPIO_PORT, Open_CAN_RX_SOURCE, Open_CAN_AF_PORT);
  GPIO_PinAFConfig(Open_CAN_GPIO_PORT, Open_CAN_TX_SOURCE, Open_CAN_AF_PORT); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = Open_CAN_RX_PIN | Open_CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(Open_CAN_GPIO_PORT, &GPIO_InitStructure);

  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB1PeriphClockCmd(Open_CAN_CLK, ENABLE);
  /* CAN register init */
  CAN_DeInit(Open_CANx);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
  /* CAN Baudrate = 1MBps (CAN clocked at 30 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 2;
  CAN_Init(Open_CANx, &CAN_InitStructure);

  /* CAN filter init */
#ifdef  OpenCAN1
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
#endif
#ifdef  OpenCAN2
  CAN_FilterInitStructure.CAN_FilterNumber = 14;
#endif
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* Transmit Structure preparation */
  TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x01;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 1;
  
  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(Open_CANx, CAN_IT_FMP0, ENABLE);
}

/**
  * @brief  Configures the NVIC for CAN.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;

#ifdef  OpenCAN1 
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
#else  /* USE_CAN2 */
  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
#endif /* OpenCAN1 */

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}
					
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//LED1 (PC9)  LED2 (PC10)  LED3 (PC11)  LED4 (PC12)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}
  			    

/*******************************************************************************
* Function Name  : CanWriteData
* Description    : Can Write Date to CAN-BUS
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void CanWriteData(uint16_t ID)
{
  CanTxMsg TxMessage;

  CAN_DATA0=rand()%0xff;  CAN_DATA1=rand()%0xff;  
  CAN_DATA2=rand()%0xff;  CAN_DATA3=rand()%0xff;  
  CAN_DATA4=rand()%0xff;  CAN_DATA5=rand()%0xff;
  CAN_DATA6=rand()%0xff;  CAN_DATA7=rand()%0xff; 

  /* transmit */
  TxMessage.StdId = ID;
//TxMessage.ExtId = 0x00;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 8;
  TxMessage.Data[0] = CAN_DATA0;    
  TxMessage.Data[1] = CAN_DATA1;    
  TxMessage.Data[2] = CAN_DATA2;    
  TxMessage.Data[3] = CAN_DATA3;    
  TxMessage.Data[4] = CAN_DATA4;    
  TxMessage.Data[5] = CAN_DATA5;     
  TxMessage.Data[6] = CAN_DATA6;    
  TxMessage.Data[7] = CAN_DATA7;      
  CAN_Transmit(Open_CANx,&TxMessage);
}


// #ifdef OpenCAN1
// /**
//   * @brief  This function handles CAN1 RX0 request.
//   * @param  None
//   * @retval None
//   */
// void CAN1_RX0_IRQHandler(void)
// {
//   CanRxMsg RxMessage;
//   CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
//   CAN_ID=RxMessage.StdId;
//   CAN_DATA0=RxMessage.Data[0];
//   CAN_DATA1=RxMessage.Data[1];
//   CAN_DATA2=RxMessage.Data[2];
//   CAN_DATA3=RxMessage.Data[3];
//   CAN_DATA4=RxMessage.Data[4];
//   CAN_DATA5=RxMessage.Data[5];
//   CAN_DATA6=RxMessage.Data[6];
//   CAN_DATA7=RxMessage.Data[7];
//   CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
//   CanFlag = ENABLE;
// }
// #endif  /* OpenCAN1 */

// #ifdef OpenCAN2
// /**
//   * @brief  This function handles CAN1 RX0 request.
//   * @param  None
//   * @retval None
//   */
// void CAN2_RX0_IRQHandler(void)
// {
//   CanRxMsg RxMessage;
//   CAN_Receive(CAN2,CAN_FIFO0, &RxMessage);
//   CAN_ID=RxMessage.StdId;
//   CAN_DATA0=RxMessage.Data[0];
//   CAN_DATA1=RxMessage.Data[1];
//   CAN_DATA2=RxMessage.Data[2];
//   CAN_DATA3=RxMessage.Data[3];
//   CAN_DATA4=RxMessage.Data[4];
//   CAN_DATA5=RxMessage.Data[5];
//   CAN_DATA6=RxMessage.Data[6];
//   CAN_DATA7=RxMessage.Data[7];
//   CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);
//   CanFlag = ENABLE;
// }
// #endif  /* OpenCAN1 */

/**
  * @brief  Delay
  * @param  None
  * @retval None
  */
void Delay(void)
{
  uint32_t nTime = 0x0000;

  for(nTime = 0; nTime <0x5fffff; nTime++)
  {
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {

  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
