#include "Delay.h"

u32 my_us = 21;   	//在21MHZ，计21个数，用时1us
u32 my_ms = 21000;   //在21MHZ，计21000个数，用时1ms

void Delay_Init(void)
{
	//选定Systick定时器的时钟 = HCLK/8 = 168MHZ/8 = 21MHZ
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

void Delay_us(uint32_t xus)
{
	u32 temp = 0x00;
	
	//设置重装值  SysTick->LOAD：24位
	SysTick->LOAD = my_us*xus - 1; //21000
	
	//设置计数器值0
	SysTick->VAL = 0x00;
	
	//启动定时器 SysTick->LOAD值装给SysTick->VAL
	SysTick->CTRL |= (0x01<<0); 
	
	do
	{
		temp = SysTick->CTRL;
		
		//(temp & (0x01<<0))判断定时器是否开启，如果关闭，则退出
		//判断temp第16位，如果为1,表示计数器从 SysTick->LOAD倒数到0
	}while((temp & (0x01<<0)) && !(temp&(0x01<<16)) );
	

	//关闭定时器
	SysTick->CTRL &= ~(0x01<<0); 	
}

void Delay_ms(uint32_t xms)
{
	u32 temp = 0x00;
	
	//设置重装值  SysTick->LOAD：24位
	SysTick->LOAD = my_ms*xms - 1; 
	
	//设置计数器值0
	SysTick->VAL = 0x00;
	
	//启动定时器 SysTick->LOAD值装给SysTick->VAL
	SysTick->CTRL |= (0x01<<0); 
	
	do
	{
		temp = SysTick->CTRL;
		
		//(temp & (0x01<<0))判断定时器是否开启，如果关闭，则退出
		//判断temp第16位，如果为1,表示计数器从 SysTick->LOAD倒数到0
	}while((temp & (0x01<<0)) && !(temp&(0x01<<16)) );
	
	//关闭定时器
	SysTick->CTRL &= ~(0x01<<0); 	
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
