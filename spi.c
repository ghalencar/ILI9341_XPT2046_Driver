/*******************************************************************************************************/
//spi.c
//author: Gleydson Henrique de Alencar Santos
//test hardware: Microcontroller TM4C123GH6PM 
//version: 1.0
//email: ghasantos@gmail.com
//Data: 28/11/2022
	
/********************************************************************************************************/

#include "spi.h" 
#include "TM4C123GH6PM.h"
  
	
void SPI_Init(void)
{	
	SYSCTL->RCGCSSI |= (1 << 2);//Enable the SPI2 peripheral
	SYSCTL->RCGCGPIO |= (1 << 1);//Enable PORTB clock 
	GPIOB->AMSEL |= 0x00; //Disable analogic function PORTB
	GPIOB->DEN |= 0xD0; // Enable digital function PB4, PB6 e PB7 pins
	GPIOB->AFSEL |= 0xD0;//Enable alternative function PB4, PB6 e PB7 pins
	GPIOB->PCTL &= ~0xFF0F0000; //Function Alternative reset
	GPIOB->PCTL |= 0x22020000; //configure pins MOSI, MISO, CLK 
	
	//SPI Clock = SysClk/(CPSDVSR * (1 + SCR)) 
	
	SSI2->CR1 = 0x00; //disable SPI2 and configure master 
	SSI2->CC = 0x00;  //Use System Clock
	SSI2->CPSR = 4; // 200MHz : 4 = 50MHz Clock prescaler
	SSI2->CR0 |= 0x00000007; //SCR = 0; CPHA = 0; CPOL = 0; 8 bits data;
	SSI2->CR1 |= 0x02; //enable SPI2
}

unsigned char SPI_Write_Byte(unsigned char data)
{
	while((SSI2->SR & 0x02) == 0);// wait until the value is not in the FIFO
	SSI2->DR = data; // transmit
	while(SSI2->SR & 0x10); //wait end transmission
	return SSI2->DR; // return byte transmitted	
}






