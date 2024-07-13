/**
   ******************************************************************************
   * @file    STM32F407ZET6_ÓÃTIM14²úÉúÂö³å²¨\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-03
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#include "stm32f4xx.h"                  // Device header
#include "Tim.h"
#include "Delay.h"
#include "Led.h"
#include "Uasrt_Send_String.h"
#include "Iwdg.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Iwdg_Init();
	Led_Init();
	
	while(1)
	{
		
		Delay_ms(500);
		IWDG_ReloadCounter();
		Delay_ms(500);
		GPIO_WriteBit(GPIOF, GPIO_Pin_9, Bit_RESET);
		IWDG_ReloadCounter();
	}
}
