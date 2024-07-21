/**
   ******************************************************************************
   * @file    STM32F407ZET6_�����걸��\USER\main.c 
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
#include "RTC.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Uasrt1_Init();
	RTC_Calendar_Init();
	
	
	while(1)
	{
		//Input_Capture_test();
		RTC_ShowTime();
	}
}
