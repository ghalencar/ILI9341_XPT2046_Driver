/*******************************************************************************************************/
//systick.h
//author: Gleydson Henrique de Alencar Santos
//test hardware: Microcontroller TM4C123GH6PM 
//version: 1.0
//email: ghasantos@gmail.com
//Data: 28/11/2022
	
/********************************************************************************************************/


#ifndef __delay_H__
#define __delay_H__

#include <stdint.h>

void delay_Init(void);
void Systick_Wait(uint32_t delay);
void delay_ms(uint32_t delay);
void delay_us(uint32_t delay);
#endif
