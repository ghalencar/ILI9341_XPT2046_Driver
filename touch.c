/*******************************************************************************************************/
//touch.c
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


#include "touch.h"
#include "systick.h"
#include "lcd.h"
#include "gui.h"
#include "math.h"
#include "stdlib.h"

extern uint16_t POINT_COLOR, BACK_COLOR;

_m_tp_dev tp_dev =
{
	Touch_Init,
	Touch_Scan,
	Touch_Adjust,
	0,
	0,
 	0,
	0,
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};				

uint8_t CMD_RDX=0xD0;
uint8_t CMD_RDY=0x90;


void Touch_Write_Byte(uint8_t num)
{
	uint8_t count = 0;
	
	for(;count<8;count++)
	{
		if(num & 0x80)TP_DIN_HIGH;
		else TP_DIN_LOW;
		num <<= 1;
		TP_CLK_LOW;
		TP_CLK_HIGH;
	}
}


uint16_t Touch_Read_AD(uint8_t cmd)
{
	uint8_t count = 0;
	uint16_t Num = 0;
	TP_CLK_LOW;  // pull the clock low first
	TP_DIN_LOW;  // Pull the data line low
	TP_CS_LOW;   //Select touch screen 
	
	Touch_Write_Byte(cmd); //send command word
	delay_us(6);
  TP_CLK_LOW;  // Falling edge valid
  delay_us(1);
  TP_CLK_HIGH;
	TP_CLK_LOW;
	
	for(;count < 16; count++) // Read 16-bit data, only the upper 12 bits are valid
	{
		Num<<=1;
		TP_CLK_LOW; // Falling edge valid
	  TP_CLK_HIGH;
		if(TP_DOUT)
		  Num++;	
	}
	Num>>=4; // Only the upper 12 bits are valid
	TP_CS_HIGH; // release chip select
	return(Num);
	
}

uint16_t Touch_Read_XY(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	
	for(i=0;i < READ_TIMES; i++)
	  buf[i] = Touch_Read_AD(xy);
	
	for(i=0;i < READ_TIMES-1; i++)
	{
		for(j=i;j< READ_TIMES; j++)
		{
			if(buf[i] > buf[j])
			{
				temp=buf[i];
				buf[i] = buf[j];
				buf[j] = temp;			
			}
		}
	}
	
	sum=0;
	
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;
}


uint8_t TP_Read_XY(uint16_t *x,uint16_t *y)
{
	uint16_t xtemp, ytemp;
	xtemp = Touch_Read_XY(READ_X);
	ytemp = Touch_Read_XY(READ_Y);

  *x=xtemp;
	*y=ytemp;
	
	return 1;

}

uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y)
{
	uint16_t x1,y1;
 	uint16_t x2,y2;
 	uint8_t flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//The two samples before and after are within +-50
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	

}

void Touch_Draw_Point(uint16_t x,uint16_t y,uint16_t color)
{
	POINT_COLOR=color;
	LCD_DrawLine(x-12,y,x+13,y);//horizontal line
	LCD_DrawLine(x,y-12,x,y+13);//vertical line
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
	gui_circle(x,y,POINT_COLOR,6,0);//draw center circle

}	

void Touch_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color)
{	    
	POINT_COLOR=color;
	LCD_DrawPoint(x,y);//center point 
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}	

