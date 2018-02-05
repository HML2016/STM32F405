/******************************************************************************

* File Name          : LTM022A69B.c
* Author             : Librae
* Last Modified Date : 08/12/2010
* Description        : This file provides the 
						LTM022A69B LCD related functions' declaration.

******************************************************************************/
#include "stm32f4xx.h"//#include "stm32f10x.h"
//#include "User_systick.h"
#include "LTM022A69B.h"
//#include "LCD_lib.h"
#include "../Common/fonts.c"

static sFONT *LCD_Currentfonts;

  /* Global variables to set the written text color */
static __IO uint16_t TextColor = 0x0000, BackColor = 0xFFFF;

void lcd_ctrl_port_init(void);      //nRS,nCS ... initialization

//=============================================================================
//							LCD Basic Functions
//=============================================================================
/******************************************************************************
* Function Name  : LCD_WRITE_REG
* Description    : write LCD register
* Input          : index
* Output         : None
* Return         : None
******************************************************************************/

void SPI0_Init(void)
{
	
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3,  GPIO_AF_SPI1);
 	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_6| GPIO_Pin_7;/// 
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

//****************************************** 
//发送数据 
//****************************************** 

unsigned char SPI0_communication(unsigned char send_char)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI1,(uint16_t)send_char);

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET);
	return ((unsigned char)SPI_I2S_ReceiveData(SPI1));
}

void LCD_WRITE_REG(unsigned int index)
{
	uint16_t value_index;
	LCD_RS_L();
	LCD_CS_L();
	value_index=index;
	value_index =value_index>>8;
    SPI0_communication((unsigned char)(value_index));    //00000000 000000000

	value_index=index;
	value_index &=0x00ff;
    SPI0_communication((unsigned char)(index));
	LCD_CS_H();
	LCD_RS_H();
}


