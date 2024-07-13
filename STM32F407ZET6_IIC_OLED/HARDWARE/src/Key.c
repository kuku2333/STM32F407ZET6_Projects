#include "Key.h"

/**
   ******************************************************************************
   * @brief		初始化按键中断函数，需根据需要更改时钟、引脚、映射、中断、
   *			中断通道以及中断服务函数
   ******************************************************************************
   * 
   ******************************************************************************
**/
void Key_Init(void)
{
	//声明结构体变量，分别用于GPIO、NVIC、EXTI
	NVIC_InitTypeDef	NVIC_InitStructure;
	EXTI_InitTypeDef	EXTI_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	//开启GPIO和SYSCFG的时钟，SYSCFG用于配置中断源
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//配置KEY0结构体成员的值
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_0;//引脚
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IN;//输入模式
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;//上拉下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化结构体
	
	//初始化 PE2 PE3 PE4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	//选择GPIO用于哪个中断源
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
	
	//配置中断源
	EXTI_InitStructure.EXTI_Line	=	EXTI_Line0 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4;//中断源
	EXTI_InitStructure.EXTI_Mode	=	EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStructure.EXTI_Trigger =	EXTI_Trigger_Falling;//触发方式
	EXTI_InitStructure.EXTI_LineCmd =	ENABLE;//使能
	EXTI_Init(&EXTI_InitStructure);//初始化结构体
	
	//配置NVIC管理外部中断
	NVIC_InitStructure.NVIC_IRQChannel						=	EXTI0_IRQn;//中断源，IRQn-》中断请求
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	0X01;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			=	0X00;//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd					=	ENABLE;//使能
	NVIC_Init(&NVIC_InitStructure);//初始化结构体
	
	//初始化外部中断通道 2
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0X02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0X00;
	NVIC_Init(&NVIC_InitStructure);
	//初始化外部中断通道 3
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0X03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0X00;
	NVIC_Init(&NVIC_InitStructure);
	//初始化外部中断通道 4
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0X04;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0X00;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line0) == SET)
	{
		Delay_ms(10);
		
		if( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == RESET)
		{
			GPIO_ToggleBits(GPIOF, GPIO_Pin_9);	
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
		//Delay_ms(10);
	}
}

void EXTI2_IRQHandler(void)
{
	//GPIO_WriteBit(GPIOF, GPIO_Pin_10, RESET);
	if(EXTI_GetFlagStatus(EXTI_Line2) == SET)
	{
		Delay_ms(10);
		
		if( GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == RESET)
		{
			GPIO_ToggleBits(GPIOF, GPIO_Pin_10);	
		}
		EXTI_ClearITPendingBit(EXTI_Line2);
		//Delay_ms(10);
	}
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line3) == SET)
	{
		Delay_ms(10);
		
		if( GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == RESET)
		{
			GPIO_ToggleBits(GPIOE, GPIO_Pin_13);
		}
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line4) == SET)
	{
		Delay_ms(10);
		
		if( GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == RESET)
		{
			GPIO_ToggleBits(GPIOE, GPIO_Pin_14);
		}
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}
