#include "Gpio.h"

/**
   ******************************************************************************
   * @brief		输出模式
   * @param		Periph_Clock-->外设时钟
   *			例如：RCC_AHB1Periph_GPIOF
   * @param		GPIOx-->端口
   *			例如：GPIOF
   * @param		Pin_x-->引脚
   *			GPIO_Pin_9
   * @param		NewState-->初始输出状态
   *			例如：ENABLE DISABLE
   ******************************************************************************
   * 输出模式、速度、上拉下拉，看情况手动修改
   ******************************************************************************
**/
void GPIO_Out_Init(uint32_t RCC_AHB1Periph_GPIOx, GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x, FunctionalState NewState)
{
	//定义GPIO结构体GPIO_Out_Init(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_9, DISABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//开启GPIO的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);

	//配置结构体成员的值
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_x;//引脚
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;//输出模式
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;//输出类型
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;//输出速度
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;//上拉下拉
	GPIO_Init(GPIOx, &GPIO_InitStructure);//初始化GPIO
	
	if (NewState != DISABLE)
	{
		GPIO_WriteBit(GPIOx, GPIO_Pin_x, Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOx, GPIO_Pin_x, Bit_RESET);
	}
}

/**
   ******************************************************************************
   * @brief		输入模式
   * @param		Periph_Clock-->外设时钟
   *			例如：RCC_AHB1Periph_GPIOF
   * @param		GPIOx-->端口
   *			例如：GPIOF
   * @param		Pin_x-->引脚
   *			GPIO_Pin_9
   * @param		NewState-->初始输出状态
   *			例如：ENABLE DISABLE
   ******************************************************************************
   * 
   ******************************************************************************
**/
void GPIO_In_Init(uint32_t RCC_AHB1Periph_GPIOx, GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x, FunctionalState NewState)
{
	//定义GPIO结构体
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//开启GPIO的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);

	//配置结构体成员的值
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_x;//引脚
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IN;//输入模式
	GPIO_Init(GPIOx, &GPIO_InitStructure);//初始化GPIO
	
	if (NewState != DISABLE)
	{
		GPIO_WriteBit(GPIOx, GPIO_Pin_x, Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOx, GPIO_Pin_x, Bit_RESET);
	}
}
