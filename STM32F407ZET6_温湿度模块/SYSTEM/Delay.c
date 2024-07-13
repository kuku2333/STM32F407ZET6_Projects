/**
   ******************************************************************************
   * @file    STM32F407ZET6_外设完备版\SYSTEM\Delay.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-09
   * @brief   用systick产生us、ms、s级别的定时
   ******************************************************************************
   * 
   ******************************************************************************
**/

#include "Delay.h"

void Delay_us(uint32_t xus)
{
	/*每次计数周期为1us，原理查内核手册，所以循环多少次就是多少us*/ 
	
	while(xus--)
	{
		SysTick->CTRL 	= 	0; 			/*	控制与状态寄存器，0位段：1开0关*/
		SysTick->LOAD 	= 	21 - 1; 	/*	重装载值寄存器，决定记多少个计数周期*/
		SysTick->VAL 	= 	0; 			/*	当前数值寄存器，写它可使之清零，还会
											清除CTRL中COUNTFLAG标志，具体作用查内核手册*/
		SysTick->CTRL 	= 	1; 			/*	先关，再配置LOAD，清零VAL，再开*/
		
		while ((SysTick->CTRL & 0x00010000)==0);	/*	16位段是COUNTFLAG标志，为1则表示
														数完了，可以退出循环了*/
	}
	
	SysTick->CTRL 	= 	0; 				/*	数完之后要关闭定时器*/
}

void Delay_ms(uint32_t xms)
{
	/*每次计数周期为1ms，原理查内核手册，所以循环多少次就是多少ms*/ 
	
	while(xms--)
	{
		SysTick->CTRL 	= 	0; 						/*	控制与状态寄存器，0位段：1开0关*/
		SysTick->LOAD 	= 	(21 * 1000) - 1; 		/*	重装载值寄存器，决定记多少个计数周期*/
		SysTick->VAL 	= 	0; 						/*	当前数值寄存器，写它可使之清零，还会*/
		SysTick->CTRL 	= 	1; 						/*	清除CTRL中COUNTFLAG标志，具体作用查内核手册
														先关，再配置LOAD，清零VAL，再开*/
		while ((SysTick->CTRL & 0x00010000)==0);	/*	16位段是COUNTFLAG标志，为1则表示
															数完了，可以退出循环了*/
	}
	
	SysTick->CTRL = 0; /*	数完之后要关闭定时器*/
}

/*用ms延时来实现s延时*/
void Delay_s(int ns)
{
	int i;
	
	for(i=0; i<ns; i++)
	{
		Delay_ms(1000);
	}
}
