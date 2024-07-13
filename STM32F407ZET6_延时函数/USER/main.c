#include "stm32f4xx.h"                  // Device header
#include "Delay.h"

/**
   ******************************************************************************
   * @file    STM32F407ZET6_�⺯������յ��\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-06-27
   * @brief   ��PF9��PF10��PE13��PE14���ƿ������ϵ�LED����
   ******************************************************************************
   * �������ϵ��ĸ�LED����MCP��PF9��PF10��PE13��PE14������
   ******************************************************************************
 **/


int main(void)
{
	//����GPIO�ṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//����GPIOF��GPIOE��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//���ýṹ���Ա��ֵ
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_6 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	
	//��ʼ���ṹ��
	//GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_6 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_SET);
	GPIO_WriteBit(GPIOE, GPIO_Pin_14, Bit_SET);
	
	while(1)
	{
		//�ö�Ӧ��������͵�ƽ���õ���
		GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_RESET);
		Delay_ms(10);
		//�ö�Ӧ��������ߵ�ƽ���õ���
		GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_SET);
		Delay_ms(10);	
	}
}
