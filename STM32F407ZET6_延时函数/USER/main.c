#include "stm32f4xx.h"                  // Device header
#include "Delay.h"

/**
   ******************************************************************************
   * @file    STM32F407ZET6_库函数点四盏灯\USER\main.c 
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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//配置结构体成员的值
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_6 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	
	//初始化结构体
	//GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_6 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_SET);
	GPIO_WriteBit(GPIOE, GPIO_Pin_14, Bit_SET);
	
	while(1)
	{
		//让对应引脚输出低电平，让灯亮
		GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_RESET);
		Delay_ms(10);
		//让对应引脚输出高电平，让灯灭
		GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_SET);
		Delay_ms(10);	
	}
}
