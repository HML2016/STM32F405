#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include "stm32f4xx.h"

/**
 * @brief Definition for COM port1, connected to USART1
 */ 
#define Open_USART                        USART3
#define Open_USART_CLK                    RCC_APB1Periph_USART3

#define Open_USART_TX_PIN                 GPIO_Pin_10
#define Open_USART_TX_GPIO_PORT           GPIOC
#define Open_USART_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define Open_USART_TX_SOURCE              GPIO_PinSource10
#define Open_USART_TX_AF                  GPIO_AF_USART3


#define Open_USART_RX_PIN                 GPIO_Pin_11
#define Open_USART_RX_GPIO_PORT           GPIOC
#define Open_USART_RX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define Open_USART_RX_SOURCE              GPIO_PinSource11
#define Open_USART_RX_AF                  GPIO_AF_USART3

#define Open_USART_IRQn                   USART3_IRQn

/**
 * @brief Definition for connected to I2C1
 */
#define Open_I2C                        	I2C2
#define Open_I2C_CLK                    	RCC_APB1Periph_I2C2

#define Open_I2C_SDA_PIN                 	GPIO_Pin_11
#define Open_I2C_SDA_GPIO_PORT           	GPIOB
#define Open_I2C_SDA_GPIO_CLK            	RCC_AHB1Periph_GPIOB
#define Open_I2C_SDA_SOURCE              	GPIO_PinSource11
#define Open_I2C_SDA_AF                  	GPIO_AF_I2C2

#define Open_I2C_SCL_PIN                 	GPIO_Pin_10
#define Open_I2C_SCL_GPIO_PORT           	GPIOB
#define Open_I2C_SCL_GPIO_CLK            	RCC_AHB1Periph_GPIOB
#define Open_I2C_SCL_SOURCE              	GPIO_PinSource10
#define Open_I2C_SCL_AF                  	GPIO_AF_I2C2

#define I2C_SPEED               1000000
#define I2C_SLAVE_ADDRESS7      0x30

#endif
