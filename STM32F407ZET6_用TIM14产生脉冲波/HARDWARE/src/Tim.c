#include "Tim.h"

/**
  * @brief 用定时器输出脉冲波
  *
  * @steps 1.开启定时器和对应引脚的时钟
  *		   2.配置定时器对应的引脚的结构体，TIM14对应的引脚为PF9
  *		   3.改变引脚的映射
  *		   3.配置定时器时基结构体并初始化
  *		   4.配置定时器输出比较结构体并初始化
  *		   5.使用这个函数TIM_OCxPreloadConfig(TIMx, TIM_OCPreload_ENABLE);
  *		   6.使能定时器
  */
void Tim14_Init()
{
	//定义时基、GPIO、输出比较结构体
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	GPIO_InitTypeDef			GPIO_InitStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;

	//开启定时器、GPIO的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);//开启TIM14的时钟，在APB1上
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/* GPIOF 配置: TIM14 CH1 (PF9)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//引脚9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);//引脚映射到定时器
	
	/*定时器14在APB1上，2倍频后，是84MHZ。下面的结构体中TIM_Prescaler（预分频值）和
	TIM_Period（周期值）是最关键的，84MHZ经过8400分频后，成了1KHZ。
	1HZ的意思是在1s内的频率为1，1/f=t，周期就是时间。也就是说，1/1HZ对应
	1s，那么1/1000HZ=1ms。那么现在就是计数器每记一次数，就过了1ms，所以将计数器的值
	设置成100，就过了100ms。此处需要定时100ms，来当作输出比较的一个周期*/
	TIM_TimeBaseStructure.TIM_Prescaler			=	84000 - 1;
	TIM_TimeBaseStructure.TIM_Period			=	100 -  1;
	TIM_TimeBaseStructure.TIM_CounterMode		=	TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
	
	/* Output Compare Toggle Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//选择PWM1模式，
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 30;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC1Init(TIM14, &TIM_OCInitStructure);

	/*Enables or disables the TIMx peripheral Preload register on CCR2.
	使能或者失能TIMx外设周期寄存器在CCR2上，在配置脉冲波时，这个函数是强制使用的*/
	TIM_OC2PreloadConfig(TIM14, TIM_OCPreload_Disable);
	
	TIM_Cmd(TIM14, ENABLE);//使能定时器
}


/**
  * @brief 用定时器14输出脉冲波
  *
  * @steps 1.开启定时器14的时钟
  *		   2.配置定时器14对应的引脚，引脚为PF9
  *		   3.配置定时器14的结构体并初始化
  *		   4.配置定时器14的输出比较的结构体并初始化
  *		   5.使用这个函数TIM_OCxPreloadConfig(TIMx, TIM_OCPreload_ENABLE);
  *		   6.使能定时器14
  */
void Tim13_Init()
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	GPIO_InitTypeDef			GPIO_InitStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);//开启TIM14的时钟，在APB1上
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF, ENABLE);
	
	/* GPIOF 配置: TIM14 CH1 (PF9)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);
	
	TIM_TimeBaseStructure.TIM_Prescaler			=	84000 - 1;//
	TIM_TimeBaseStructure.TIM_Period			=	100 -  1;
	TIM_TimeBaseStructure.TIM_CounterMode		=	TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);
	
	/* Output Compare Toggle Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 30;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC1Init(TIM13, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM13, TIM_OCPreload_Disable);
	
	TIM_Cmd(TIM13, ENABLE);
}
