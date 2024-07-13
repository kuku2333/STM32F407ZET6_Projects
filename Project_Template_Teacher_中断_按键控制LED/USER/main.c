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
#include "Key.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Led_Init();
	//Key_Init();
	
	while(1)
	{
//		//Ĭ�ϸߵ�ƽ�����µ͵�ƽ
//		if(!(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)))
//		{
//			//LED��
//			GPIO_ResetBits(GPIOF, GPIO_Pin_10);
//			//GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
//		}
//		else
//		{
//			//LED��
//			GPIO_SetBits(GPIOF, GPIO_Pin_10);
//		}
		GPIO_WriteBit(GPIOF, GPIO_Pin_9, RESET);
		Delay_ms(1000);
		GPIO_WriteBit(GPIOF, GPIO_Pin_9, SET);
		Delay_ms(1000);
	}
}
