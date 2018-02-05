#ifndef _USART_H
#define _USART_H

#include <stdio.h>
#include "stm32f4xx.h"

/**
 * @brief Definition for COM port1, connected to USART3
 */  
#define Open207V_USART                        USART2
#define Open207V_USART_CLK                    RCC_APB1Periph_USART2

#define Open207V_USART_TX_PIN                 GPIO_Pin_2
#define Open207V_USART_TX_GPIO_PORT           GPIOA
#define Open207V_USART_TX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define Open207V_USART_TX_SOURCE              GPIO_PinSource2
#define Open207V_USART_TX_AF                  GPIO_AF_USART2


#define Open207V_USART_RX_PIN                 GPIO_Pin_3
#define Open207V_USART_RX_GPIO_PORT           GPIOA
#define Open207V_USART_RX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define Open207V_USART_RX_SOURCE              GPIO_PinSource3
#define Open207V_USART_RX_AF                  GPIO_AF_USART2

#define Open207V_USART_IRQn                   USART2_IRQn	 
/*
#define Open207V_USART                        USART3
#define Open207V_USART_CLK                    RCC_APB1Periph_USART3

#define Open207V_USART_TX_PIN                 GPIO_Pin_10
#define Open207V_USART_TX_GPIO_PORT           GPIOC
#define Open207V_USART_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define Open207V_USART_TX_SOURCE              GPIO_PinSource10
#define Open207V_USART_TX_AF                  GPIO_AF_USART3


#define Open207V_USART_RX_PIN                 GPIO_Pin_11
#define Open207V_USART_RX_GPIO_PORT           GPIOC
#define Open207V_USART_RX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define Open207V_USART_RX_SOURCE              GPIO_PinSource11
#define Open207V_USART_RX_AF                  GPIO_AF_USART3

#define Open207V_USART_IRQn                   USART3_IRQn	 */

void USART_Configuration(void);
void USART_NVIC_Config(void);

#endif /*_USART_H*/
