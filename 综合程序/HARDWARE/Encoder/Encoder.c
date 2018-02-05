#include "Encoder.h"

ENCODER Encoder;
/**********************************
	Encoder1  `	TIM3 	PA6
				TIM3 	PA7
	Encoder2 	TIM12	PB14  TIM12不支持正交编码功能
				TIM12	PB15
************************************/
//void Encoder_configuration(void)
//{
//        GPIO_InitTypeDef ENCODER;
//        TIM_TimeBaseInitTypeDef TIMER3,TIMER12;
//        TIM_ICInitTypeDef IC_ENCODER;
//        NVIC_InitTypeDef nvic;

//        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);
//        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM12, ENABLE);
//       
//        //PA6、PA7的复用功能一定要分开配置才行
//        GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
//        GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);
//        ENCODER.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//        ENCODER.GPIO_Mode = GPIO_Mode_AF;
//        ENCODER.GPIO_Speed = GPIO_Speed_100MHz;
//        ENCODER.GPIO_PuPd = GPIO_PuPd_DOWN;
//        GPIO_Init(GPIOA, &ENCODER);
//		//PB14、PB15
//        GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12);
//        GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_TIM12);
//        ENCODER.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
//        ENCODER.GPIO_Mode = GPIO_Mode_AF;
//        ENCODER.GPIO_Speed = GPIO_Speed_100MHz;
//        ENCODER.GPIO_PuPd = GPIO_PuPd_DOWN;
//        GPIO_Init(GPIOB, &ENCODER);

//        /***********configure the TIM3**********/
//        TIMER3.TIM_Prescaler = 0;
//        TIMER3.TIM_Period = 0xffff;
//        TIMER3.TIM_ClockDivision = TIM_CKD_DIV1;
//        TIMER3.TIM_CounterMode = TIM_CounterMode_Up;   
//        TIM_TimeBaseInit(TIM3, &TIMER3);
//        /***********configure the TIM12***********/
//        TIMER12.TIM_Prescaler = 0;
//        TIMER12.TIM_Period = 0xffff;
//        TIMER12.TIM_ClockDivision = TIM_CKD_DIV1;
//        TIMER12.TIM_CounterMode = TIM_CounterMode_Up;   
//        TIM_TimeBaseInit(TIM12, &TIMER12);
//       
//        /**********Set TIM3 and TIM12 to the ecoder mode***************/
//        TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
//        TIM_EncoderInterfaceConfig(TIM12, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
//       
//        /**********Set TIM3 and TIM12 to the input capture mode***************/
//        IC_ENCODER.TIM_Channel        =        TIM_Channel_1;
//        IC_ENCODER.TIM_ICSelection        =        TIM_ICSelection_DirectTI;
//        TIM_ICInit(TIM3, &IC_ENCODER);
//        IC_ENCODER.TIM_Channel        =        TIM_Channel_2;
//        IC_ENCODER.TIM_ICSelection        =        TIM_ICSelection_DirectTI;
//        TIM_ICInit(TIM3, &IC_ENCODER);
//        IC_ENCODER.TIM_Channel        =        TIM_Channel_1;
//        IC_ENCODER.TIM_ICSelection        =        TIM_ICSelection_DirectTI;
//        TIM_ICInit(TIM12, &IC_ENCODER);
//        IC_ENCODER.TIM_Channel        =        TIM_Channel_2;
//        IC_ENCODER.TIM_ICSelection        =        TIM_ICSelection_DirectTI;
//        TIM_ICInit(TIM12, &IC_ENCODER);
//       
//        /*************Configurate interrupts of TIM3 and TIM12*********/
//        nvic.NVIC_IRQChannel = TIM3_IRQn;
//        nvic.NVIC_IRQChannelPreemptionPriority = 2;
//        nvic.NVIC_IRQChannelSubPriority = 2;
//        nvic.NVIC_IRQChannelCmd = ENABLE;
//        NVIC_Init(&nvic);
//        nvic.NVIC_IRQChannel = TIM12_IRQn;
//        nvic.NVIC_IRQChannelPreemptionPriority = 2;
//        nvic.NVIC_IRQChannelSubPriority = 2;
//        nvic.NVIC_IRQChannelCmd = ENABLE;
//        NVIC_Init(&nvic);
//       
//        /*Clear the interruput flag and then enable the interrupt*/
//        TIM_ClearFlag(TIM3, TIM_FLAG_Update);
//        TIM_ClearFlag(TIM12, TIM_FLAG_Update);
//        TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//        TIM_ITConfig(TIM12, TIM_IT_Update, ENABLE);
//       
//        /*Set TIM3 and TIM12 CNT to 0x7fff*/
//        TIM_SetCounter(TIM3,0x7fff);
//        TIM_SetCounter(TIM12,0x7fff);
//        /*Enable TIM3 and TIM12 */
//        TIM_Cmd(TIM3, ENABLE);
//        TIM_Cmd(TIM12, ENABLE);
//}

void Quad_Encoder_Configuration(void)
{
    GPIO_InitTypeDef gpio;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&gpio);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);
	
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
	TIM3->CNT = 0x7fff;
    TIM_Cmd(TIM3,ENABLE);

}

 
int32_t GetQuadEncoderDiff(void)
{
    int32_t cnt = 0; 
	cnt = (TIM3->CNT)-0x7fff;
	TIM3->CNT = 0x7fff;    	
    Encoder.Num_a=cnt;
	
	return cnt;
}
