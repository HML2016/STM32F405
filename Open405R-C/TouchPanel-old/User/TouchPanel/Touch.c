#include "stm32f4xx.h"//#include "stm32f10x.h"
//#include "User_systick.h"
#include "LCD/LTM022A69B.h"
#include "LCD/LCD_lib.h"

#include "TouchPanel/touch.h"
#include "User_systick.h"



#define TOUCH_nCS_H()    GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define TOUCH_nCS_L()    GPIO_ResetBits(GPIOB,GPIO_Pin_9)


#define CMD_READ_X    0xD0  //0B11010000即用差分方式读X坐标
#define CMD_READ_Y    0x90  //0B10010000即用差分方式读Y坐标




_touch_dot touch_dot,*p_touch_dot;//定义笔



void TOUCH_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA,ENABLE);
	//GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}


void TOUCH_WRITE_REG(unsigned int index)
{
	
	LCD_RS_L();
	TOUCH_nCS_L();

    SPI0_communication((unsigned char)(index>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(index));

	TOUCH_nCS_H();
	LCD_RS_H();

}

/******************************************************************************
* Function Name  : LCD_WRITE_COMMAND
* Description    : send command to LCD
* Input          : index, data
* Output         : None
* Return         : None
******************************************************************************/
void TOUCH_WRITE_COMMAND(unsigned int index,unsigned int data)
{
	//select .command register

	LCD_RS_L();
	TOUCH_nCS_L();

    SPI0_communication((unsigned char)(index>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(index));
	LCD_CS_H();
	//send data
	LCD_RS_H();

	TOUCH_nCS_L();
    SPI0_communication((unsigned char)(data>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(data));
	TOUCH_nCS_H();

}

/*******************************************************************************
* Function Name  : LCD_WRITE_DATA
* Description    : write data to LCD
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TOUCH_WRITE_DATA(unsigned int data)
{
    SPI0_communication((unsigned char)(data>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(data));
}

/******************************************************************************
* Function Name  : GetTouchADC
* Description    : 读取1次XPT2046，获取1次ADC结果。
* Input          : 发向XPT2046的命令字节.
                    只能是如下的代码:
                    CMD_READ_X:读取X的ADC值                
                    CMD_READ_Y:读取Y的ADC值
* Output         : None
* Return         : AD转换的结果.
******************************************************************************/
unsigned int GetTouchADC (unsigned char data)
{	  
	unsigned int NUMH , NUML;
	unsigned int Num;
	LCD_CS_H();
	//delay_10us(5);
	TOUCH_nCS_L();
	
	//delay_10us(5);
	SPI0_communication(data);
	//delay_10us(5);            // 延时等待转换完成
	NUMH=SPI0_communication(0x00);
	NUML=SPI0_communication(0x00);
	Num=((NUMH)<<8)+NUML; 	
	Num>>=4;                // 只有高12位有效.
	TOUCH_nCS_H();
	//delay_10us(5);
	return(Num);   
}


#define READ_TIMES 10 //读取次数
#define LOST_VAL 4	  //丢弃值

/*************************************************
 功    能：  读取数次XPT2046,获取ADC结果,次数由 READ_TIMES 定义.
             与上一个函数相比，本函数对返回值进行了筛选.
 入口参数：cmd_code:发向XPT2046的控制字节.
            只能是如下的代码:
            CMD_READ_X:读取X的ADC值                
            CMD_READ_Y:读取Y的ADC值
*************************************************/

unsigned int GetTouchADCEx(unsigned char cmd_code)
{
	unsigned int i, j;
	unsigned int buf[READ_TIMES];
	unsigned int sum=0;
	unsigned int temp;

	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=GetTouchADC(cmd_code);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
}


/*************************************************/
/* 功    能：读取X轴和轴的ADC值                  */
/* 入口参数：&touch_dot.x_ad_val,&touch_dot.y_ad_val   */
/* 出口参数：0：成功（返回的X,Y_ADC值有效）      */
/*           1: 失败（返回的X,Y_ADC值无效）      */ 
/*************************************************/
unsigned char Read_ADS(unsigned int *x_ad,unsigned int *y_ad)
{
	unsigned int xtemp,ytemp;			 	 		  
	xtemp=GetTouchADCEx(CMD_READ_X);    //有筛选的读取X轴AD转换结果
	ytemp=GetTouchADCEx(CMD_READ_Y);	    //有筛选的读取Y轴AD转换结果											   
	if(xtemp<100||ytemp<100)
        return 1;   //读数失败
	*x_ad=xtemp;
	*y_ad=ytemp;        
	return 0;//读数成功
}

/*************************************************/
/* 功能：连续两次读取ADC值						 */
/* 原理：把两次读取的值作比较，在误差范围内可取  */ 
/* 入口参数：x:&touch_dot.x_ad_val(X坐标ADC值)      */
/*           y:&touch_dot.y_ad_val(Y坐标ADC值)      */
/* 出口参数：0：成功（返回的X,Y_ADC值有效）      */
/*           1: 失败（返回的X,Y_ADC值无效）      */ 
/*************************************************/
#define ERR_RANGE 50 //误差范围 

unsigned char Read_ADS2(unsigned long *x_ad,unsigned long *y_ad) 
{
	unsigned int x1,y1;
 	unsigned int x2,y2;
 	unsigned char res; 

    res=Read_ADS(&x1,&y1);  // 第一次读取ADC值 
    if(res==1)  return(1);	// 如果读数失败，返回1
    res=Read_ADS(&x2,&y2);	// 第二次读取ADC值   
    if(res==1)  return(1);   	// 如果读数失败，返回1
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
        &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x_ad=(x1+x2)/2;
        *y_ad=(y1+y2)/2;
        return 0;	 // 正确读取，返回0
    }
    else return 1;	 // 前后不在+-50内，读数错误 
} 

/*************************************************/
/* 功能：把读出的ADC值转换成坐标值               */
/*************************************************/	  
void convert_ad_to_xy(void)
{
//	touch_dot.x=(240-(touch_dot.x_ad_val-100)/7.500); // 把读到的X_ADC值转换成TFT X坐标值
//	touch_dot.y=(320-(touch_dot.y_ad_val-135)/5.705); // 把读到的Y_ADC值转换成TFT Y坐标值
//    touch_dot.x = (((touch_dot.x_ad_val * 240)>>12)-110)*2;
//    touch_dot.y = (((touch_dot.y_ad_val * 320)>>12)-150)*2;

//X=(240 * AD - 2100* 240) / 1900
//Y=(320 * AD - 2100* 320) / 1900
    touch_dot.x=(240*touch_dot.x_ad_val -2100*240)/ 1900;
//    touch_dot.y=(320*touch_dot.y_ad_val -2100*320)/ 1900;
			touch_dot.y=(320*touch_dot.y_ad_val -2100*320)/ 1900-200;
}
 
/*************************************************/
/* 功能：读取一次XY坐标值                        */
/*************************************************/	
unsigned char Read_Once(void)
{
//	touch_dot.pen_status=Pen_Up;
	if(Read_ADS2(&touch_dot.x_ad_val,&touch_dot.y_ad_val)==0)	// 如果读取数据成功
	{
		while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1))==0);   // 检测笔是不是还在屏上:IRQ为低电平(bit7为0)说明笔在屏上
		convert_ad_to_xy();   // 把读到的ADC值转变成TFT坐标值
		return 0;	// 返回0，表示成功
	}
	else return 1;	// 如果读取数据失败，返回1表示失败
}
/*************************************************/
/* 功能：持续读取XY坐标值                        */
/*************************************************/
unsigned char Read_Continue(void)
{
//	touch_dot.pen_status=Pen_Up;	  
	if(Read_ADS2( &touch_dot.x_ad_val,&touch_dot.y_ad_val )==0)	 // 如果读取数据成功
	{
		convert_ad_to_xy();   // 把读到的ADC值转变成TFT坐标值
		return 0;	   // 返回0，表示成功
	}
	else return 1;	   // 如果读取数据失败，返回1表示失败
}










