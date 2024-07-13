#include "Gpio.h"

/**
   ******************************************************************************
   * @brief		输出模式，推挽，上拉
   * @param		Periph_Clock-->外设时钟
   * @param		GPIOx-->端口
   * @param		Pin_x-->引脚
   * @param		NewState-->初始输出状态
   ******************************************************************************
   * 
   ******************************************************************************
**/
void GPIO_Out_Init(uint32_t Periph_Clock,GPIO_TypeDef* GPIOx, uint32_t Pin_x, FunctionalState NewState)
{
	//定义GPIO结构体
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//开启GPIO的时钟
	RCC_AHB1PeriphClockCmd(Periph_Clock, ENABLE);

	//配置结构体成员的值
	GPIO_InitStructure.GPIO_Pin 	= 	Pin_x;//引脚
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;//输出模式
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;//输出类型
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;//输出速度
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;//上拉下拉
	GPIO_Init(GPIOx, &GPIO_InitStructure);//初始化GPIO
	
	if (NewState != DISABLE)
	{
		GPIO_WriteBit(GPIOx, Pin_x, Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOx, Pin_x, Bit_RESET);
	}
}

void GPIO_In_Init(uint32_t Periph_Clock,GPIO_TypeDef* GPIOx, uint32_t Pin_x, FunctionalState NewState)
{
	//定义GPIO结构体
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//开启GPIO的时钟
	RCC_AHB1PeriphClockCmd(Periph_Clock, ENABLE);

	//配置结构体成员的值
	GPIO_InitStructure.GPIO_Pin 	= 	Pin_x;//引脚
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IN;//输入模式
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;//输出类型
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;//输出速度
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;//上拉下拉
	GPIO_Init(GPIOx, &GPIO_InitStructure);//初始化GPIO
	
	if (NewState != DISABLE)
	{
		GPIO_WriteBit(GPIOx, Pin_x, Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOx, Pin_x, Bit_RESET);
	}
}
