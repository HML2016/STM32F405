/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : Jian-guo JIANG
* Date First Issued  : 2008.5.14
* Description        : lcd display
*
********************************************************************************
* History:
* 2008.5.14 : Version 1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "lcd.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u16  frame[4];    /* LCD frame buffer  */
u16  digit[4];    /* Digit frame buffer */  
char text[4];

/*the varitronix LCD digit is:

               A
      --  ----------
    X \/  |\   |I  /|
         F| H  |  J |B
          |  \ | /  |
          --G-- --K--
          |   /| \  |
        E |  L |  N |C
          | /  |M  \|   _
          -----------  | |DP   
              D         -

        PE0  PE1  PE2  PE3  ...................................................... PE15
----------------------------------------------------------------------------------------
|      | S0 | S1 | S2 | S3 | S4 | S5 | S6 | S7 | S8 | S9 | S10| S11| S12| S13| S14| S15|
----------------------------------------------------------------------------------------
| COM1 | 1X | 1I | 1A | 1DP| 2X | 2I | 2A | 2DP| 3X | 3I | 3A | 3DP| 4X | 4I | 4A | 4DP|
----------------------------------------------------------------------------------------
| COM2 | 1F | 1H | 1J | 1B | 2F | 2H | 2J | 2B | 3F | 3H | 3J | 3B | 4F | 4H | 4J | 4B |
----------------------------------------------------------------------------------------
| COM3 | 1E | 1G | 1K | 1C | 2E | 2G | 2K | 2C | 3E | 3G | 3K | 3C | 4E | 4G | 4K | 4C |
----------------------------------------------------------------------------------------
| COM4 | 1L | 1M | 1N | 1D | 2L | 2M | 2N | 2D | 3L | 3M | 3N | 3D | 4L | 4M | 4N | 4D |
----------------------------------------------------------------------------------------

A LCD character coding is based on the following matrix:

{ X , F , E , L  }
{ I , H , G , M  }
{ A , J , K , N  }
{ DP, B , C , D  }

The characher A for example is:

 { 0 , 1 , 1 , 0 }
 { 0 , 0 , 1 , 0 }
 { 1 , 0 , 1 , 0 }
 { 0 , 1 , 1 , 0 }
-------------------
=  4   9   F   0   hex

=> 'A' = 0x4E70  */

const u16 letter[26]={0x49F0,0x01F8,0x4118,0x08F8,0x4178,0x4170,0x41D8,0x09F0,0x600A,
		      0x0888,0x0534,0x0118,0x0F90,0x0B94,0x4998,0x4970,0x499C,0x4974,
		      0x41E8,0x6002,0x0998,0x0511,0x299A,0x0605,0x0601,0x4409};

const u16 number[10]={0x4998,0x0880,0x4878,0x48E8,0x09E0,0x41E8,0x41F8,0x4880,0x49F8,0x49E8};

const u16 sign[2]={0x2062,0x0060}; // {+,-}

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : convert
* Description    : converts an ascii char to the a LCD digit (previous coding)
* Input1         : char
* Input2         : point : flag indicating if a point has to be add in front of
*                  displayed character (0: no, 1: yes)
* Output         : None
* Return         : None
*******************************************************************************/
void convert (char* c,u8 point)
{ 
  u16 car=0,tmp;
  u8 i;
  const u16 mask[4]={0xF000,0x0F00,0x00F0,0x000F};
  const u8 shift[3]={0xC,0x8,0x4};

  if ((*c<0x5B)&(*c>0x40))  // 'A' --- 'F'
    car = letter[*c-0x41]; 
  else if ((*c<0x3A)&(*c>0x2F))  // '0' --- '9'
    car = number[*c-0x30];
  else if (*c==0x20) car =0;     // 'space'
  else if (*c=='+')  car = sign[0];   // '+'     
  else if (*c=='-')  car = sign[1];   // '-'  
  
  if (point==1) car|=0x8000;
  
  for (i=0;i<3;i++)
  {
    tmp = car&mask[i];
    digit[i] = tmp>>shift[i];
  }
  digit[3] = (car&mask[3]); 
}

/*******************************************************************************
* Function Name  : write char
* Description    : This function writes a char in the LCD frame buffer
* Input1         : char ascii value
* Input2         : point 0: no point to add, 1 a point to add in front of char 
* Input2         : pos: position of the caracter to write [0:3]
* Output         : None
* Return         : None
*******************************************************************************/
void write_char(char* car,u8 point,u8 pos)
{
   int i;
   const u16 mask[4]={0x0F,0xF0,0xF00,0xF000};

   convert(car,point);
   
   if (pos == 1) for (i=0;i<4;i++) digit[i]=(digit[i]<<4);
   if (pos == 2) for (i=0;i<4;i++) digit[i]=(digit[i]<<8); 
   if (pos == 3) for (i=0;i<4;i++) digit[i]=(digit[i]<<12); 
   
   for (i=0;i<4;i++) frame[i]= (frame[i]&~mask[pos])|digit[i];   
}


/*******************************************************************************
* Function Name  : write_string
* Description    : This function writes a string in the LCD
* Input          : string
* Output         : None
* Return         : None
*******************************************************************************/
void write_string(char* str)
{
  int i;
  for (i=0;i<4;i++) write_char(str+i,0,i);
}
 
/*******************************************************************************
* Function Name  : int2char
* Description    : This function convertes a int to a string
* Input          : u16 value
* Output         : None
* Return         : None
*******************************************************************************/
char* int2char(u16 value)
{ 
  u8 i;
  u8 bValue;
  u16 temp;  
  
  temp = 1000;  
  for (i=0;i<4;i++)
  {
    bValue = (u8)(value/temp);
    text[i] = (char)(bValue+0x30);
    
    value %= temp;
    temp /= 10;                
  }    
  return(text);
}


/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
