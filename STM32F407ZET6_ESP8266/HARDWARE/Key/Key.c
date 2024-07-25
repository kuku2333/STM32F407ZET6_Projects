#include "Key.h"

/**
   ******************************************************************************
   * @brief		��ʼ�������жϺ������������Ҫ����ʱ�ӡ����š�ӳ�䡢�жϡ�
   *			�ж�ͨ���Լ��жϷ�����
   ******************************************************************************
   * 
   ******************************************************************************
**/
void Key_Init(void)
{
	//�����ṹ��������ֱ�����GPIO��NVIC��EXTI
	NVIC_InitTypeDef	NVIC_InitStructure;
	EXTI_InitTypeDef	EXTI_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	//����GPIO��SYSCFG��ʱ�ӣ�SYSCFG���������ж�Դ
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//����KEY0�ṹ���Ա��ֵ
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_0;//����
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IN;//����ģʽ
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ���ṹ��
	
	//��ʼ�� PE2 PE3 PE4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	//ѡ��GPIO�����ĸ��ж�Դ
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
	
	//�����ж�Դ
	EXTI_InitStructure.EXTI_Line	=	EXTI_Line0 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4;//�ж�Դ
	EXTI_InitStructure.EXTI_Mode	=	EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger =	EXTI_Trigger_Falling;//������ʽ
	EXTI_InitStructure.EXTI_LineCmd =	ENABLE;//ʹ��
	EXTI_Init(&EXTI_InitStructure);//��ʼ���ṹ��
	
	//����NVIC�����ⲿ�ж�
	NVIC_InitStructure.NVIC_IRQChannel						=	EXTI0_IRQn;//�ж�Դ��IRQn-���ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	0X01;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			=	0X00;//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd					=	ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStructure);//��ʼ���ṹ��
	
	//��ʼ���ⲿ�ж�ͨ�� 2
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0X02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0X00;
	NVIC_Init(&NVIC_InitStructure);
	//��ʼ���ⲿ�ж�ͨ�� 3
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0X03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0X00;
	NVIC_Init(&NVIC_InitStructure);
	//��ʼ���ⲿ�ж�ͨ�� 4
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
