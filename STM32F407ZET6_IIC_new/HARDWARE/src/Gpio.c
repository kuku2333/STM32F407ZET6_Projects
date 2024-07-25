#include "Gpio.h"

/**
   ******************************************************************************
   * @brief		���ģʽ
   * @param		Periph_Clock-->����ʱ��
   *			���磺RCC_AHB1Periph_GPIOF
   * @param		GPIOx-->�˿�
   *			���磺GPIOF
   * @param		Pin_x-->����
   *			GPIO_Pin_9
   * @param		NewState-->��ʼ���״̬
   *			���磺ENABLE DISABLE
   ******************************************************************************
   * ���ģʽ���ٶȡ�����������������ֶ��޸�
   ******************************************************************************
**/
void GPIO_Out_Init(uint32_t RCC_AHB1Periph_GPIOx, GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x, FunctionalState NewState)
{
	//����GPIO�ṹ��GPIO_Out_Init(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_9, DISABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//����GPIO��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);

	//���ýṹ���Ա��ֵ
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_x;//����
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;//����ٶ�
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;//��������
	GPIO_Init(GPIOx, &GPIO_InitStructure);//��ʼ��GPIO
	
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
   * @brief		����ģʽ
   * @param		Periph_Clock-->����ʱ��
   *			���磺RCC_AHB1Periph_GPIOF
   * @param		GPIOx-->�˿�
   *			���磺GPIOF
   * @param		Pin_x-->����
   *			GPIO_Pin_9
   * @param		NewState-->��ʼ���״̬
   *			���磺ENABLE DISABLE
   ******************************************************************************
   * 
   ******************************************************************************
**/
void GPIO_In_Init(uint32_t RCC_AHB1Periph_GPIOx, GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x, FunctionalState NewState)
{
	//����GPIO�ṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//����GPIO��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);

	//���ýṹ���Ա��ֵ
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_x;//����
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IN;//����ģʽ
	GPIO_Init(GPIOx, &GPIO_InitStructure);//��ʼ��GPIO
	
	if (NewState != DISABLE)
	{
		GPIO_WriteBit(GPIOx, GPIO_Pin_x, Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOx, GPIO_Pin_x, Bit_RESET);
	}
}
