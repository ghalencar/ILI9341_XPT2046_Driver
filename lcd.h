/*******************************************************************************************************/
//lcd.h
//author: Gleydson Henrique de Alencar Santos
//test hardware: Microcontroller TM4C123GH6PM 
//version: 1.0
//email: ghasantos@gmail.com
//Data: 28/11/2022
	
/********************************************************************************************************/
//////////////////////////////////////////////////// //////////////////////////////////
//This program is only for learning use, without the author's permission, it cannot be used for any other purpose
//Test hardware: MCU STM32F103RCT6, punctual atom MiniSTM32 development board, main frequency 72MHZ, crystal oscillator 12MHZ
//QDtech-TFT LCD driver for STM32 IO simulation
//xiaofeng@ShenZhen QDtech co.,LTD
//Company website: www.qdtft.com
//Taobao website: http://qdtech.taobao.com
//wiki technology website: http://www.lcdwiki.com
//Our company provides technical support, any technical problems are welcome to communicate and learn at any time
//Fixed telephone (fax):+86 0755-23594567
//Mobile phone: 15989313508 (Feng Gong)
//Email: lcdwiki01@gmail.com support@lcdwiki.com goodtft@163.com
//Technical support QQ: 3002773612 3002778157
//Technical exchange QQ group: 324828016
//Creation date: 2018/08/09
//Version: V1.0
//All rights reserved, piracy must be investigated.
//Copyright(C) Shenzhen Quandong Electronic Technology Co., Ltd. 2018-2028
//All rights reserved
/**************************************************** *****************************************************/
/***************************************************** ***************************************************/
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	

#ifndef _LCD_H__
#define _LCD_H__

#include "TM4C123GH6PM.h"



#define LCD_CS_HIGH   GPIOB->DATA |=  (1 << 5); // PB5 high
#define LCD_CS_LOW    GPIOB->DATA &= ~(1 << 5); // PB5 low 
#define LCD_DC_HIGH   GPIOD->DATA |=  (1 << 1); // PD1 high
#define LCD_DC_LOW    GPIOD->DATA &= ~(1 << 1); // PD1 low
#define LCD_RST_HIGH  GPIOB->DATA |=  (1 << 2); // PB2 high
#define LCD_RST_LOW   GPIOB->DATA &= ~(1 << 2); // PB2 low
#define LCD_LED_ON    GPIOD->DATA |=  (1 << 0); // PD0 high
#define LCD_LED_OFF   GPIOD->DATA &= ~(1 << 0); // PD0 low

#define LCD_W 240
#define LCD_H 320


#define WHITE        0xFFFF
#define BLACK        0x0000	  
#define BLUE         0x001F  
#define BRED         0XF81F
#define GRED 			   0XFFE0
#define GBLUE			   0X07FF
#define RED          0xF800
#define MAGENTA      0xF81F
#define GREEN        0x07E0
#define CYAN         0x7FFF
#define YELLOW       0xFFE0
#define BROWN 			 0XBC40 
#define BRRED 			 0XFC07 
#define GRAY  			 0X8430 

#define DARKBLUE     0X01CF	
#define LIGHTBLUE    0X7D7C	  
#define GRAYBLUE     0X5458 

#define LIGHTGREEN   0X841F 
#define LIGHTGRAY    0XEF5B 
#define LGRAY 			 0XC618 

#define LGRAYBLUE    0XA651 
#define LBBLUE       0X2B12 


typedef struct 
{
	uint16_t width;
	uint16_t height;
	uint16_t id;
	uint8_t  dir;
	uint16_t wramcmd;
	uint16_t setxcmd;
	uint16_t setycmd;

}LCD_DEV;

extern LCD_DEV lcd_dev;

void LCD_GPIO_Init(void);
void LCD_Init(void);
void LCD_WR_Command(uint8_t data);
void LCD_WR_Data(uint8_t data);
void LCD_RESET(void);
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
void LCD_Direction(uint8_t direction);
void LCD_Clear(uint16_t Color);
void LCD_WriteRAM_Prepare(void);
void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_WriteData_16Bit(uint16_t Data);
void LCD_DrawPoint(uint16_t x,uint16_t y);
void LCD_Test(void);
#endif