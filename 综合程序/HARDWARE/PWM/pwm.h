#ifndef __PWM_H
#define	__PWM_H	   
#include "sys.h"

void TIM4_PWM_Init(u16 Hz);
void PWM_Out(u16 pwm_tem1,u16 pwm_tem2,u16 pwm_tem3,u16 pwm_tem4);
#endif
