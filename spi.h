/*******************************************************************************************************/
//spi.h
//author: Gleydson Henrique de Alencar Santos
//test hardware: Microcontroller TM4C123GH6PM 
//version: 1.0
//email: ghasantos@gmail.com
//Data: 28/11/2022
	
/********************************************************************************************************/

#ifndef _SPI_H__
#define _SPI_H__


void SPI_Init(void);
unsigned char SPI_Write_Byte(unsigned char data);

#endif