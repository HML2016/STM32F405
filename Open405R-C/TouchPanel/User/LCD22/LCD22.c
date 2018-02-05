/*********************************************************************************************************
*
* @File                : LCD22.c
* @Hardware Environment: 
* @Build Environment   : RealView MDK-ARM  Version: 4.54
* @Version             : V1.0
* @date                : 201231221
* @Author              :jason
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/
#include "LCD22.h"
#include "Touchpanel.h"
#include "User_systick.h"

//****************************************** 
//发送数据 
//****************************************** 

unsigned char SPI_Communication(unsigned char send_char)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI1,(uint16_t)send_char);

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET);
	return ((unsigned char)SPI_I2S_ReceiveData(SPI1));
}

void SPI0_Init(void)
{
	
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3,  GPIO_AF_SPI1);
 	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	//GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);//IRQ
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_5;/// 
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1, &SPI_InitStructure); 
	
	SPI_Cmd(SPI1, ENABLE);
	
}

/*******************************************************************************
* Function Name  : LCD_Configuration
* Description    : Configure the LCD Control pins 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_Configuration(void)
{
/*
* 
*  LCD_MOSI ----------------  PB15
*  LCD_MISO ----------------  PB14 
*  LCD_CLK------------------  PB13
*
*  LCD_CS ------------------  PB12
*  LCD_RST  ----------------  PB9
*	 LCD_RS ------------------  PB8
*  LCD_PWM -----------------  PB0
*/	
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
// 	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
// 	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
// 	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
// 			GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
// 	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	
	SPI0_Init();
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : 
* Input          : - index:address of the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void LCD_WriteIndex(uint16_t index)
{
	//select command register
	TOUCH_nCS_H();
	LCD_CS_L();
//	delay_ms(1);
	LCD_RS_L();
	
	SPI_Communication((uint8_t)(index>>8));    //00000000 000000000
	SPI_Communication((uint8_t)(index));

	LCD_RS_H();
//	delay_ms(1);
	LCD_CS_H();
}


/*******************************************************************************
* Function Name  : LCD_WriteData
* Description    : 
* Input          : - data:value to write to the selected register
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void LCD_WriteData(uint16_t data)
{
	//send data
	TOUCH_nCS_H();
	LCD_CS_L();
//	delay_ms(1);
	LCD_RS_H();
	SPI_Communication((uint8_t)(data>>8));    //00000000 000000000
	SPI_Communication((uint8_t)(data));
	LCD_CS_H();
	
}


/*******************************************************************************
* Function Name  : LCD_ReadData
* Description    : 
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
__inline uint16_t LCD_ReadData(void)
{

}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{
	//select command register	
	TOUCH_nCS_H();	
	LCD_CS_L();
	LCD_RS_L();	
	SPI_Communication((uint8_t)(LCD_Reg>>8));    //00000000 000000000
	SPI_Communication((uint8_t)(LCD_Reg));
	LCD_RS_H();
	SPI_Communication((uint8_t)(LCD_RegValue>>8));    //00000000 000000000
	SPI_Communication((uint8_t)(LCD_RegValue));
	LCD_CS_H();
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention		 : None
*******************************************************************************/
__inline uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{

}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Set LCD coordinates.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

static void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos )
{
	LCD_WriteReg(0x210,Xpos);
	LCD_WriteReg(0x212,Ypos);
	LCD_WriteReg(0x211,Xpos);
	LCD_WriteReg(0x213,Ypos);
}

/*******************************************************************************
* Function Name  : LCD_Initializtion
* Description    : SSD1963 Resets
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Initializtion(void)
{
	LCD_Configuration();
	LCD_RST_L();
	delay_ms(10);
	LCD_RST_H();
	delay_ms(10);
 	LCD_WriteReg( 0x000, 0x0001 ); /* oschilliation start */
	delay_ms(10);
	/* Power settings */  	
	LCD_WriteReg( 0x100, 0x0000 ); /*power supply setup*/	
	LCD_WriteReg( 0x101, 0x0000 ); 
	LCD_WriteReg( 0x102, 0x3110 ); 
	LCD_WriteReg( 0x103, 0xe200 ); 
	LCD_WriteReg( 0x110, 0x009d ); 
	LCD_WriteReg( 0x111, 0x0022 ); 
	LCD_WriteReg( 0x100, 0x0120 ); 
	delay_ms( 20 );

	LCD_WriteReg( 0x100, 0x3120 );
	delay_ms( 80 );
	/* Display control */   
	LCD_WriteReg( 0x002, 0x0000 );
	
  LCD_WriteReg( 0x001, 0x0100 );
	LCD_WriteReg( 0x003, 0x1230  );  //high-speed write mode
