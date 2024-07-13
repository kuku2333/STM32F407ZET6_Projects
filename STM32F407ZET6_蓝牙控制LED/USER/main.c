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

extern char revbuff[REVSIZE];

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Uasrt1_Init();
	Led_Init();

	

//	Send_String("Hello");
//	Send_String("Hello");
//	Send_String("Hello");
	while(1)
	{
		//Send_String("HelloWorld!\r\n");
		
	}
}
