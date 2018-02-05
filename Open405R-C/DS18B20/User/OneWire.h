#ifndef _ONEWIRE_H
#define _ONEWIRE_H

#include "stm32f4xx.h"

#define SET_DQ() 		GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define CLR_DQ() 		GPIO_ResetBits(GPIOC,GPIO_Pin_1)

#define GET_DQ() 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)

void Onewire_Enable_GPIO_Port(void);
void Onewire_OUT_PULL_UP(void);
void Onewire_OUT_FLOATING(void);
void Onewire_IN_FLOATING(void);
void Onewire_IN_PULL_UP(void);
void _delay_us(u8 us);
void resetOnewire(void) ;
u8 rOnewire(void);
void wOnewire(u8 data);

#endif /*_ONEWIRE_H*/
