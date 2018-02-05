#include "main.h"

int main(void)
{   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	Led_Configuration();
	KEY_Configuration();
	LED_GREEN_ON();
	USART3_Configuration();
	Buzzer_Configuration();
    while(1)
    {
	USART3_SendChar(0x06);
  //  delay_ms(50);
	BUZZER_ON();
	//delay_ms(50);	
	BUZZER_OFF();	
    }
}
