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
#include "Key.h"

int main(void)
{
	Tim14_Init();
	Tim3_Init();
	Key_Init();
	
	while(1)
	{
		
	}
	
//	while(1)
//	{
//		for(int i=0; i<100; i++)
//		{
//			TIM13->CCR1 = i;
//			Delay_ms(10);
//		}
//		
//		for(int i=100; i>=0; i--)
//		{
//			TIM13->CCR1 = i;
//			Delay_ms(10);
//		}
//	}
}
