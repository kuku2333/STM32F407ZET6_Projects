#include "Tim.h"

void Tim7_Init(uint32_t num)
{
	NVIC_InitTypeDef 			NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Prescaler		=	84000 - 1;
	TIM_TimeBaseStructure.TIM_Period			=	1000 - 1;
	TIM_TimeBaseStructure.TIM_CounterMode		=	TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM3, ENABLE);
}

void TIM7_IRQHandler(void)
{
	GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
}