uint8_t Touch_Scan(uint8_t tp)
{			   
	if(TP_IRQ==0)//There is a button pressed
	{
  		if(tp)TP_Read_XY2(&tp_dev.x,&tp_dev.y);//read physical coordinates
		else if(TP_Read_XY2(&tp_dev.x,&tp_dev.y))//read screen coordinates
		{
	 		tp_dev.x=tp_dev.xfac*tp_dev.x+tp_dev.xoff;//Convert the result to screen coordinates
			tp_dev.y=tp_dev.yfac*tp_dev.y+tp_dev.yoff;  
	 	} 
		if((tp_dev.sta&TP_PRES_DOWN)==0)//was not pressed before
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;//button pressed  
			tp_dev.x0=tp_dev.x;//Record the coordinates of the first press
			tp_dev.y0=tp_dev.y;  	   			 
		}			   
	}else
	{
		if(tp_dev.sta&TP_PRES_DOWN)//was pressed before
		{
			tp_dev.sta&=~(1<<7);//Mark key released	
		}else//was not pressed before
		{
			tp_dev.x0=0;
			tp_dev.y0=0;
			tp_dev.x=0xffff;
			tp_dev.y=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN;//Returns the current touch screen state
}

void Touch_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac)
{	  
	POINT_COLOR=RED;
	LCD_ShowString(40,140,16,(uint8_t *)"x1:",1);
 	LCD_ShowString(40+80,140,16,(uint8_t *)"y1:",1);
 	LCD_ShowString(40,160,16,(uint8_t *)"x2:",1);
 	LCD_ShowString(40+80,160, 16,(uint8_t *)"y2:",1);
	LCD_ShowString(40,180, 16,(uint8_t *)"x3:",1);
 	LCD_ShowString(40+80,180, 16,(uint8_t *)"y3:",1);
	LCD_ShowString(40,200, 16,(uint8_t *)"x4:",1);
 	LCD_ShowString(40+80,200, 16,(uint8_t *)"y4:",1);  
 	LCD_ShowString(40,220, 16,(uint8_t *)"fac is:",1);     
	LCD_ShowNum(40+24,140,x0,4,16);		//display value
	LCD_ShowNum(40+24+80,140,y0,4,16);	
	LCD_ShowNum(40+24,160,x1,4,16);		
	LCD_ShowNum(40+24+80,160,y1,4,16);	
	LCD_ShowNum(40+24,180,x2,4,16);		
	LCD_ShowNum(40+24+80,180,y2,4,16);	
	LCD_ShowNum(40+24,200,x3,4,16);		
	LCD_ShowNum(40+24+80,200,y3,4,16);	
 	LCD_ShowNum(40+56,220,fac,3,16); 	//Display the value, the value must be within the range of 95~105.
}