//	LCD_WriteReg( 0x003, 0x1030 );    
		
	LCD_WriteReg( 0x006, 0x0000 );
	LCD_WriteReg( 0x007, 0x0101 );
	LCD_WriteReg( 0x008, 0x0808 );
	LCD_WriteReg( 0x009, 0x0000 );
	LCD_WriteReg( 0x00b, 0x0000 );
	LCD_WriteReg( 0x00c, 0x0000 );
	LCD_WriteReg( 0x00d, 0x0018 );
	/* LTPS control settings */   
	LCD_WriteReg( 0x012, 0x0000 );
	LCD_WriteReg( 0x013, 0x0000 );
	LCD_WriteReg( 0x018, 0x0000 );
	LCD_WriteReg( 0x019, 0x0000 );

	LCD_WriteReg( 0x203, 0x0000 );
	LCD_WriteReg( 0x204, 0x0000 );

	LCD_WriteReg( 0x210, 0x0000 );
	LCD_WriteReg( 0x211, 0x00ef );
	LCD_WriteReg( 0x212, 0x0000 );
	LCD_WriteReg( 0x213, 0x013f );
	LCD_WriteReg( 0x214, 0x0000 );
	LCD_WriteReg( 0x215, 0x0000 );
	LCD_WriteReg( 0x216, 0x0000 );
	LCD_WriteReg( 0x217, 0x0000 );

	// Gray scale settings
	LCD_WriteReg( 0x300, 0x5343);
	LCD_WriteReg( 0x301, 0x1021);
	LCD_WriteReg( 0x302, 0x0003);
	LCD_WriteReg( 0x303, 0x0011);
	LCD_WriteReg( 0x304, 0x050a);
	LCD_WriteReg( 0x305, 0x4342);
	LCD_WriteReg( 0x306, 0x1100);
	LCD_WriteReg( 0x307, 0x0003);
	LCD_WriteReg( 0x308, 0x1201);
	LCD_WriteReg( 0x309, 0x050a);

	/* RAM access settings */ 
	LCD_WriteReg( 0x400, 0x4027 );
	LCD_WriteReg( 0x401, 0x0000 );
	LCD_WriteReg( 0x402, 0x0000 );	/* First screen drive position (1) */   	
	LCD_WriteReg( 0x403, 0x013f );	/* First screen drive position (2) */   	
	LCD_WriteReg( 0x404, 0x0000 );

	LCD_WriteReg( 0x200, 0x0000 );
	LCD_WriteReg( 0x201, 0x0000 );
	
	LCD_WriteReg( 0x100, 0x7120 );
	LCD_WriteReg( 0x007, 0x0103 );
	delay_ms( 10 );
	LCD_WriteReg( 0x007, 0x0113 );	
//	LCD_Clear(Red);

}

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : 
* Input          : - Color: Screen Color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Clear(uint16_t Color)
{
	uint32_t index=0;
	TOUCH_nCS_H();
	LCD_WriteReg( 0x003, 0x1230 );  //高速页面更新模式

	LCD_WriteReg(0x210,0x0000);
	LCD_WriteReg(0x212,0x0000);
	LCD_WriteReg(0x211,0xEF);
	LCD_WriteReg(0x213,0x013F);
	
	LCD_WriteReg(0x200,0x0000);
	LCD_WriteReg(0x201,0x0000);
	
	LCD_WriteIndex(0x202);	//RAM Write index
	LCD_CS_L();
	LCD_RS_H();
	
	for( index = 0; index < MAX_X * MAX_Y; index++ )
	{
		LCD_WriteData(Color);
	}
	
	LCD_CS_H();
	LCD_WriteReg( 0x003, 0x1030 );    //低速模式
	
}

/******************************************************************************
* Function Name  : LCD_BGR2RGB
* Description    : RRRRRGGGGGGBBBBB convert to BBBBBGGGGGGRRRRR
* Input          : RGB color
* Output         : None
* Return         : RGB color
* Attention		 :
*******************************************************************************/
static uint16_t LCD_BGR2RGB(uint16_t color)
{
	uint16_t  r, g, b, rgb;
	
	b = ( color>>0 )  & 0x1f;
	g = ( color>>5 )  & 0x3f;
	r = ( color>>11 ) & 0x1f;
	
	rgb =  (b<<11) + (g<<5) + (r<<0);
	
	return( rgb );
}

