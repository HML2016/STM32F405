#ifndef __LCD22_H__
#define __LCD22_H__

#include "stm32f4xx.h"
#include "AsciiLib.h"

#define LCD_RST_H()		GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define LCD_RST_L()		GPIO_ResetBits(GPIOB,GPIO_Pin_9)

#define LCD_RS_H()		GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define LCD_RS_L()		GPIO_ResetBits(GPIOB,GPIO_Pin_8)


#define LCD_CS_H()		GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define LCD_CS_L()		GPIO_ResetBits(GPIOB,GPIO_Pin_7)


#define  MAX_X  (240)
#define  MAX_Y  (320)  

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#define RGB565CONVERT(red, green, blue) (int) (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))
unsigned char SPI_Communication(unsigned char send_char);
/* Private function prototypes -----------------------------------------------*/
void LCD_Initializtion(void);
void LCD_Clear(uint16_t Color);	
//void LCD_SetBacklight(uint8_t intensity);
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos);
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
void PutChar(uint16_t Xpos,uint16_t Ypos,uint8_t c,uint16_t charColor,uint16_t bkColor);
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void PutChinese(uint16_t Xpos,uint16_t Ypos,uint8_t *str,uint16_t Color,uint16_t bkColor); 
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);
void GUI_Chinese(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);	
void LCD_DrawPicture(uint16_t StartX,uint16_t StartY,uint16_t EndX,uint16_t EndY,uint16_t *pic);
void LCD_Drawcircle(int x0, int y0, int r, int color);
void DrawGrids(void);
__inline void LCD_WriteIndex(uint16_t index);
__inline void LCD_WriteData(uint16_t data);
__inline uint16_t LCD_ReadData(void);
__inline uint16_t LCD_ReadReg(uint16_t LCD_Reg);
__inline void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue);
static void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos );

#endif 
