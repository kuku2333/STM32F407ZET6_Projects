/**
   ******************************************************************************
   * @file    STM32F407ZET6_�ж�_��������LED\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-01
   * @brief   �ð�������LED�������ж�ʵ��
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
