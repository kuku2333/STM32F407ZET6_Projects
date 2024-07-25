#include "stm32f4xx.h"
#include "sys.h"
#include "../lvgl.h"
#include "tim.h"

void tim3_init(void)
{
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	NVIC_InitTypeDef   		NVIC_InitStructure;
	
	//打开TIM3硬件时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		
	TIM_TimeBaseStructure.TIM_Period = 10000/1000-1;//计数值，用于定时时间的设置
	TIM_TimeBaseStructure.TIM_Prescaler = 8399;//预分频值的配置
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;//F407没有，不需要配置
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数，就是0 -> TIM_Period，然后就会触发时间更新中断
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* 定时器时间更新中断使能 */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	//配置NVIC，用于打开TIM3的中断请求通道
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	//TIM3的请求通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级0xF
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//响应优先级0xF
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//打开其请求通道
	NVIC_Init(&NVIC_InitStructure);
	
	/* 使能TIM3工作 */
	TIM_Cmd(TIM3, ENABLE);	
}

void TIM3_IRQHandler(void)
{
	static uint32_t usart1_rx_cnt_last=0;
	static uint32_t irq_cnt=0;
	
	//检测时间更新中断的标志位是否置位
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		irq_cnt++;
		
		if(irq_cnt>=10 && g_usart1_rx_cnt)
		{
			//若相等，则表示串口3目前接收数据完毕
			if(g_usart1_rx_cnt == usart1_rx_cnt_last)
			{
				g_usart1_rx_end=1;
			}
			else
			{
				usart1_rx_cnt_last = g_usart1_rx_cnt;
			}
			
			irq_cnt=0;
		}
		
		lv_tick_inc(1);
		
		//清空标志位
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void Tim14_Init(void)
{
	//定义时基、GPIO、输出比较结构体
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	GPIO_InitTypeDef			GPIO_InitStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;

	//开启定时器、GPIO的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//开启TIM14的时钟，在APB1上
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/* GPIOF 配置: TIM14 CH1 (PF9)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//引脚9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);//引脚映射到定时器
	
	/*定时器14在APB1上，2倍频后，是84MHZ。下面的结构体中TIM_Prescaler（预分频值）和
	TIM_Period（周期值）是最关键的，84MHZ经过8400分频后，成了1KHZ。
	1HZ的意思是在1s内的频率为1，1/f=t，周期就是时间。也就是说，1/1HZ对应
	1s，那么1/1000HZ=1ms。那么现在就是计数器每记一次数，就过了1ms，所以将计数器的值
	设置成100，就过了100ms。此处需要定时100ms，来当作输出比较的一个周期*/
	TIM_TimeBaseStructure.TIM_Prescaler			=	84000 - 1;
	TIM_TimeBaseStructure.TIM_Period			=	100 -  1;
	TIM_TimeBaseStructure.TIM_CounterMode		=	TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/* Output Compare Toggle Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//选择PWM1模式，
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 30;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);

	/*Enables or disables the TIMx peripheral Preload register on CCR2.
	使能或者失能TIMx外设周期寄存器在CCR2上，在配置脉冲波时，这个函数是强制使用的*/
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Disable);
	
	TIM_Cmd(TIM4, ENABLE);//使能定时器
}