/******************************************************************************
* Function Name  : LCD_SEND_COMMAND
* Description    : send command to LCD
* Input          : index, data
* Output         : None
* Return         : None
******************************************************************************/
void LCD_SEND_COMMAND(unsigned int index,unsigned int data)
{
	//select command register
	LCD_RS_L();
	LCD_CS_L();
    SPI0_communication((unsigned char)(index>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(index));
	LCD_CS_H();
	//send data
	LCD_RS_H();
	LCD_CS_L();
    SPI0_communication((unsigned char)(data>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(data));
	LCD_CS_H();
}


/******************************************************************************
* Function Name  : LCD_WRITE_COMMAND
* Description    : send command to LCD
* Input          : index, data
* Output         : None
* Return         : None
******************************************************************************/
void LCD_WRITE_COMMAND(unsigned int index,unsigned int data)
{
	//select command register
	LCD_RS_L();
	LCD_CS_L();
    SPI0_communication((unsigned char)(index>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(index));
	LCD_CS_H();
	//send data
	LCD_RS_H();

	LCD_CS_L();
    SPI0_communication((unsigned char)(data>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(data));
	LCD_CS_H();
}

/*******************************************************************************
* Function Name  : LCD_WRITE_DATA
* Description    : write data to LCD
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WRITE_DATA(unsigned int data)
{
    SPI0_communication((unsigned char)(data>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(data));
}
/********************************************************************************
* Function Name  : lcd_ctrl_port_init
* Description    : nRS,nCS ... initialization  for LPC1114 PIN
* Input          : None
* Output         : None
* Return         : None
********************************************************************************/
void lcd_ctrl_port_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

// 	
// // 	GPIO_SetBits(GPIOC,GPIO_Pin_5);
 	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
// 	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
}
void delay_10us(uint32_t delay_conter)
{ unsigned int i,j,k;
	{
   for(j=delay_conter;j<=0;j--)
     { 
			 for(i=10000;i<=0;i--)for(k=1000;k<=0;k--);
     }
	}
}
/******************************************************************************
* Function Name  : lcd_init
* Description    : LCD initialization
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void STM324xG_LCD_Init(void)
{
  lcd_ctrl_port_init();
  SPI0_Init();
	
	LCD_RST_L();
	delay_10us(10);
	LCD_RST_H();
	delay_10us(10);

	LCD_WRITE_COMMAND( 0x000, 0x0001 ); /* oschilliation start */
	delay_10us( 10 );
	/* Power settings */  	
	LCD_WRITE_COMMAND( 0x100, 0x0000 ); /*power supply setup*/	
	LCD_WRITE_COMMAND( 0x101, 0x0000 ); 
	LCD_WRITE_COMMAND( 0x102, 0x3110 ); 
	LCD_WRITE_COMMAND( 0x103, 0xe200 ); 
	LCD_WRITE_COMMAND( 0x110, 0x009d ); 
	LCD_WRITE_COMMAND( 0x111, 0x0022 ); 
	LCD_WRITE_COMMAND( 0x100, 0x0120 ); 
	delay_10us( 10 );

	LCD_WRITE_COMMAND( 0x100, 0x3120 );
	delay_10us( 10 );
	/* Display control */   
	LCD_WRITE_COMMAND( 0x001, 0x0100 );
	LCD_WRITE_COMMAND( 0x002, 0x0000 );
	LCD_WRITE_COMMAND( 0x003, 0x1230 );
	LCD_WRITE_COMMAND( 0x006, 0x0000 );
	LCD_WRITE_COMMAND( 0x007, 0x0101 );
	LCD_WRITE_COMMAND( 0x008, 0x0808 );
	LCD_WRITE_COMMAND( 0x009, 0x0000 );
	LCD_WRITE_COMMAND( 0x00b, 0x0000 );
	LCD_WRITE_COMMAND( 0x00c, 0x0000 );
	LCD_WRITE_COMMAND( 0x00d, 0x0018 );
	/* LTPS control settings */   
	LCD_WRITE_COMMAND( 0x012, 0x0000 );
	LCD_WRITE_COMMAND( 0x013, 0x0000 );
	LCD_WRITE_COMMAND( 0x018, 0x0000 );
	LCD_WRITE_COMMAND( 0x019, 0x0000 );

	LCD_WRITE_COMMAND( 0x203, 0x0000 );
	LCD_WRITE_COMMAND( 0x204, 0x0000 );

	LCD_WRITE_COMMAND( 0x210, 0x0000 );
	LCD_WRITE_COMMAND( 0x211, 0x00ef );
	LCD_WRITE_COMMAND( 0x212, 0x0000 );
	LCD_WRITE_COMMAND( 0x213, 0x013f );
	LCD_WRITE_COMMAND( 0x214, 0x0000 );
	LCD_WRITE_COMMAND( 0x215, 0x0000 );
	LCD_WRITE_COMMAND( 0x216, 0x0000 );
	LCD_WRITE_COMMAND( 0x217, 0x0000 );

	// Gray scale settings
	LCD_WRITE_COMMAND( 0x300, 0x5343);
	LCD_WRITE_COMMAND( 0x301, 0x1021);
	LCD_WRITE_COMMAND( 0x302, 0x0003);
	LCD_WRITE_COMMAND( 0x303, 0x0011);
	LCD_WRITE_COMMAND( 0x304, 0x050a);
	LCD_WRITE_COMMAND( 0x305, 0x4342);
	LCD_WRITE_COMMAND( 0x306, 0x1100);
	LCD_WRITE_COMMAND( 0x307, 0x0003);
	LCD_WRITE_COMMAND( 0x308, 0x1201);
	LCD_WRITE_COMMAND( 0x309, 0x050a);

	/* RAM access settings */ 
	LCD_WRITE_COMMAND( 0x400, 0x4027 );
	LCD_WRITE_COMMAND( 0x401, 0x0000 );
	LCD_WRITE_COMMAND( 0x402, 0x0000 );	/* First screen drive position (1) */   	
	LCD_WRITE_COMMAND( 0x403, 0x013f );	/* First screen drive position (2) */   	
	LCD_WRITE_COMMAND( 0x404, 0x0000 );

	LCD_WRITE_COMMAND( 0x200, 0x0000 );
	LCD_WRITE_COMMAND( 0x201, 0x0000 );
	
	LCD_WRITE_COMMAND( 0x100, 0x7120 );
	LCD_WRITE_COMMAND( 0x007, 0x0103 );
	delay_10us( 10 );
	LCD_WRITE_COMMAND( 0x007, 0x0113 );

	delay_10us(2);
  LCD_SetFont(&LCD_DEFAULT_FONT);
	//LCD_Clear(0XF800);
}



/**
  * @brief  Sets the Text color.
  * @param  Color: specifies the Text color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetTextColor(__IO uint16_t Color)
{
  TextColor = Color;
}

/**
  * @brief  Sets the Background color.
  * @param  Color: specifies the Background color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetBackColor(__IO uint16_t Color)
{
  BackColor = Color;
}

/**
  * @brief  Prepare to write to the LCD RAM.
  * @param  None
  * @retval None
  */
void LCD_WriteRAM_Prepare(void)
{
	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();
}
/******************************************************************************
* Function Name  : lcd_clear_screen
* Description    : clear screen
* Input          : color_background: refresh background color
* Output         : None
* Return         : None
******************************************************************************/

void LCD_Clear(uint16_t color_background)
{
	unsigned int i,j;

	LCD_WRITE_COMMAND(0x210,0x00);
	LCD_WRITE_COMMAND(0x212,0x0000);
	LCD_WRITE_COMMAND(0x211,0xEF);
	LCD_WRITE_COMMAND(0x213,0x013F);

	LCD_WRITE_COMMAND(0x200,0x0000);
	LCD_WRITE_COMMAND(0x201,0x0000);
	
	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();

	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
		  LCD_WRITE_DATA( color_background );
		}
	}
	LCD_RS_L();
	LCD_CS_H();
}
/**
  * @brief  Gets the Text Font.
  * @param  None.
  * @retval the used font.
  */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}
