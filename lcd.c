/*******************************************************************************************************/
//lcd.c
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


#include "lcd.h"
#include "spi.h"
#include "systick.h"


LCD_DEV lcd_dev;

uint16_t POINT_COLOR = 0x0000, BACK_COLOR = 0xFFFF;  
uint16_t DeviceCode;	 

void LCD_GPIO_Init(void)
{
	SYSCTL->RCGCGPIO |= (1 << 3);//Enable clock PortD
	GPIOD->AMSEL |= 0x00; //Disable analogic function PortD
	
	//Configure pins  PD1, PD2, PB2, PB5 digital and output
	GPIOB->DEN |= 0x24;
	GPIOB->DIR |= 0x24;
	GPIOD->DEN |= 0x03;
  GPIOD->DIR |= 0x03;
	//init CS HIGH
	GPIOB->DATA |= (1 << 5);
}

void LCD_WR_Command(uint8_t data)
{
	LCD_CS_LOW;
	LCD_DC_LOW;
	SPI_Write_Byte(data);
	LCD_CS_HIGH;
}

void LCD_WR_Data(uint8_t data)
{
	LCD_CS_LOW;
  LCD_DC_HIGH;
	SPI_Write_Byte(data);
	LCD_CS_HIGH;
}

void LCD_RESET(void)
{
	LCD_RST_LOW;
	delay_ms(100);	
	LCD_RST_HIGH;
	delay_ms(50);
}

void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
	LCD_WR_Command(LCD_Reg);  
	LCD_WR_Data(LCD_RegValue);	
}

void LCD_WriteData_16Bit(uint16_t Data)
{	
   LCD_CS_LOW;
   LCD_DC_HIGH;  
   SPI_Write_Byte(Data>>8);
	 SPI_Write_Byte(Data);
   LCD_CS_HIGH;
}

