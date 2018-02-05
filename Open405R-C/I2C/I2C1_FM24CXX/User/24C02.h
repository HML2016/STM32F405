/*********************************************************************************************************
*
* File                : 24c02.h
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#ifndef __24C02_H
#define __24C02_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "config.h"
					  
/* Private define ------------------------------------------------------------*/
#define AT24C01A
//#define AT24C01

#define ADDR_24LC02		0xA0
#define I2C_PAGESIZE	4


/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define I2C_Open_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2C_Open_LONG_TIMEOUT         ((uint32_t)(10 * I2C_Open_FLAG_TIMEOUT))

/* Private function prototypes -----------------------------------------------*/
void I2C_Configuration(void);
uint8_t I2C_Read(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t num);
uint8_t I2C_Write(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t num);

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

