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
#include "sys.h"

int main(void)
{
	Tim14_Init();
//	Tim13_Init();
//	Led_Init();
	
	while(1)
	{
//		for(int i=0; i<100; i++)
//		{
//			TIM14->CCR1 = i;
//			Delay_ms(10);
//		}
//		
//		for(int i=100; i>=0; i--)
//		{
//			TIM14->CCR1 = i;
//			Delay_ms(10);
//		}
		PBout(7) = 1;
		Delay_ms(50);
		PBout(7) = 0;
		Delay_ms(50);
	}
}
