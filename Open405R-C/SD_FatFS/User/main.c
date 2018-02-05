/*********************************************************************************************************
*
* File                : main.c
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "sdcard.h"
#include "usart.h"
#include "string.h"
#include "diskio.h"
#include "ff.h"

/* Private functions ---------------------------------------------------------*/
FATFS fs;            // Work area (file system object) for logical drive
FIL fsrc, fdst;      // file objects
BYTE buffer[512]; // file copy buffer
FRESULT res;         // FatFs function common result code
UINT br, bw;         // File R/W count

#if _USE_LFN
char Fs_LongFileName[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
#endif
/*******************************************************************************
* Function Name  : NVIC_Config
* Description    : Configures SDIO IRQ channel.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
 
void OutPutFile(void)
{ 
	unsigned int a;
	FRESULT res;
	FILINFO finfo;
	DIR dirs;
	char path[16]={""};  
#if _USE_LFN
    finfo.lfname = Fs_LongFileName;
    finfo.lfsize = sizeof(Fs_LongFileName);
#endif

	printf(" 磁盘初始化:%d\r\n",disk_initialize(0));
	printf("挂载磁盘:%d\r\n",f_mount(0, &fs));

	if (f_opendir(&dirs, path) == FR_OK) 
	{
    	while (f_readdir(&dirs, &finfo) == FR_OK)  
    	{
      		if (finfo.fattrib & AM_ARC) 
      		{
        		if(!finfo.fname[0]) break;        
        		printf("短文件名:%s\r\n",finfo.fname);
#if _USE_LFN
        		printf("长文件名:%s\r\n",finfo.lfname);
#endif
        		res = f_open(&fsrc, finfo.fname, FA_OPEN_EXISTING | FA_READ);
        		br=1;
				a=0;
				printf("文件头255字节内容:");
					for(a=0; a<512; a++) buffer[a]=0; 
		    	    res = f_read(&fsrc, buffer, sizeof(buffer), &br);
					printf("%s\r\n",buffer);	
		    	    if (res || br == 0) break;   // error or eof
				f_close(&fsrc);                 
			}
		} 
	}
  
	while(1);
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{    
  SystemInit();/* Clock Config */
  
  NVIC_Configuration();/* Interrupt Config */

  USART_Configuration();  

  OutPutFile();
  
  /* Infinite loop */
  while (1)
  {}
}


/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
