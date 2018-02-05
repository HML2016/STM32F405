#ifndef _18B20_H
#define _18B20_H

#include "stm32f4xx.h"
#include "OneWire.h"

void convertDs18b20(void);
u8* readID(void);
u16 readTemp(void);

#endif /*_18B20_H*/

