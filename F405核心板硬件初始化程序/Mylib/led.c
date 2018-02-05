#include "main.h"

/*----LED_GREEN----PA2-----'0' is on,'1' is off */
/*----LED_RED------PA1-----'0' is on,'1' is off */

void Led_Configuration(void)
{
    GPIO_InitTypeDef gpio;
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&gpio);
    
    LED_GREEN_OFF();
    LED_RED_OFF();
}

void emergency(void)
{   
	int i=12;
	LED_GREEN_OFF();
	LED_RED_ON();
	while(1)
	{
	delay_ms(50);
	LED_RED_TOGGLE();
	}
}
