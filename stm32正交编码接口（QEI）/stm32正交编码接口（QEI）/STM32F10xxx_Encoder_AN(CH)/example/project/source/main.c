/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : Jian-guo JIANG
* Date First Issued  : 2008.5.14
* Description        : Main program body
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
#include "lcd.h"
#include "stm32f10x_encoder.h"
#include "stm32f10x_Timebase.h"

/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KEY_2_PORT GPIOD
#define KEY_2_BIT  GPIO_Pin_3

#define KEY_3_PORT GPIOD
#define KEY_3_BIT  GPIO_Pin_4

// These are defines for reading the buttonS
#define  NOKEY      (u8)0
#define  KEY2       (u8)1
#define  KEY3       (u8)2
#define  KEY_HOLD   (u8)3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static DisplayType DisplayStatus = DISPLAY_TIMCNT;
static u8 bKey = NOKEY;
static u8 bPrevious_key = NOKEY;

/* Private functions ---------------------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void LcdShow_Init(void);
void KEYS_Init(void);
u8 KEYS_Read( void );

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{

#ifdef DEBUG
  debug();
#endif

  /* System Clocks Configuration */
  RCC_Configuration();

  /* GPIO ports pins Configuration */
  GPIO_Configuration();

  /* NVIC Configuration */
  NVIC_Configuration();

  /* Configure the systick */
  TB_Init();
  ENC_Init();
  
  LcdShow_Init();
  KEYS_Init();

  while(1)
  {    
    bKey = KEYS_Read();    // read key pushed (if any...)
    if (bKey == KEY2)
    {
      if (DisplayStatus == DISPLAY_W) DisplayStatus = DISPLAY_TIMCNT;
      else DisplayStatus++;
    }
    
    if (TB_DisplayDelay_IsElapsed() == TRUE)
    {
      TB_Set_DisplayDelay_500us(100);  // 50ms    
      LCD_Display(DisplayStatus);            
    }    
  }
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;

  /* RCC system reset(for debug purpose) */
//  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* ADCCLK = PCLK2/6 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }

  /* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
         | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

  /* TIM2 clocks enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* CAN Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure PC.04 -- PC.11 as Output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_6 | GPIO_Pin_7
        | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure PD.03, PC.04, PC.11 -- PC.15 as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_11 | GPIO_Pin_12 |
      GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Configure PE.00 -- PE.15 as Output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the NVIC and Vector Table base address.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* enabling interrupt */
  NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
}

/*******************************************************************************
* Function Name  : LcdShow_Init
* Description    : Configure the lcd dispaly: TIM2 initialize in Output Compare
*                  Timing Mode
* Input          : None
* Return         : None
* Comment        : TIM2 Configuration: Output Compare Timing Mode:
                   TIM2CLK = 36 *2 =72MHz, Prescaler = 17, TIM2 counter clock = 4 MHz
                   TIM2 update Period = ARR / TIM2 counter Period = 2 ms
                   CC1 OC period = 1ms
*******************************************************************************/
void LcdShow_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 8000;
  TIM_TimeBaseStructure.TIM_Prescaler = 17;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_ARRPreloadConfig(TIM2,DISABLE);
  /* only counter overflow/underflow generate U interrupt */
  TIM_UpdateRequestConfig(TIM2,TIM_UpdateSource_Regular);

  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_OCInitStructure.TIM_Pulse = 4000;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInit(TIM2, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

  /* TIM IT enable */
  TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_Update, ENABLE);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}

/*******************************************************************************
* Function Name  : KEYS_Init
* Description    : Init GPIOs for button management
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void KEYS_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
    
  /* Enable GPIOD clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
 
  GPIO_StructInit(&GPIO_InitStructure);
  
  /* Joystick GPIOs configuration*/
  
  GPIO_InitStructure.GPIO_Pin = KEY_2_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY_2_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = KEY_3_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY_3_PORT, &GPIO_InitStructure);    
}

/*******************************************************************************
* Function Name  : KEYS_Read
* Description    : Reads key from demoboard.
* Input          : None
* Output         : None
* Return         : Return KEY2,KEY3, KEY_HOLD or NOKEY
*******************************************************************************/
u8 KEYS_Read( void )
{
  /* "KEY2" key is pressed */
  if(!GPIO_ReadInputDataBit(KEY_2_PORT, KEY_2_BIT))
  {
    if (bPrevious_key == KEY2) 
    {
      return KEY_HOLD;
    }
    else
    {
      bPrevious_key = KEY2;
      return KEY2;
    }
  }
  /* "KEY3" key is pressed */
  else if(!GPIO_ReadInputDataBit(KEY_3_PORT, KEY_3_BIT))
  {
    if (bPrevious_key == KEY3) 
    {
      return KEY_HOLD;
    }
    else
    {
      bPrevious_key = KEY3;
      return KEY3;
    }
  }    
  /* No key is pressed */
  else
  {
    bPrevious_key = NOKEY;
    return NOKEY;
  }
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {

  }
}
#endif
/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