/******************************************************************************
* Function Name  : LCD_GetPoint
* Description    : Get color of the point
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : Screen Color
* Attention		 : None
*******************************************************************************/
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos)
{

}

/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
	if( Xpos >= MAX_X || Ypos >= MAX_Y )
	{
		return;
	}
	TOUCH_nCS_H();
	LCD_WriteReg(0x210,0x00);
	LCD_WriteReg(0x212,0x00);
	LCD_WriteReg(0x211,MAX_X);
	LCD_WriteReg(0x213,MAX_Y);
	LCD_WriteReg(0x200,Xpos);	
	LCD_WriteReg(0x201,Ypos);	
	LCD_WriteReg(0x202,point);	
}



/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Bresenham's line algorithm
* Input          : - x0:
*                  - y0:
*       				   - x1:
*       		       - y1:
*                  - color:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
  short dx,dy;
  short temp;

  if( x0 > x1 )
  {
    temp = x1;
    x1 = x0;
    x0 = temp;   
  }
  if( y0 > y1 )
  {
    temp = y1;
    y1 = y0;
    y0 = temp;   
  }

  dx = x1-x0;
  dy = y1-y0;

  if( dx == 0 )
  {
    do
    { 
      LCD_SetPoint(x0, y0, color);
      y0++;
    }
    while( y1 >= y0 ); 
    return; 
  }
  if( dy == 0 )
  {
    do
    {
      LCD_SetPoint(x0, y0, color);
      x0++;
    }
    while( x1 >= x0 ); 
		return;
  }

	/* Bresenham's line algorithm  */
  if( dx > dy )
  {
    temp = 2 * dy - dx;
    while( x0 != x1 )
    {
	    LCD_SetPoint(x0,y0,color);
	    x0++;
	    if( temp > 0 )
	    {
	      y0++;
	      temp += 2 * dy - 2 * dx; 
	 	  }
      else         
      {
			  temp += 2 * dy;
			}       
    }
    LCD_SetPoint(x0,y0,color);
  }  
  else
  {
    temp = 2 * dx - dy;
    while( y0 != y1 )
    {
	 	  LCD_SetPoint(x0,y0,color);     
      y0++;                 
      if( temp > 0 )           
      {
        x0++;               
        temp+=2*dy-2*dx; 
      }
      else
			{
        temp += 2 * dy;
			}
    } 
    LCD_SetPoint(x0,y0,color);
	}
} 
/******************************************************************************
* Function Name  : LCD_Drawcircle
* Description    : Draw round
* Input  
* int x,         : - x0:
* int y,         : - y0:
* int r,      	 : - radius:
* int color      : - color:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 


void LCD_Drawcircle(int x0, int y0, int r, int color)
{
   int  draw_x0, draw_y0;                        
    int  draw_x1, draw_y1;        
    int  draw_x2, draw_y2;        
    int  draw_x3, draw_y3;        
    int  draw_x4, draw_y4;        
    int  draw_x5, draw_y5;        
    int  draw_x6, draw_y6;        
    int  draw_x7, draw_y7;        
    int  xx, yy;                                  

    int  di;                                           
   
 
   if(0==r) return;
   

   draw_x0 = draw_x1 = x0;
   draw_y0 = draw_y1 = y0 + r;
   if(draw_y0<MAX_Y) LCD_SetPoint(draw_x0, draw_y0, color);        // 90?è
        
   draw_x2 = draw_x3 = x0;
   draw_y2 = draw_y3 = y0 - r;
   if(draw_y2>=0) LCD_SetPoint(draw_x2, draw_y2, color);                        // 270?è
   
        
   draw_x4 = draw_x6 = x0 + r;
   draw_y4 = draw_y6 = y0;
   if(draw_x4<MAX_X) LCD_SetPoint(draw_x4, draw_y4, color);        // 0?è
   
   draw_x5 = draw_x7 = x0 - r;
   draw_y5 = draw_y7 = y0;
   if(draw_x5>=0) LCD_SetPoint(draw_x5, draw_y5, color);                        // 180?è   
   if(1==r) return;                                        // è?°????a1￡??òò??2?-íê
   
   
   /* ê1ó?Bresenham·¨??DD?-?2 */
   di = 3 - 2*r;                                        // 3?ê??ˉ??2?±?á?
   
   xx = 0;
   yy = r;        
   while(xx<yy)
   {  if(di<0)
          {  di += 4*xx + 6;              
          }
          else
          {  di += 4*(xx - yy) + 10;
          
             yy--;          
                 draw_y0--;
                 draw_y1--;
                 draw_y2++;
                 draw_y3++;
                 draw_x4--;
                 draw_x5++;
                 draw_x6--;
                 draw_x7++;                 
          }
          
          xx++;   
          draw_x0++;
          draw_x1--;
          draw_x2++;
          draw_x3--;
          draw_y4++;
          draw_y5++;
          draw_y6--;
          draw_y7--;
                
        
          /* òa?D??μ±?°μ?ê?·??úóDD§·??§?ú */
          if( (draw_x0<=MAX_X)&&(draw_y0>=0) )        
          {  LCD_SetPoint(draw_x0, draw_y0, color);
          }            
          if( (draw_x1>=0)&&(draw_y1>=0) )        
          {  LCD_SetPoint(draw_x1, draw_y1, color);
          }
          if( (draw_x2<=MAX_X)&&(draw_y2<=MAX_Y) )        
          {  LCD_SetPoint(draw_x2, draw_y2, color);   
          }
          if( (draw_x3>=0)&&(draw_y3<=MAX_Y) )        
          {  LCD_SetPoint(draw_x3, draw_y3, color);
          }
          if( (draw_x4<=MAX_X)&&(draw_y4>=0) )        
          {  LCD_SetPoint(draw_x4, draw_y4, color);
          }
          if( (draw_x5>=0)&&(draw_y5>=0) )        
          {  LCD_SetPoint(draw_x5, draw_y5, color);
          }
          if( (draw_x6<=MAX_X)&&(draw_y6<=MAX_Y) )        
          {  LCD_SetPoint(draw_x6, draw_y6, color);
          }
          if( (draw_x7>=0)&&(draw_y7<=MAX_Y) )        
          {  LCD_SetPoint(draw_x7, draw_y7, color);
          }
   }
}

