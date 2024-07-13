#include "Gpio.h"

/**
   ******************************************************************************
   * @brief		���ģʽ�����죬����
   * @param		Periph_Clock-->����ʱ��
   * @param		GPIOx-->�˿�
   * @param		Pin_x-->����
   * @param		NewState-->��ʼ���״̬
   ******************************************************************************
   * 
   ******************************************************************************
**/
void GPIO_Out_Init(uint32_t Periph_Clock,GPIO_TypeDef* GPIOx, uint32_t Pin_x, FunctionalState NewState)
{
	//����GPIO�ṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//����GPIO��ʱ��
	RCC_AHB1PeriphClockCmd(Periph_Clock, ENABLE);

	//���ýṹ���Ա��ֵ
	GPIO_InitStructure.GPIO_Pin 	= 	Pin_x;//����
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;//�������
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;//����ٶ�
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;//��������
	GPIO_Init(GPIOx, &GPIO_InitStructure);//��ʼ��GPIO
	
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
	//����GPIO�ṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//����GPIO��ʱ��
	RCC_AHB1PeriphClockCmd(Periph_Clock, ENABLE);

	//���ýṹ���Ա��ֵ
	GPIO_InitStructure.GPIO_Pin 	= 	Pin_x;//����
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IN;//����ģʽ
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;//�������
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;//����ٶ�
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;//��������
	GPIO_Init(GPIOx, &GPIO_InitStructure);//��ʼ��GPIO
	
	if (NewState != DISABLE)
	{
		GPIO_WriteBit(GPIOx, Pin_x, Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOx, Pin_x, Bit_RESET);
	}
}
