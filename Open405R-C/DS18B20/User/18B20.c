#include "18B20.h"


void convertDs18b20(void) 
{ 
    resetOnewire(); 
    wOnewire(0xcc); 
    wOnewire(0x44); 
}


u8* readID(void) 
{ 
	u8 ID[8],i,*p;
	resetOnewire(); 
	wOnewire(0x33);
	for(i=0;i<8;i++)
	{ID[i]=rOnewire();}
	p=ID;
	return p;
}


u16 readTemp(void) 
{ 
	u8 temp1,temp2;
	convertDs18b20();
	resetOnewire(); 
	wOnewire(0xcc); 
	wOnewire(0xbe); 	
	temp1=rOnewire(); 
	temp2=rOnewire(); 
	return (((temp2<<8)|temp1)*6.25); //0.0625=xx, 0.625=xx.x, 6.25=xx.xx
}

