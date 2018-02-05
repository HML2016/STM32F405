#ifndef __USER_SYSTICK_H
#define __USER_SYSTICK_H

#include "stm32f4xx.h"

void delay_1ms(uint32_t delay_conter);
void delay_10us(uint32_t delay_conter);
extern void TimingDelay_Decrement(void);

#endif
