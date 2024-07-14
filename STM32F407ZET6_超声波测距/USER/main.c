/**
   ******************************************************************************
   * @file    STM32F407ZET6_外设完备版\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-09
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include "sys.h"
#include "Gpio.h"
#include "Tim.h"
#include "Uasrt_Send_String.h"
#include "Wave.h"

int main(void)
{
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uint32_t count = 0;
	
	Wave_Init();
	Uasrt1_Init();
	
	while(1)
	{
		count = Get_Distance();
		printf("Distance: %dcm\r\n", count);
		Delay_ms(1000);
	}
}
