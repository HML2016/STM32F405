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
#include "config.h"
#include <stdlib.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


#define USER_KEY_Port					GPIOA
#define USER_KEY_Pin					GPIO_Pin_1
#define USER_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOA

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
CanTxMsg TxMessage;

/* Private variables ---------------------------------------------------------*/
uint16_t CAN1_ID;
uint8_t CAN1_DATA0,CAN1_DATA1,CAN1_DATA2,CAN1_DATA3,CAN1_DATA4,CAN1_DATA5,CAN1_DATA6,CAN1_DATA7;

uint16_t CAN2_ID;
uint8_t CAN2_DATA0,CAN2_DATA1,CAN2_DATA2,CAN2_DATA3,CAN2_DATA4,CAN2_DATA5,CAN2_DATA6,CAN2_DATA7;

__IO uint8_t Can1Flag,Can2Flag;

void CAN1_Config(void);
void CAN2_Config(void);
void NVIC_Config(void);
void Can1WriteData(uint16_t ID);
void Can2WriteData(uint16_t ID);
void Key_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t error_can=0;
  USART_Configuration();
  printf("\r\nCAN-Bus Test \r\n");
  printf("CAN-Bus Speed 100kHz \r\n");
  printf("Please press the JOYSTICK middle button to continue!\r\n");

  Key_Config();
  CAN1_Config();
  CAN2_Config();
  NVIC_Config();
    /* Infinite loop */
  while (1)
	{
	  while(GPIO_ReadInputDataBit(USER_KEY_Port,USER_KEY_Pin));
	  Can1WriteData(0x123);
	  while(Can2Flag!= ENABLE);
      Can2Flag = DISABLE;	 
      printf("CAN2 Receive Data \r\n");
      printf("CAN2 ID %x \r\n",CAN2_ID);
      printf("CAN2_DATA0 %x \r\n",CAN2_DATA0);
      printf("CAN2_DATA1 %x \r\n",CAN2_DATA1);
      printf("CAN2_DATA2 %x \r\n",CAN2_DATA2);
      printf("CAN2_DATA3 %x \r\n",CAN2_DATA3);
      printf("CAN2_DATA4 %x \r\n",CAN2_DATA4);
      printf("CAN2_DATA5 %x \r\n",CAN2_DATA5);
      printf("CAN2_DATA6 %x \r\n",CAN2_DATA6);
      printf("CAN2_DATA7 %x \r\n",CAN2_DATA7);	
	  if(CAN2_ID!=0x123 || 
	  CAN2_DATA0!=CAN1_DATA0 || 
	  CAN2_DATA1!=CAN1_DATA1 || 
	  CAN2_DATA2!=CAN1_DATA2 || 
	  CAN2_DATA3!=CAN1_DATA3 || 
	  CAN2_DATA4!=CAN1_DATA4 || 
	  CAN2_DATA5!=CAN1_DATA5 || 
	  CAN2_DATA6!=CAN1_DATA6 || 
	  CAN2_DATA7!=CAN1_DATA7 )
	   {printf("CAN1 发送或者CAN2 接收有问题\r\n");error_can=1;}

      Can2WriteData(0x321);
	  while(Can1Flag!= ENABLE);
      Can1Flag = DISABLE; 
      printf("CAN1 Receive Data \r\n");
      printf("CAN1 ID %x \r\n",CAN1_ID);
      printf("CAN1_DATA0 %x \r\n",CAN1_DATA0);
      printf("CAN1_DATA1 %x \r\n",CAN1_DATA1);
      printf("CAN1_DATA2 %x \r\n",CAN1_DATA2);
      printf("CAN1_DATA3 %x \r\n",CAN1_DATA3);
      printf("CAN1_DATA4 %x \r\n",CAN1_DATA4);
      printf("CAN1_DATA5 %x \r\n",CAN1_DATA5);
      printf("CAN1_DATA6 %x \r\n",CAN1_DATA6);
      printf("CAN1_DATA7 %x \r\n",CAN1_DATA7);	
	  if(CAN1_ID!=0x321 || 
	  CAN1_DATA0!=CAN2_DATA0 || 
	  CAN1_DATA1!=CAN2_DATA1 || 
	  CAN1_DATA2!=CAN2_DATA2 || 
	  CAN1_DATA3!=CAN2_DATA3 || 
	  CAN1_DATA4!=CAN2_DATA4 || 
	  CAN1_DATA5!=CAN2_DATA5 || 
	  CAN1_DATA6!=CAN2_DATA6 || 
	  CAN1_DATA7!=CAN2_DATA7 )
	   {printf("CAN2 发送或者CAN1 接收有问题\r\n");error_can=1;}
  	  while(!GPIO_ReadInputDataBit(USER_KEY_Port,USER_KEY_Pin));
	  if(error_can==0){printf("CAN1 CAN2正常!\r\n");}
  }
}

void Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(USER_KEY_RCC_AHBPeriph, ENABLE);
	
	/* Configure Input pushpull mode */
	GPIO_InitStructure.GPIO_Pin = USER_KEY_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USER_KEY_Port, &GPIO_InitStructure);	
}

/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */
void CAN1_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* Connect CAN pins */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CAN1);
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
  CAN_Init(CAN1, &CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
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
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

void CAN2_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* CAN GPIOs configuration **************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

  /* Connect CAN pins to AF9 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CAN2);
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
  CAN_Init(CAN2, &CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 14;
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
  TxMessage.StdId = 0x123;
  TxMessage.ExtId = 0x01;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 1;
  
  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}

/**
  * @brief  Configures the NVIC for CAN.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
					
  			    

/*******************************************************************************
* Function Name  : Can1WriteData
* Description    : Can1 Write Date to CAN-BUS
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Can1WriteData(uint16_t ID)
{
  CanTxMsg TxMessage;

  CAN1_DATA0=rand()%0xff;  CAN1_DATA1=rand()%0xff;  
  CAN1_DATA2=rand()%0xff;  CAN1_DATA3=rand()%0xff;  
  CAN1_DATA4=rand()%0xff;  CAN1_DATA5=rand()%0xff;
  CAN1_DATA6=rand()%0xff;  CAN1_DATA7=rand()%0xff; 

  /* transmit */
  TxMessage.StdId = ID;
//TxMessage.ExtId = 0x00;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 8;
  TxMessage.Data[0] = CAN1_DATA0;    
  TxMessage.Data[1] = CAN1_DATA1;    
  TxMessage.Data[2] = CAN1_DATA2;    
  TxMessage.Data[3] = CAN1_DATA3;    
  TxMessage.Data[4] = CAN1_DATA4;    
  TxMessage.Data[5] = CAN1_DATA5;     
  TxMessage.Data[6] = CAN1_DATA6;    
  TxMessage.Data[7] = CAN1_DATA7;      
  CAN_Transmit(CAN1,&TxMessage);
}

/*******************************************************************************
* Function Name  : CanWriteData
* Description    : Can Write Date to CAN-BUS
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Can2WriteData(uint16_t ID)
{
  CanTxMsg TxMessage;

  CAN2_DATA0=rand()%0xff;  CAN2_DATA1=rand()%0xff;  
  CAN2_DATA2=rand()%0xff;  CAN2_DATA3=rand()%0xff;  
  CAN2_DATA4=rand()%0xff;  CAN2_DATA5=rand()%0xff;
  CAN2_DATA6=rand()%0xff;  CAN2_DATA7=rand()%0xff; 

  /* transmit */
  TxMessage.StdId = ID;
//TxMessage.ExtId = 0x00;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 8;
  TxMessage.Data[0] = CAN2_DATA0;    
  TxMessage.Data[1] = CAN2_DATA1;    
  TxMessage.Data[2] = CAN2_DATA2;    
  TxMessage.Data[3] = CAN2_DATA3;    
  TxMessage.Data[4] = CAN2_DATA4;    
  TxMessage.Data[5] = CAN2_DATA5;     
  TxMessage.Data[6] = CAN2_DATA6;    
  TxMessage.Data[7] = CAN2_DATA7;      
  CAN_Transmit(CAN2,&TxMessage);
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