void Touch_Adjust(void)
{								 
	uint16_t pos_temp[4][2];//Coordinate cache value
	uint8_t  cnt=0;	
	uint16_t d1,d2;
	uint32_t tem1,tem2;
	float fac; 	
	uint16_t outtime=0;
 	cnt=0;				
	POINT_COLOR=BLUE;
	BACK_COLOR =WHITE;
	LCD_Clear(WHITE);//clear screen   
	POINT_COLOR=RED;//red 
	LCD_Clear(WHITE);//clear screen 	   
	POINT_COLOR=BLACK;
	LCD_ShowString(10,40,16,(uint8_t*)"Please use the stylus click",1);//Show hints
	LCD_ShowString(10,56,16,(uint8_t*)"the cross on the screen.",1);
	LCD_ShowString(10,72,16,(uint8_t*)"The cross will always move",1);
	LCD_ShowString(10,88,16,(uint8_t*)"until the screen adjustment",1);
  LCD_ShowString(10,104,16,(uint8_t*)"is completed.",1);
	 
	Touch_Draw_Point(20,20,RED);//draw point 1(uint8_t*)
	tp_dev.sta=0;//Eliminate trigger signal 
	tp_dev.xfac=0;//xfacIt is used to mark whether it has been calibrated, so it must be cleared before calibration! In order to avoid mistakes	 
	while(1)
	{
		tp_dev.scan(1);//scan physical coordinates
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)//The button is pressed once (the button is released at this time.)
		{	
			outtime=0;		
			tp_dev.sta&=~(1<<6);//Indicates that the button has been processed.
						   			   
			pos_temp[cnt][0]=tp_dev.x;
			pos_temp[cnt][1]=tp_dev.y;
			cnt++;	  
			switch(cnt)
			{			   
				case 1:						 
					Touch_Draw_Point(20,20,WHITE);				//clear point 1 
					Touch_Draw_Point(lcd_dev.width-20,20,RED);	//draw point 2
					break;
				case 2:
 					Touch_Draw_Point(lcd_dev.width-20,20,WHITE);	//clear point 2
					Touch_Draw_Point(20,lcd_dev.height-20,RED);	//draw point 3
					break;
				case 3:
 					Touch_Draw_Point(20,lcd_dev.height-20,WHITE);			//clear point3
 					Touch_Draw_Point(lcd_dev.width-20,lcd_dev.height-20,RED);	//draw point 4
					break;
				case 4:	 //All four points have been obtained
	    		    //Opposite sides are equal
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//get a distance of 1,2
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//get a distance of 3,4
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)//unqualified
					{
						cnt=0;
 				    	Touch_Draw_Point(lcd_dev.width-20,lcd_dev.height-20,WHITE);	//clear point 4
   	 					Touch_Draw_Point(20,20,RED);								//draw point 1
 						Touch_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//显示数据   
 						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//get a distance of 1,3
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//get a distance of 2,4
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//unqualified
					{
						cnt=0;
 				    	Touch_Draw_Point(lcd_dev.width-20,lcd_dev.height-20,WHITE);	// clear point 4
   	 					Touch_Draw_Point(20,20,RED);								//draw point 1
 						Touch_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//显示数据   
						continue;
					}//correct
								   
					//Diagonals are equal
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//get a distance of 1,4
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//get a distance of 2,3
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//unqualified
					{
						cnt=0;
 				    	Touch_Draw_Point(lcd_dev.width-20,lcd_dev.height-20,WHITE);	//clear point 4
   	 					Touch_Draw_Point(20,20,RED);								//draw point 1
 						Touch_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//显示数据   
						continue;
					}//correct
					//Calculation results
					tp_dev.xfac=(float)(lcd_dev.width-40)/(pos_temp[1][0]-pos_temp[0][0]);//get xfac		 
					tp_dev.xoff=(lcd_dev.width-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//get xoff
						  
					tp_dev.yfac=(float)(lcd_dev.height-40)/(pos_temp[2][1]-pos_temp[0][1]);//get yfac
					tp_dev.yoff=(lcd_dev.height-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//get yoff  
					if(abs(tp_dev.xfac)>2||abs(tp_dev.yfac)>2)//The touch screen is the opposite of the default.
					{
						cnt=0;
 				    	Touch_Draw_Point(lcd_dev.width-20,lcd_dev.height-20,WHITE);	//clear point 4
   	 					Touch_Draw_Point(20,20,RED);								//draw point 1
						LCD_ShowString(40,26, 16,(uint8_t*)"TP Need readjust!",1);
						tp_dev.touchtype=!tp_dev.touchtype;//Modify touch screen type.
						if(tp_dev.touchtype)//X,YOrientation is opposite to the screen
						{
							CMD_RDX=0X90;
							CMD_RDY=0XD0;	 
						}else				   //X,Y same orientation as screen
						{
							CMD_RDX=0XD0;
							CMD_RDY=0X90;	 
						}			    
						continue;
					}		
					POINT_COLOR=BLUE;
					LCD_Clear(WHITE);
					LCD_ShowString(35,110, 16,(uint8_t*)"Touch Screen Adjust OK!",1);//Calibration completed
					delay_ms(1000);
					
 					LCD_Clear(WHITE); 
					return;//Calibration completed				 
			}
		}
		/*
		delay_ms(10);
		
		if(++outtime>1000)
		{
		  LCD_Clear(WHITE);
		  break;
	 	} 
		*/
 	}
}		

void Touch_Test(void)
{
	uint8_t key = 0;
	uint16_t i=0;
	uint16_t j=0;
	uint16_t colorTemp=0;
	
	while(1)
	{
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//touch screen is pressed
		{	
		 	if(tp_dev.x<lcd_dev.width&&tp_dev.y<lcd_dev.height)
			{	
				if((tp_dev.x> lcd_dev.width-52) && (tp_dev.x < lcd_dev.width-30) && (tp_dev.y<16)) //24 e 16
				{
					LCD_Clear(WHITE);
					LCD_ShowString(lcd_dev.width-24,0,16,(uint8_t*)"RST",1);//Show clear area
					POINT_COLOR=RED;
					LCD_Fill(lcd_dev.width-52,2,lcd_dev.width-50+20,18,POINT_COLOR); 
					
				}
				else if((tp_dev.x>(lcd_dev.width-60)&&tp_dev.x<(lcd_dev.width-50+20))&&tp_dev.y<20)
				{
					colorTemp=POINT_COLOR;
					delay_ms(10);
				}

				else Touch_Draw_Big_Point(tp_dev.x,tp_dev.y,POINT_COLOR); //drawing	  			   
			}
		}else delay_ms(10);	//when no key is pressed 
		
		if(key == 0)
		{
			LCD_Clear(WHITE);
			tp_dev.adjust();
			LCD_ShowString(lcd_dev.width-24,0,16,(uint8_t*)"RST",1);//Show clear area
			POINT_COLOR=RED;
			LCD_Fill(lcd_dev.width-52,2,lcd_dev.width-50+20,18,RED);

			key = 1;
    }			
	}   
}


uint8_t Touch_Init(void)
{
	SYSCTL->RCGCGPIO |= 0x01; // turn-on clock PORTA
	while(!(SYSCTL->PRGPIO & 0x01)){} // wait enable clock PORTA
  GPIOA->AMSEL = 0x00; // disable analog function
	GPIOA->PCTL = 0x00; //  select function pins
	GPIOA->DIR |= 0x2C; // direction pin IN: PA6 PA4    OUT: PA5 PA3 PA2
  GPIOA->AFSEL = 0x00; // disable alternative function pins
	GPIOA->PUR |= 0x50; // enable internal pull-up PA6 and PA4
	GPIOA->DEN |= 0X7C; // enable digital function pins		
		
	TP_Read_XY(&tp_dev.x,&tp_dev.y);
		
	LCD_Clear(WHITE);
	
			
	return 1;
}



