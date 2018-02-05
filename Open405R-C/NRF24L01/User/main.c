/**
  ******************************************************************************
  * @file    GPIO/IOToggle/main.c 
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
#include "config.h"
#include "usart.h"
#include "NRF24L01/NRF24L01.h"

#define Open_TX	1			//发送

//#define Open_RX	0			//接收

extern u8 RX_BUF[];
extern u8 TX_BUF[];

void  Delay (uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

int main(void)
{
	USART_Configuration();
	USART_NVIC_Config();

	nRF24L01_Initial();
	printf("\r\nUSART1测试成功!NRF2401初始化成功!\r\n");
	while(1)
	{
		#ifdef Open_TX
			TX_BUF[0]=0xaa;
			NRF24L01_Send();
			_printf("\r\nSend data：0x%x\r\n",TX_BUF[0]);
			Delay(0xffffff);
		#else
			NRF24L01_Receive();
		#endif
	}

}


