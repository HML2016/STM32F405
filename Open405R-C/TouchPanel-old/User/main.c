/*********************************************************************************************************
*
* File                : main.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "User_systick.h"
#include "LCD/LTM022A69B.h"

#include "TouchPanel/touch.h"
#include <stdio.h>


unsigned char read_res=1 ;
extern _touch_dot touch_dot;
/*******************************************************************************
* Function Name  : Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void  Delay (uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
int main(void)
{
	//lcd_init();
	lcd_init();
//	lcd_display_test();
	TOUCH_init();
	
	lcd_display_string( "CPU:ATMAGE128A-AU          ", BLACK, GREEN, 0, 1 );
  lcd_display_string( "www.WaveShare.net          ", BLACK, GREEN, 0, 3 );
	lcd_draw_line(0, 0, 80, 80, 0xffff );	
	//lcd_clear_Rect(BLACK,10,10,100,100);
	lcd_draw_dot(100,100,BLACK);
   while (1)	
   {
		 if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4))
		read_res=Read_Continue();
		 delay_10us(1);
		 if(!read_res)
 		lcd_draw_dot((touch_dot.x-20),(touch_dot.y-2),BLACK);
		 read_res=1;
   }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
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

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

