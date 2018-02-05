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


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Init Structure definition */
DAC_InitTypeDef            DAC_InitStructure;
TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void Delay(__IO uint32_t nCount);

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
       
  /* System Clocks Configuration */
  RCC_Configuration();   

  /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically 
     connected to the DAC converter. In order to avoid parasitic consumption, 
     the GPIO pin should be configured in analog */
  GPIO_Configuration();

  /* TIM2 Configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = 0x1;           /*TIM_Period 设置了在下一个更新事件装入活动的自动重装载寄存器周期的值。它的取值必须在 0x0000 和0xFFFF 之间。  */
  TIM_TimeBaseStructure.TIM_Prescaler = 0xA;        /*TIM_Prescaler设置了用来作为 TIMx 时钟频率除数的预分频值。它的取值必须在 0x0000 和0xFFFF 之间。  */
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    /*TIM_ClockDivision 设置了时钟分割。*/
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	/*TIM_CounterMode 选择了计数器模式。TIM_CounterMode_Up: TIM 向上计数模式 */
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM2 TRGO selection */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
  /*
TIMx：x 可以是 2，3 或者 4，来选择 TIM 外设 
TIM_TRGOSource：触发输出模式 


Table 539. TIM_TRGOSource值 
TIM_TRGOSource  描述 
TIM_TRGOSource_Reset  使用寄存器 TIM_EGR 的 UG 位作为触发输出
（TRGO） 
TIM_TRGOSource_Enable  使用计数器使能 CEN作为触发输出（TRGO） 
TIM_TRGOSource_Update  使用更新事件作为触发输出（TRGO） 
TIM_TRGOSource_OC1  一旦捕获或者比较匹配发生，当标志位 CC1F被设置时触发输出发送一个肯定脉冲（TRGO） 
TIM_TRGOSource_OC1Ref  使用 OC1REF 作为触发输出（TRGO） 
TIM_TRGOSource_OC2Ref  使用 OC2REF 作为触发输出（TRGO） 
TIM_TRGOSource_OC3Ref  使用 OC3REF 作为触发输出（TRGO） 
TIM_TRGOSource_OC4Ref  使用 OC4REF 作为触发输出（TRGO） 
  
  */

  /* DAC channel1 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_2047;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* DAC channel2 Configuration */
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_1023;
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

  /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is 
     automatically connected to the DAC converter. */
  DAC_Cmd(DAC_Channel_1, ENABLE);

  /* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is 
     automatically connected to the DAC converter. */
  DAC_Cmd(DAC_Channel_2, ENABLE);

  /* Set DAC dual channel DHR12RD register */
  DAC_SetDualChannelData(DAC_Align_12b_R, 0x100, 0x100);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);

  while (1)
  {
		GPIO_SetBits(GPIOC , GPIO_Pin_6);
		GPIO_SetBits(GPIOC , GPIO_Pin_7);
		GPIO_SetBits(GPIOC , GPIO_Pin_8);
		GPIO_SetBits(GPIOC , GPIO_Pin_9);
		Delay(0x5fffff);
		GPIO_ResetBits(GPIOC , GPIO_Pin_6);
		GPIO_ResetBits(GPIOC , GPIO_Pin_7);
		GPIO_ResetBits(GPIOC , GPIO_Pin_8);
		GPIO_ResetBits(GPIOC , GPIO_Pin_9);
		Delay(0x5fffff);
  }
}
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	  /* Enable peripheral clocks ------------------------------------------------*/
  /* GPIOA Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  /* TIM2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  /* DAC channel 1 & 2 (DAC_OUT1 = PA.4)(DAC_OUT2 = PA.5) configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);



	//LED1 (PC9)  LED2 (PC10)  LED3 (PC11)  LED4 (PC12)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

}

void RCC_Configuration(void)
{   
  /* Enable peripheral clocks ------------------------------------------------*/
  /* GPIOA Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  /* TIM2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}




/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
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
