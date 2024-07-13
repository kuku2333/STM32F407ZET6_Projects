#include "delay.h"

void delay_us(uint32_t xus)
{
	uint32_t temp;
	
	while(xus--)
	{
		SysTick->LOAD	=	168000000/8/1000000;
		SysTick->VAL    = 	0;
		SysTick->CTRL  |= 	0x01;
		
		do
		{
			temp = SysTick->CTRL;
		}while((temp&0x01) && !(0x01<<16));
		
		SysTick->VAL	=	0;
		SysTick->CTRL  &=	0;
	}
}

void delay_ms(uint32_t xms)
{
	uint32_t temp;
	
	while(xms--)
	{
		SysTick->LOAD	=	168000000/8/1000;
		SysTick->VAL    = 	0;
		SysTick->CTRL  |= 	0x01;
		
		do
		{
			temp = SysTick->CTRL;
		}while((temp&0x01) && !(0x01<<16));
		
		SysTick->VAL	=	0;
		SysTick->CTRL  &=	0;
	}
}

