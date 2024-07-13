/**
   ******************************************************************************
   * @file    STM32F407ZET6_中断_按键控制LED\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-01
   * @brief   用按键控制LED的亮灭，中断实现
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
//		//默认高电平，按下低电平
//		if(!(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)))
//		{
//			//LED亮
//			GPIO_ResetBits(GPIOF, GPIO_Pin_10);
//			//GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
//		}
//		else
//		{
//			//LED灭
//			GPIO_SetBits(GPIOF, GPIO_Pin_10);
//		}
		GPIO_WriteBit(GPIOF, GPIO_Pin_9, RESET);
		Delay_ms(1000);
		GPIO_WriteBit(GPIOF, GPIO_Pin_9, SET);
		Delay_ms(1000);
	}
}
