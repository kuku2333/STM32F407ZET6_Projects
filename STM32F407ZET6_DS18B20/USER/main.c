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
#include "DS18B20.h"
#include "Uasrt_Send_String.h"
//#include <stdio.h>

int main(void)
{
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Uasrt1_Init();
	GPIO_Out_Init(RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_6, DISABLE);
	
	test();
	
	while(1)
	{
		
		
	}
}
