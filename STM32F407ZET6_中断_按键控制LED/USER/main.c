/**
   ******************************************************************************
   * @file    STM32F407ZET6_中断_按键控制LED\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-01
   * @brief   用按键控制LED的亮灭，中断实现
   ******************************************************************************
   * LED0 == PF9, KEY0 == PA0
   ******************************************************************************
**/

#include "stm32f4xx.h"                  // Device header
#include "Led.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Delay_Init();
	Led_Init();
	
	while(1)
	{
		GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
		Delay_s(1);
	}
}