void LCD_Direction(uint8_t direction)
{ 
			lcd_dev.setxcmd=0x2A;
			lcd_dev.setycmd=0x2B;
		  lcd_dev.wramcmd=0x2C;
	switch(direction)
	{		  
		case 0:						 	 		
			lcd_dev.width=LCD_W;
			lcd_dev.height=LCD_H;		
			LCD_WriteReg(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		
		case 1:
			lcd_dev.width=LCD_H;
			lcd_dev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		
		case 2:						 	 		
			lcd_dev.width=LCD_W;
			lcd_dev.height=LCD_H;	
			LCD_WriteReg(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		
		case 3:
			lcd_dev.width=LCD_H;
			lcd_dev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		
		default:
			break;
	}		
}	 

void LCD_WriteRAM_Prepare(void)
{
  LCD_WR_Command(lcd_dev.wramcmd);
}	 

void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{	
	LCD_WR_Command(lcd_dev.setxcmd);	
	LCD_WR_Data(xStar>>8);
	LCD_WR_Data(0x00FF&xStar);		
	LCD_WR_Data(xEnd>>8);
	LCD_WR_Data(0x00FF&xEnd);

	LCD_WR_Command(lcd_dev.setycmd);	
	LCD_WR_Data(yStar>>8);
	LCD_WR_Data(0x00FF&yStar);		
	LCD_WR_Data(yEnd>>8);
	LCD_WR_Data(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();			
} 

void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y);//设置光标位置 
	LCD_WriteData_16Bit(BLACK); 
}

void LCD_Clear(uint16_t Color)
{
  unsigned int i,m;  
	LCD_SetWindows(0,0,lcd_dev.width-1,lcd_dev.height-1);   
	LCD_CS_LOW;
	LCD_DC_HIGH;
	for(i=0;i<lcd_dev.height;i++)
	{
    for(m=0;m<lcd_dev.width;m++)
    {	
			LCD_WriteData_16Bit(Color);
		}
	}
	 LCD_CS_HIGH;
} 

void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

void LCD_Test(void)
{
  LCD_Clear(BLUE);
	delay_ms(500);
	LCD_Clear(GREEN);
	delay_ms(500);
	LCD_Clear(RED);
	delay_ms(500);
	LCD_Clear(MAGENTA);
	delay_ms(500);
	LCD_Clear(CYAN);
	delay_ms(500);
	LCD_Clear(YELLOW);
	delay_ms(500);
	LCD_Clear(BROWN);
	delay_ms(500);
	LCD_Clear(GRAY);
	delay_ms(500);
	LCD_Clear(WHITE);
	delay_ms(500);
}


void LCD_Init(void)
{
	SPI_Init();
	LCD_GPIO_Init();
	LCD_RESET();
	
	LCD_WR_Command(0xCF);  
	LCD_WR_Data(0x00); 
	LCD_WR_Data(0xD9); 
	LCD_WR_Data(0X30); 
	LCD_WR_Command(0xED);  
	LCD_WR_Data(0x64); 
	LCD_WR_Data(0x03); 
	LCD_WR_Data(0X12); 
	LCD_WR_Data(0X81); 
	LCD_WR_Command(0xE8);  
	LCD_WR_Data(0x85); 
	LCD_WR_Data(0x10); 
	LCD_WR_Data(0x7A); 
	LCD_WR_Command(0xCB);  
	LCD_WR_Data(0x39); 
	LCD_WR_Data(0x2C); 
	LCD_WR_Data(0x00); 
	LCD_WR_Data(0x34); 
	LCD_WR_Data(0x02); 
	LCD_WR_Command(0xF7);  
	LCD_WR_Data(0x20); 
  LCD_WR_Command(0xEA);  
	LCD_WR_Data(0x00); 
	LCD_WR_Data(0x00); 
	LCD_WR_Command(0xC0); //Power control 
	LCD_WR_Data(0x1B);   //VRH[5:0] 
	LCD_WR_Command(0xC1); //Power control 
	LCD_WR_Data(0x12);   //SAP[2:0];BT[3:0] //0x01
	LCD_WR_Command(0xC5);    //VCM control 
	LCD_WR_Data(0x26); 	 //3F
	LCD_WR_Data(0x26); 	 //3C
	LCD_WR_Command(0xC7);    //VCM control2 
	LCD_WR_Data(0XB0); 
	LCD_WR_Command(0x36); // Memory Access Control 
	LCD_WR_Data(0x08); 
	LCD_WR_Command(0x3A);   
	LCD_WR_Data(0x55); 
	LCD_WR_Command(0xB1);   
	LCD_WR_Data(0x00);   
	LCD_WR_Data(0x1A); 
	LCD_WR_Command(0xB6); // Display Function Control 
	LCD_WR_Data(0x0A); 
	LCD_WR_Data(0xA2); 
	LCD_WR_Command(0xF2); // 3Gamma Function Disable 
	LCD_WR_Data(0x00); 
	LCD_WR_Command(0x26); //Gamma curve selected 
	LCD_WR_Data(0x01); 
	LCD_WR_Command(0xE0); //Set Gamma
	LCD_WR_Data(0x1F);
	LCD_WR_Data(0x24);
	LCD_WR_Data(0x24);
	LCD_WR_Data(0x0D);
	LCD_WR_Data(0x12);
	LCD_WR_Data(0x09);
	LCD_WR_Data(0x52);
	LCD_WR_Data(0xB7);
	LCD_WR_Data(0x3F);
	LCD_WR_Data(0x0C);
	LCD_WR_Data(0x15);
	LCD_WR_Data(0x06);
	LCD_WR_Data(0x0E);
	LCD_WR_Data(0x08);
	LCD_WR_Data(0x00);
	LCD_WR_Command(0XE1); //Set Gamma
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x1B);
	LCD_WR_Data(0x1B);
	LCD_WR_Data(0x02);
	LCD_WR_Data(0x0E);
	LCD_WR_Data(0x06);
	LCD_WR_Data(0x2E);
	LCD_WR_Data(0x48);
	LCD_WR_Data(0x3F);
	LCD_WR_Data(0x03);
	LCD_WR_Data(0x0A);
	LCD_WR_Data(0x09);
	LCD_WR_Data(0x31);
	LCD_WR_Data(0x37);
	LCD_WR_Data(0x1F);

	LCD_WR_Command(0x2B); 
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x01);
	LCD_WR_Data(0x3F);
	LCD_WR_Command(0x2A); 
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x00);
	LCD_WR_Data(0xef);	 
	LCD_WR_Command(0x11); //Exit Sleep
	delay_ms(120);
	LCD_WR_Command(0x29); //display on		
  LCD_Direction(0);
	LCD_LED_ON; // led display 
	
}
