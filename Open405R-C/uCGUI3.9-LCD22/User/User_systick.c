/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"//#include "stm32f10x.h"
#include "User_systick.h"

static uint32_t TimingDelay;

void delay_1ms(uint32_t delay_conter)
{
	SysTick_Config((uint32_t)9000);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

	TimingDelay = delay_conter;
	while(TimingDelay!=0);
}

// void delay_10us(uint32_t delay_conter)
// { unsigned int i;
// // 	SysTick_Config((uint32_t)90);
// // 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

// 	TimingDelay = delay_conter;
// //	while(delay_conter>0)
// 	{
// 		for(i=1000;i<=0;i--)
//     if (delay_conter != 0x00)
//      { 
//        delay_conter--;
//       }
// 	}
// }

void TimingDelay_Decrement(void)
{
//   if (TimingDelay != 0x00)
//   { 
//     TimingDelay--;
//   }
}



