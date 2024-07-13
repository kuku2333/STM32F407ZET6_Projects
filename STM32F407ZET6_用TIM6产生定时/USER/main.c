/**
   ******************************************************************************
   * @file    STM32F407ZET6_用TIM6产生定时\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-03
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#include "stm32f4xx.h"                  // Device header
#include "Led.h"
#include "Tim.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Led_Init();
	Tim7_Init();
	
	while(1)
	{
		
	}
}
