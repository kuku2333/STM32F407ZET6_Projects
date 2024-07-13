#include "Tim.h"

uint32_t g_cap_val;

/**
  * @brief �ö�ʱ��������岨���ɸ�����Ҫ��������
  *
  * @steps 1.������ʱ���Ͷ�Ӧ���ŵ�ʱ��
  *		   2.���ö�ʱ����Ӧ�����ŵĽṹ�塣TIM14��Ӧ������ΪPF9
  *		   3.�ı����ŵ�ӳ��
  *		   3.���ö�ʱ��ʱ���ṹ�岢��ʼ��
  *		   4.���ö�ʱ������ȽϽṹ�岢��ʼ��
  *		   5.ʹ���������TIM_OCxPreloadConfig(TIMx, TIM_OCPreload_ENABLE);
  *		   6.ʹ�ܶ�ʱ��
  */
void Tim14_PWM_Init(void)
{
	//����ʱ����GPIO������ȽϽṹ��
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	GPIO_InitTypeDef			GPIO_InitStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;

	//������ʱ����GPIO��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);//����TIM14��ʱ�ӣ���APB1��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/* GPIOF ����: TIM14 CH1 (PF9)*/
    GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_9;//����9
    GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_AF;//����ģʽ
    GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);//����ӳ�䵽��ʱ��
	
	/*��ʱ��14��APB1�ϣ�2��Ƶ����84MHZ������Ľṹ����TIM_Prescaler��Ԥ��Ƶֵ����
	TIM_Period������ֵ������ؼ��ģ�84MHZ����8400��Ƶ�󣬳���10KHZ��*/
/**
   ******************************************************************************
   * @brief		�˹����������ؼ�������Ԥ��Ƶֵ���Զ���װ��ֵ����ͨ���������÷�Ƶ
   *			ֵ��Ҳ�����·�һ��ͨ���Ĵ������á��õ��˷�Ƶ���Ƶ�ʺ��Զ���װ
   *			��ֵ�����˼������ڣ�Ҳ�����Ӷ���ʱ����������������ģ���ǰֵ�Ĵ�
   *			�����Զ���װ�ؼĴ��������ȡ�趨��ֵ����ǰֵ�Ĵ�����ֵ���趨ֵ��
   *			0���õݼ���������һ��������һ����ʱ����ʱ�����ڣ������ʱ������
   *			��ֵ�����÷�Ƶ���Ƶ��/�Զ���װ��ֵ�õ��á������Ƶ���Ƶ��Ϊ
   *			21000000���Զ���װ��ֵΪ21����ô��ʱ�����ھ���21000000/21=
   *			1000000=1us��Ҳ����˵����ǰֵ�Ĵ����е�ֵ���趨ֵ��0����������1us
   *			�Դ˴ﵽ��ʱ��Ŀ��
   ******************************************************************************
   * 
   ******************************************************************************
**/
	TIM_TimeBaseStructure.TIM_Prescaler			=	8400 - 1;//84000000/8400=10000HZ
	TIM_TimeBaseStructure.TIM_Period			=	100 - 1;//10000/100=100HZ=100ms
	TIM_TimeBaseStructure.TIM_CounterMode		=	TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
	
	/*����Ƚ�����*/
    TIM_OCInitStructure.TIM_OCMode 			= 		TIM_OCMode_PWM1;//ѡ��PWM1ģʽ��
    TIM_OCInitStructure.TIM_OutputState 	= 		TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse 			= 		30;
    TIM_OCInitStructure.TIM_OCPolarity 		= 		TIM_OCPolarity_Low;/*������ǰֵ���趨��ֵ���ʱ
													�������ʲô��ƽ����PWDʱ��������Ҫ���óɵ͵�ƽ*/
    TIM_OC1Init(TIM14, &TIM_OCInitStructure);

	/*ʹ�ܻ���ʧ��TIMx�������ڼĴ�����CCR2�ϣ����������岨ʱ�����������ǿ��ʹ�õ�*/
	TIM_OC2PreloadConfig(TIM14, TIM_OCPreload_Disable);
	
	TIM_Cmd(TIM14, ENABLE);//ʹ�ܶ�ʱ��
}

/****
    ************************************************************************************
    * @brief	�ö�ʱ��13������
    * @steps	1.ʹ��RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE)����ʹ��TIMʱ��
	*			2.���ö�ʱ����Ӧ�����ŵĽṹ�塣
	*			3.�ı����ŵ�ӳ��
	*			4.���ö�ʱ��ʱ���ṹ�岢��ʼ��
	*			5.���ö�ʱ�����벶��ṹ�岢��ʼ��
	*			6.ʹ��NVIC������DMA����������Ƶ��
	*			7.ʹ�ܶ�Ӧ���жϻ���DMA���������ֵ��ʹ��TIM_ITConfig(TIMx, TIM_IT_CCx)
	*			�����������TIM_DMA_Cmd(TIMx, TIM_DMA_CCx)�������
	*			8.����TIM_Cmd(ENABLE)����ʹ�ܶ�ʱ��
	*			9.ʹ��TIM_GetCapturex(TIMx)��������ȡ�����ֵ
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);//����TIM14��ʱ�ӣ���APB1��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* GPIOF ����: TIM14 CH1 (PF9)*/
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