/**
  * @brief  Sets the Text Font.
  * @param  fonts: specifies the font to be used.
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}
/******************************************************************************
* Function Name  : lcd_clear_area
* Description    : clear area,fill
* Input          : color_front:draw a rectangle, fill with color_front
                   x:
                   y:
                   width: rectangle width
                   height:rectangle height
* Output         : None
* Return         : None
******************************************************************************/
void LCD_DrawFullRect(uint16_t x, uint16_t y, uint16_t height, uint16_t width)
{
	unsigned int i, j;

	LCD_WRITE_COMMAND( 0x210, x ); 	//x start point
	LCD_WRITE_COMMAND( 0x212, y ); 	//y start point
	LCD_WRITE_COMMAND( 0x211, x + width - 1 );	//x end point
	LCD_WRITE_COMMAND( 0x213, y + height - 1 );	//y end point
		
	LCD_WRITE_COMMAND( 0x200, x );
	LCD_WRITE_COMMAND( 0x201, y );

	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();

	for( i = 0; i < height; i++ )
	{
		for( j = 0; j < width; j++ )
		{
			LCD_WRITE_DATA(TextColor);// 0x0000

		}
	}
	LCD_RS_L();
	LCD_CS_H();
}
void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t height, uint16_t width)
{
  lcd_draw_line(x, y, x+width, y,TextColor);
  lcd_draw_line(x, y, x, y+height,TextColor);
  
  lcd_draw_line(x+width, y, x+width, y+height,TextColor);
  lcd_draw_line(x, y+height, x+width, y+height,TextColor);
}
void lcd_clear_Rect(unsigned int color_front, 
                    unsigned int x0, 
                    unsigned int y0, 
                    unsigned int x1, 
                    unsigned int y1)
{
	unsigned int i, j;

	LCD_WRITE_COMMAND( 0x210, x0 ); 	//x start point
	LCD_WRITE_COMMAND( 0x212, y0 ); 	//y start point
	LCD_WRITE_COMMAND( 0x211, x1);	//x end point
	LCD_WRITE_COMMAND( 0x213, y1);	//y end point
		
	LCD_WRITE_COMMAND( 0x200, x0 );
	LCD_WRITE_COMMAND( 0x201, y0 );

	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();

	for( i = 0; i <=x1-x0; i++ )//;
	{
		for( j = 0; j <= y1-y0; j++ )
		{
			LCD_WRITE_DATA( color_front );

		}
	}

}
/******************************************************************************
* Function Name  : lcd_set_cursor
* Description    : Set cursor
* Input          : x, y
* Output         : None
* Return         : None
******************************************************************************/
void LCD_SetCursor(unsigned char x, unsigned int y)
{
  	if( (x > 320) || (y > 240) )
	{
		return;
	}
	LCD_WRITE_COMMAND( 0x200, x );
	LCD_WRITE_COMMAND( 0x201, y );
}

