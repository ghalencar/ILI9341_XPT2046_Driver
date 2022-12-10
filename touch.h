/*******************************************************************************************************/
//touch.h
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


#ifndef _TOUCH_H_
#define _TOUCH_H_


#include "TM4C123GH6PM.h"


//IN pins definition
#define TP_IRQ       (*((volatile uint32_t *) 0x40004100))  
#define TP_DOUT      (*((volatile uint32_t *) 0x40004040))

// OUT pins definition
#define TP_DIN_HIGH  ((*((volatile uint32_t *) 0x40004080)) = 0xFF)
#define TP_DIN_LOW   ((*((volatile uint32_t *) 0x40004080)) = 0x00)
#define TP_CS_HIGH   ((*((volatile uint32_t *) 0x40004020)) = 0xFF)
#define TP_CS_LOW    ((*((volatile uint32_t *) 0x40004020)) = 0x00) 	
#define TP_CLK_HIGH  ((*((volatile uint32_t *) 0x40004010)) = 0xFF)
#define TP_CLK_LOW   ((*((volatile uint32_t *) 0x40004010)) = 0x00)

// Endereços Coordenadas
#define READ_X  0xD0
#define READ_Y  0x90

#define ERR_RANGE 50

#define READ_TIMES 5 
#define LOST_VAL   1

#define TP_PRES_DOWN 0x80  //tela sens¨ªvel ao toque ¨¦ pressionada
#define TP_CATH_PRES 0x40  //Um bot?o foi pressionado	  

typedef struct
{
	uint8_t (*init)(void); // control touch init
	uint8_t (*scan)(uint8_t); // touch scan
	void (*adjust) (void); // touch calibration
	uint16_t x0;
	uint16_t y0;
	uint16_t x;
	uint16_t y;
	uint8_t sta;
	
	float xfac;
	float yfac;
	float xoff;
	float yoff;
	
	uint8_t touchtype;
		
}_m_tp_dev;

extern _m_tp_dev tp_dev;


void Touch_Write_Byte(uint8_t num);
uint16_t Touch_Read_AD(uint8_t cmd);
uint16_t Touch_Read_XY(uint8_t xy);
uint8_t TP_Read_XY(uint16_t *x, uint16_t *y);
uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y);
uint8_t Touch_Init(void);
void Touch_Test(void);
void Touch_Adjust(void);
void Touch_Draw_Point(uint16_t x,uint16_t y,uint16_t color);
void Touch_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color);
void Touch_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac);
uint8_t Touch_Scan(uint8_t tp);




#endif