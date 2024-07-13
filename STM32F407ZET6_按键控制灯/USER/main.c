/**
   ******************************************************************************
   * @file    STM32F407ZET6_按键控制LED\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-06-28
   * @brief   用KEY0控制LED0，按下亮，松开灭。
   ******************************************************************************
   * 开发板上的四个LED灯与MCU的PF9、PF10、PE13、PE14相连。
   * KEY0与PA0相连。
   ******************************************************************************
 **/

#include "stm32f4xx.h"                  // Device header

int main(void)
{
	//定义GPIO结构体
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//开启GPIOF和GPIOA的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//配置PF9
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	//配置PA0
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	while(1)
	{
		//默认高电平，按下低电平
		if(!(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)))
		{
			//LED亮
			GPIO_ResetBits(GPIOF, GPIO_Pin_9);
			//GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
		}
		else
		{
			//LED灭
			GPIO_SetBits(GPIOF, GPIO_Pin_9);
		}
	}
}