/******************************************************************************
* Function Name  : lcd_display_char
* Description    : ch_asc: ascii code of data. position_x, position_y.
					color_front, color_background.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void LCD_DrawChar(  uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  uint32_t index = 0, i = 0;
//   uint16_t  Xaddress = 0;
//   Xaddress = Xpos;
	
	LCD_WRITE_COMMAND(0x210,Xpos); 	//x start point
	LCD_WRITE_COMMAND(0x212,Ypos); 	//y start point
	LCD_WRITE_COMMAND(0x211,Xpos+LCD_Currentfonts->Width-1);	//x end point
	LCD_WRITE_COMMAND(0x213,Ypos+LCD_Currentfonts->Height-1);	//y end point

	LCD_WRITE_COMMAND(0x200,Xpos);	//RAM X address(0 ~ FF, actually 0 ~ EF)	
	LCD_WRITE_COMMAND(0x201,Ypos);	//RAM Y address(0 ~ 1FF, actually 0 ~ 13F)
	
	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();

  for(index = 0; index < LCD_Currentfonts->Height; index++)
	{
		
    for(i = 0; i < LCD_Currentfonts->Width; i++)
		{
      if((((c[index] & ((0x80 << ((LCD_Currentfonts->Width / 12 ) * 8 ) ) >> i)) == 0x00) &&(LCD_Currentfonts->Width <= 12))||
        (((c[index] & (0x1 << i)) == 0x00)&&(LCD_Currentfonts->Width > 12 )))

			{
				LCD_WRITE_DATA(BackColor);
			}
			else
			{
				LCD_WRITE_DATA(TextColor);
			}
   // Xaddress++;	
		}	
	}

	LCD_CS_H();
}
void LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii)
{
  Ascii -= 32;
  LCD_DrawChar(Line, Column, &LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height]);
}
/**
  * @brief  Displays a maximum of 20 char on the LCD.
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  *ptr: pointer to string to display on LCD.
  * @retval None
  */
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr)
{
//   uint16_t refcolumn = LCD_PIXEL_WIDTH - 1;
   uint16_t refcolumn =0;
  /* Send the string character by character on lCD */
  while ((*ptr != 0) )//& (((refcolumn + 1) & 0xFFFF) >= LCD_Currentfonts->Width)
  {
    /* Display one character on LCD */
    LCD_DisplayChar(refcolumn, Line, *ptr);
    /* Decrement the column position by 16 */
    refcolumn += LCD_Currentfonts->Width;
    /* Point on the next character */
    ptr++;
  }
}

/**
  * @brief  Clears the selected line.
  * @param  Line: the Line to be cleared.
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..n
  * @retval None
  */
void LCD_ClearLine(uint16_t Line)
{
  uint16_t refcolumn =0;// LCD_PIXEL_WIDTH - 1
  /* Send the string character by character on lCD */
  while (refcolumn<=240)//((refcolumn + 1)& 0xFFFF) >= LCD_Currentfonts->Width
  {
    /* Display one character on LCD */
    LCD_DisplayChar(refcolumn, Line, ' ');
    /* Decrement the column position by 16 */
    refcolumn += LCD_Currentfonts->Width;
  }
}
/******************************************************************************
* Function Name  : 汉字显示
* Description    : 16x16点阵的汉字显示函数
* Input          : gb:汉字对应字模中的位置，x:显示位置第几列，y:显示位置第几行
					color_front, color_background.  
* Output         : None
* Return         : None
******************************************************************************/
/*显示方法（年月日）
for( i = 2; i < 8; i++ )
{
	lcd_display_GB2312( i-2, (i-1)*2, 0 );
}
*/
void lcd_display_GB2312( unsigned char gb, 
						unsigned int color_front, 
						unsigned int color_background, 
						unsigned char postion_x, 
						unsigned char postion_y )
{
	unsigned char i,j,b;
	static unsigned char *p;
	
	LCD_WRITE_COMMAND(0x210,postion_x*16); 	//x start point
	LCD_WRITE_COMMAND(0x212,postion_y*16); 	//y start point
	LCD_WRITE_COMMAND(0x211,postion_x*16+15);	//x end point
	LCD_WRITE_COMMAND(0x213,postion_y*16+15);	//y end point
	LCD_WRITE_COMMAND(0x200,postion_x*16);	
	LCD_WRITE_COMMAND(0x201,postion_y*16);

	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();
	
	//p = (unsigned char *)GB2312;
	p += gb*32;
	for(j=0;j<32;j++)
	{
		b=*(p+j);
		for(i=0;i<8;i++)
		{
			if(b&0x80)
			{
				LCD_WRITE_DATA(color_front);
			}
			else
			{
				LCD_WRITE_DATA(color_background);
			}
			b=b<<1;
			
		}	
	}
	LCD_CS_H();
}

