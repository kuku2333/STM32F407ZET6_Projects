/**
   ******************************************************************************
   * @file    USER/main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-06-28
   * @brief   用库函数来驱动蜂鸣器。
   ******************************************************************************
   * 开发板上的PF8与蜂鸣器相连。
   ******************************************************************************
 **/

#include "stm32f4xx.h"                  // Device header


int main(void)
{	
	//配置PF8引脚。
	GPIO_InitTypeDef    GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	while(1)
	{
		//高电平让蜂鸣器响。
		GPIO_WriteBit(GPIOF, GPIO_Pin_8, Bit_SET);
	} 
}
