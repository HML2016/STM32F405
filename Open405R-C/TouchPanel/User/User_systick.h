#ifndef __USER_SYSTICK_H
#define __USER_SYSTICK_H

#include "stm32f4xx.h"

void delay_ms(uint32_t delay_conter);
void delay_us(uint32_t delay_conter);
extern void TimingDelay_Decrement(void);

#endif