/******************************************************************************
* Function Name  : lcd_display_image
* Description    : Draw image
* Input          : x, y: image start at x, y. width, length, *img.
* Output         : None
* Return         : None
******************************************************************************/
void lcd_display_image( const unsigned char *img, 
					unsigned char x, 
					unsigned int y, 
					unsigned int width, 
					unsigned height )
{
	unsigned int i, j;
	unsigned int data16;


	LCD_WRITE_COMMAND( 0x210, x ); 	//x start point
	LCD_WRITE_COMMAND( 0x212, y ); 	//y start point
	LCD_WRITE_COMMAND( 0x211, x + width - 1 );	//x end point
	LCD_WRITE_COMMAND( 0x213, y + height - 1 );	//y end point
		
	LCD_WRITE_COMMAND( 0x200, x );
	LCD_WRITE_COMMAND( 0x201, y );

	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();

	for( i = 0; i < height; i++ )
	{
		for( j = 0; j < width; j++ )
		{
			//Be carful of MCU type, big endian or little endian
			//little endian
			data16 = ( *(img + 1) << 8 ) | (*img);
			LCD_WRITE_DATA( data16 );
			img += 2;
		}
	}
	LCD_CS_H();
}

//=============================================================================
//							Application Functions
//=============================================================================

/******************************************************************************
* Function Name  : lcd_display_test
* Description    : test
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
const unsigned int color[] =
{
	// 0 ~ 262143, 0x00000 ~ 0x3FFFF
	0xf800,	//red
	0x07e0,	//green
	0x001f,	//blue
	0xffe0,	//yellow
	0x0000,	//black
	0xffff,	//white
	0x07ff,	//light blue
	0xf81f	//purple
};

// void lcd_display_test(void)
// {
// 	unsigned int temp, num;
// 	unsigned int i;
// 	unsigned char n;

// 	lcd_clear_screen(RED);
//     delay_10us(60);
// 	LCD_WRITE_COMMAND(0x210,0x00);
// 	LCD_WRITE_COMMAND(0x212,0x0000);
// 	LCD_WRITE_COMMAND(0x211,0xEF);
// 	LCD_WRITE_COMMAND(0x213,0x013F);
// 	
// 	LCD_WRITE_COMMAND(0x200,0x0000);
// 	LCD_WRITE_COMMAND(0x201,0x0000);
// 	
// 	LCD_RS_L();
// 	LCD_WRITE_REG(0x202);	//RAM Write index
// 	LCD_CS_L();
// 	for(n=0;n<8;n++)
// 	{
// 		LCD_CS_L();
// 		LCD_RS_H();
// 		temp=color[n];
// 		for(num=40*240;num>0;num--)
// 		{
// 			LCD_WRITE_DATA(temp);
// 		}
// 	}

// 	delay_10us(60);

// 	//display pure color
// 	for(n=0;n<8;n++)
// 	{
// 	LCD_WRITE_COMMAND(0x210,0x00);
// 	LCD_WRITE_COMMAND(0x212,0x0000);
// 	LCD_WRITE_COMMAND(0x211,0xEF);
// 	LCD_WRITE_COMMAND(0x213,0x013F);
// 	
// 	LCD_WRITE_COMMAND(0x200,0x0000);
// 	LCD_WRITE_COMMAND(0x201,0x0000);
// 	
// 	LCD_RS_L();
// 	LCD_WRITE_REG(0x202);	//RAM Write index
// 	LCD_CS_L();

// 		LCD_CS_L();
// 		LCD_RS_H();

// 	    temp=color[n];
// 		for(i=0;i<240;i++)
// 		{
// 			for(num=0;num<320;num++)
// 			{
// 		  		LCD_WRITE_DATA(temp);
// 			}
// 		}
//         delay_10us(6);
//         delay_10us(6);
//         delay_10us(6);
// 	}
// 	
// 	
// 	LCD_CS_H();
// 	delay_10us(6);

// 	lcd_clear_screen(GREEN);

// }


void Swap(unsigned int *a , unsigned int *b)	//for BresenhamLine
{
	unsigned int tmp;
	tmp = *a ;
	*a = *b ;
	*b = tmp ;
} 

/******************************************************************************
* Function Name  : lcd_draw_dot
* Description    : draw dot
* Input          : color, x, y
* Output         : None
* Return         : None
******************************************************************************/
void lcd_draw_dot(
                  unsigned int x,
                  unsigned int y,
                  unsigned int color_front)
{
	//unsigned int i;

//  	y=239-y;
//  	x=319-x;	
// 	i=x;
// 	x=y;
// 	y=i;
	LCD_WRITE_COMMAND(0x210,x);
	LCD_WRITE_COMMAND(0x212,y);
	LCD_WRITE_COMMAND(0x211,x+1);
	LCD_WRITE_COMMAND(0x213,y+1);
	
 	LCD_SEND_COMMAND( 0x200, x );
 	LCD_SEND_COMMAND( 0x201, y );
// 	
// 	LCD_SEND_COMMAND( 0x202, color_front );
// 	LCD_SEND_COMMAND( 0x202, color_front );
	
	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();
	LCD_WRITE_DATA(color_front);
	LCD_WRITE_DATA(color_front);

}

