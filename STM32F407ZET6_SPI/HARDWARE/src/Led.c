#include "Led.h"

void Led_Init(void)
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
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	//配置结构体成员的值
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//
	GPIO_WriteBit(GPIOF, GPIO_Pin_9, Bit_SET);
	GPIO_WriteBit(GPIOF, GPIO_Pin_10, Bit_SET);
	GPIO_WriteBit(GPIOE, GPIO_Pin_13, Bit_SET);
	GPIO_WriteBit(GPIOE, GPIO_Pin_14, Bit_SET);
}

