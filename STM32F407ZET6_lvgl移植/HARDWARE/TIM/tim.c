#include "stm32f4xx.h"
#include "sys.h"
#include "../lvgl.h"

void tim3_init(void)
{
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	NVIC_InitTypeDef   		NVIC_InitStructure;
	
	//��TIM3Ӳ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	
	TIM_TimeBaseStructure.TIM_Period = 10000/1000-1;//����ֵ�����ڶ�ʱʱ�������
	TIM_TimeBaseStructure.TIM_Prescaler = 8399;//Ԥ��Ƶֵ������
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;//F407û�У�����Ҫ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���������0 -> TIM_Period��Ȼ��ͻᴥ��ʱ������ж�
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	
	/* ��ʱ��ʱ������ж�ʹ�� */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	//����NVIC�����ڴ�TIM3���ж�����ͨ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	//TIM3������ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�0xF
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//��Ӧ���ȼ�0xF
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//��������ͨ��
	NVIC_Init(&NVIC_InitStructure);
	
	
	/* ʹ��TIM3���� */
	TIM_Cmd(TIM3, ENABLE);	
}

void TIM3_IRQHandler(void)
{
	/* ���ʱ������жϵı�־λ�Ƿ���λ */
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		/* ϵͳʱ����ǰ�ƽ�һ����λ��ʱ���� */
		lv_tick_inc(1);
	
		/* ��ձ�־λ */
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
