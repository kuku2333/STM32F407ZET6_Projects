#include "Tim.h"

uint32_t g_cap_val;

/**
  * @brief 用定时器输出脉冲波。可根据需要更改配置
  *
  * @steps 1.开启定时器和对应引脚的时钟
  *		   2.配置定时器对应的引脚的结构体。TIM14对应的引脚为PF9
  *		   3.改变引脚的映射
  *		   3.配置定时器时基结构体并初始化
  *		   4.配置定时器输出比较结构体并初始化
  *		   5.使用这个函数TIM_OCxPreloadConfig(TIMx, TIM_OCPreload_ENABLE);
  *		   6.使能定时器
  */
void Tim14_PWM_Init(void)
{
	//定义时基、GPIO、输出比较结构体
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	GPIO_InitTypeDef			GPIO_InitStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;

	//开启定时器、GPIO的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);//开启TIM14的时钟，在APB1上
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/* GPIOF 配置: TIM14 CH1 (PF9)*/
    GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_9;//引脚9
    GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_AF;//复用模式
    GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);//引脚映射到定时器
	
	/*定时器14在APB1上，2倍频后，是84MHZ。下面的结构体中TIM_Prescaler（预分频值）和
	TIM_Period（周期值）是最关键的，84MHZ经过8400分频后，成了10KHZ。*/
/**
   ******************************************************************************
   * @brief		此功能有两个关键参数，预分频值和自动重装载值，可通过函数配置分频
   *			值，也可像下方一样通过寄存器设置。得到了分频后的频率后，自动重装
   *			载值决定了计数周期，也就是延多少时。具体过程是这样的：当前值寄存
   *			器从自动重装载寄存器那里获取设定的值，当前值寄存器的值从设定值到
   *			0（用递减计数），一共经历了一个定时器的时钟周期，而这个时钟周期
   *			的值，是用分频后的频率/自动重装载值得到得。比如分频后的频率为
   *			21000000，自动重装载值为21，那么定时器周期就是21000000/21=
   *			1000000=1us，也就是说，当前值寄存器中的值从设定值到0，共经历了1us
   *			以此达到定时的目的
   ******************************************************************************
   * 
   ******************************************************************************
**/
	TIM_TimeBaseStructure.TIM_Prescaler			=	8400 - 1;//84000000/8400=10000HZ
	TIM_TimeBaseStructure.TIM_Period			=	100 - 1;//10000/100=100HZ=100ms
	TIM_TimeBaseStructure.TIM_CounterMode		=	TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
	
	/*输出比较配置*/
    TIM_OCInitStructure.TIM_OCMode 			= 		TIM_OCMode_PWM1;//选择PWM1模式，
    TIM_OCInitStructure.TIM_OutputState 	= 		TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse 			= 		30;
    TIM_OCInitStructure.TIM_OCPolarity 		= 		TIM_OCPolarity_Low;/*决定当前值与设定的值相等时
													引脚输出什么电平，在PWD时，很明显要设置成低电平*/
    TIM_OC1Init(TIM14, &TIM_OCInitStructure);

	/*使能或者失能TIMx外设周期寄存器在CCR2上，在配置脉冲波时，这个函数是强制使用的*/
	TIM_OC2PreloadConfig(TIM14, TIM_OCPreload_Disable);
	
	TIM_Cmd(TIM14, ENABLE);//使能定时器
}

/****
    ************************************************************************************
    * @brief	用定时器13捕获波形
    * @steps	1.使用RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE)函数使能TIM时钟
	*			2.配置定时器对应的引脚的结构体。
	*			3.改变引脚的映射
	*			4.配置定时器时基结构体并初始化
	*			5.配置定时器输入捕获结构体并初始化
	*			6.使能NVIC或者用DMA来读测量的频率
	*			7.使能对应的中断或者DMA来读捕获的值，使用TIM_ITConfig(TIMx, TIM_IT_CCx)
	*			这个函数或者TIM_DMA_Cmd(TIMx, TIM_DMA_CCx)这个函数
	*			8.调用TIM_Cmd(ENABLE)函数使能定时器
	*			9.使用TIM_GetCapturex(TIMx)函数来读取捕获的值
    ************************************************************************************
    * 
    ************************************************************************************
*****/
void Tim13_InputCapture_Init(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	GPIO_InitTypeDef			GPIO_InitStructure;
	TIM_ICInitTypeDef  			TIM_ICInitStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);//开启TIM14的时钟，在APB1上
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* GPIOF 配置: TIM14 CH1 (PF9)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM13);
	
	TIM_TimeBaseStructure.TIM_Prescaler			=	8400 - 1;
	TIM_TimeBaseStructure.TIM_Period			=	10 -  1;
	TIM_TimeBaseStructure.TIM_CounterMode		=	TIM_CounterMode_Up;
	//TIM_TimeBaseStructure.TIM_ClockDivision		=	TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM1, &TIM_ICInitStructure);
	
	/* Enable the TIM1 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	/* TIM enable counter */
    TIM_Cmd(TIM13, ENABLE);

}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM13, TIM_IT_CC1) == SET)
	{
		g_cap_val = TIM_GetCapture1(TIM13);
	}
	TIM_ClearFlag(TIM13, TIM_FLAG_CC1);
}