/******************************************************************************
* Function Name  : lcd_draw_bigdot
* Description    : draw big dot,9 pix.
* Input          : color_frong, x, y
* Output         : None
* Return         : None
******************************************************************************/
void lcd_draw_bigdot(unsigned int   color_front,
                     unsigned int    x,
                     unsigned int    y )
{
    lcd_draw_dot(color_front,x,y);
    lcd_draw_dot(color_front,x,y+1);
    lcd_draw_dot(color_front,x,y-1);

    lcd_draw_dot(color_front,x+1,y);
    lcd_draw_dot(color_front,x+1,y+1);
    lcd_draw_dot(color_front,x+1,y-1);
    
    lcd_draw_dot(color_front,x-1,y);    
    lcd_draw_dot(color_front,x-1,y+1);
    lcd_draw_dot(color_front,x-1,y-1);
    
}

/******************************************************************************
* Function Name  : lcd_draw_line
* Description    : BresenhamLine
* Input          : c, x1, y1, x2, y2
* Output         : None
* Return         : None
******************************************************************************/
unsigned char lcd_draw_line(
                        unsigned int x0,
                        unsigned int y0,
                        unsigned int x1,
                        unsigned int y1,
												unsigned int color)
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
      lcd_draw_dot(x0, y0, color);
      y0++;
    }while( y1 >= y0 ); 
    return 0; 
  }
  if( dy == 0 )
  {
    do
    {
      lcd_draw_dot(x0, y0, color);
      x0++;
    }
    while( x1 >= x0 ); 
		return 0;
  }

	/* Bresenham's line algorithm  */
  if( dx > dy )
  {
    temp = 2 * dy - dx;
    while( x0 != x1 )
    {
	    lcd_draw_dot(x0,y0,color);
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
    lcd_draw_dot(x0,y0,color);
  }  
  else
  {
    temp = 2 * dx - dy;
    while( y0 != y1 )
    {
	 	  lcd_draw_dot(x0,y0,color);     
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
    lcd_draw_dot(x0,y0,color);
	}		
	return 0;
}



/**********************************************/
/* 函数功能；求m的n次方                       */
/**********************************************/
unsigned long mypow(unsigned char m,unsigned char n)
{
	unsigned long result=1;	 
	while(n--)result*=m;    
	return result;
}
/**********************************************/
/* 函数功能：显示数字                         */
/* 入口参数：x,y :起点坐标	 	              */
/*           num_len :数字的位数				  */
/*           num:数值(0~4294967295);	   	  */
/**********************************************/
void lcd_display_number(unsigned int x,
                        unsigned int y,
                        unsigned long num,
                        unsigned char num_len )
{         	
	unsigned char t,temp;
	unsigned char enshow=0;		 // 此变量用来去掉最高位的0	
				   
	for(t=0;t<num_len;t++)
	{
		temp=(num/mypow(10,num_len-t-1))%10;
		if(enshow==0&&t<(num_len-1))
		{
			if(temp==0)
			{
				//lcd_display_char(' ',BLACK,WHITE,x+8*t,y);
        //        LCD_DisplayStringLine(x+t,y,(uint8_t)' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	//lcd_display_char(temp+'0',BLACK,WHITE,x+8*t,y); 
    //    LCD_DisplayStringLine(x+t,y,(uint8_t)temp+'0'); 
	}
} 
/********************************end of file**************************************/


