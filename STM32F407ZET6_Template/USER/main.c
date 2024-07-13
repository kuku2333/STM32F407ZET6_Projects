#include "stm32f4xx.h"                  // Device header

/**
   ******************************************************************************
   * @file    USER/main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-06-27
   * @brief   用PF9、PF10、PE13、PE14控制开发板上的LED灯亮
   ******************************************************************************
   * 开发板上的四个LED灯与MCP的PF9、PF10、PE13、PE14相连。
   ******************************************************************************
 **/


int main(void)
{
	//定义GPIO结构体
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//开启GPIOF和GPIOE的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//配置结构体成员的值
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	
	//初始化结构体
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	
	while(1)
	{
		//让对应引脚输出低电平，让灯亮
		GPIOF->BSRRH 	= 	GPIO_Pin_9 	| GPIO_Pin_10;
		GPIOE->BSRRH 	= 	GPIO_Pin_13 | GPIO_Pin_14;
	}
}
