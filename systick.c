/*******************************************************************************************************/
//systick.c
//author: Gleydson Henrique de Alencar Santos
//test hardware: Microcontroller TM4C123GH6PM 
//version: 1.0
//email: ghasantos@gmail.com
//Data: 28/11/2022
	
/********************************************************************************************************/

#include <stdint.h>
#include "TM4C123GH6PM.h"
#include "Systick.h"


void SysTick_Init(void) 
{
	SysTick->CTRL = 0; 
	SysTick->LOAD = 0x00FFFFFF; 
	SysTick->VAL = 0; 
	SysTick->CTRL = 0x00000005;
}

void SysTick_Wait(uint32_t delay)
{
  volatile uint32_t elapsedTime;
  uint32_t startTime = SysTick->VAL;
	
	do
	{
		elapsedTime = (startTime - SysTick->VAL) & 0x00FFFFFF;
	}
	while(elapsedTime <= delay);
}
// Assuming 50 MHz system clock
void delay_ms(uint32_t delay) 
{
	uint32_t i;
	for(i=0; i<delay; i++) 
	{
		SysTick_Wait(50000); 
	}
}

void delay_us(uint32_t delay) 
{
	uint32_t i;
	for(i=0; i<delay; i++) 
	{
		SysTick_Wait(50); 
	}
}