/******************************************************************************
* Function Name  : PutChar
* Description    : 
* Input          : - Xpos:
*                  - Ypos:
*				           - ASCI:
*				           - charColor:
*				           - bkColor:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	
	  uint16_t i, j;
    uint8_t buffer[16], tmp_char;
	
	LCD_WriteReg(0x210,Xpos); 	//x start point
	LCD_WriteReg(0x212,Ypos); 	//y start point
	LCD_WriteReg(0x211,Xpos+7);	//x end point
	LCD_WriteReg(0x213,Ypos+15);	//y end point

	LCD_WriteReg(0x200,Xpos);	//RAM X address(0 ~ FF, actually 0 ~ EF)	
	LCD_WriteReg(0x201,Ypos);	//RAM Y address(0 ~ 1FF, actually 0 ~ 13F)
	
	LCD_WriteIndex(0x202);	//RAM Write index	
	
	LCD_CS_L();
	LCD_RS_H();
	
	GetASCIICode(buffer,ASCI);
	for(i=0;i<16;i++)
	{
		tmp_char = buffer[i];
		for(j=0;j<8;j++)
		{
			if( (tmp_char >> 7 - j) & 0x01 == 0x01 )
			LCD_WriteData(charColor);
			else
			LCD_WriteData(bkColor);

		}	
	}

	LCD_CS_H();
}

/******************************************************************************
* Function Name  : GUI_Text
* Description    : 
* Input          : - Xpos: 
*                  - Ypos: 
*				           - str:
*				           - charColor:
*				           - bkColor:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
    do
    {
        TempChar = *str++;  
        PutChar( Xpos, Ypos, TempChar, Color, bkColor );    
        if( Xpos < MAX_X )
        {
            Xpos += 8;;
        } 
        else if ( Ypos < MAX_Y)
        {
            Xpos = 0;
            Ypos += 16;
        }   
        else
        {
            Xpos = 0;
            Ypos = 0;
        }   
				
    }
    while ( *str != 0 );
}
/******************************************************************************
* Function Name  : DrawGrids
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

void DrawGrids(void)
{
	uint16_t i;
	LCD_Clear(Grey);	
	for(i=0;i<MAX_Y;i=i+10)
	LCD_DrawLine(0,i,MAX_X,i,Black);

	for(i=0;i<MAX_X;i=i+10)
	LCD_DrawLine(i,0,i,MAX_Y,Black);
}
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

