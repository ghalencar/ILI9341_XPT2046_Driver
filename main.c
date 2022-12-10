/*******************************************************************************************************/
//main.c
//author: Gleydson Henrique de Alencar Santos
//test hardware: Microcontroller TM4C123GH6PM 
//version: 1.0
//email: ghasantos@gmail.com
//Data: 28/11/2022

// connecting the wires


//TOUCH XPT2046
 
//PA6  connect to T_IRQ    I
//PA4  connect to T_DO     I
//PA5  connect to T_DIN    O
//PA3  connect to T_CS     O
//PA2  connect to T_CLK    O
 
//DISPLAY TFT ILI9341
 
//PD0  connect to LED  
//PD1  connect to DC
//PB2  connect to RESET  
//PB4  connect to SCK   
//PB5  connect to CS
//PB6  connect to MISO		
//PB7  connect to MOSI
	
/********************************************************************************************************/


#include "systick.h"
#include "lcd.h"
#include "touch.h"


int main(void)
{
	SysTick_Init();
	LCD_Init();
	Touch_Init();
	
	//while(1)
	//{	
		LCD_Test();
		Touch_Test();
  //}
	
}