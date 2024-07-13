#include "stm32f4xx.h"                  // Device header

/**
   ******************************************************************************
   * @file    USER/main.c 
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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//���ýṹ���Ա��ֵ
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	
	//��ʼ���ṹ��
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	
	while(1)
	{
		//�ö�Ӧ��������͵�ƽ���õ���
		GPIOF->BSRRH 	= 	GPIO_Pin_9 	| GPIO_Pin_10;
		GPIOE->BSRRH 	= 	GPIO_Pin_13 | GPIO_Pin_14;
	}
}
