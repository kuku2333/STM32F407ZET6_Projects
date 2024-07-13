#include "Delay.h"

void Delay_us(uint32_t xus)
{
	SysTick->CTRL = 0; // 
	SysTick->LOAD = (xus * 21) - 1; // 
	SysTick->VAL = 0; // 
	SysTick->CTRL = 1; // 
	while ((SysTick->CTRL & 0x00010000)==0);// 
	SysTick->CTRL = 0; // 
}

void Delay_ms(uint32_t xms)
{
	SysTick->CTRL = 0; // D
	SysTick->LOAD = (xms * 21 * 1000) - 1; // 
	SysTick->VAL = 0; // 
	SysTick->CTRL = 1; // 
	while ((SysTick->CTRL & 0x00010000)==0);// 
	SysTick->CTRL = 0; // 
}

void Delay_s(int ns)
{
	int i;
	
	for(i=0; i<ns; i++)
	{
		Delay_ms(500);
		Delay_ms(500);
	}
}
