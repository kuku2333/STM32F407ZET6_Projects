/**
   ******************************************************************************
   * @file    STM32F407ZET6_��������LED\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-06-28
   * @brief   ��KEY0����LED0�����������ɿ���
   ******************************************************************************
   * �������ϵ��ĸ�LED����MCU��PF9��PF10��PE13��PE14������
   * KEY0��PA0������
   ******************************************************************************
 **/

#include "stm32f4xx.h"                  // Device header

int main(void)
{
	//����GPIO�ṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//����GPIOF��GPIOA��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//����PF9
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	//����PA0
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	while(1)
	{
		//Ĭ�ϸߵ�ƽ�����µ͵�ƽ
		if(!(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)))
		{
			//LED��
			GPIO_ResetBits(GPIOF, GPIO_Pin_9);
			//GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
		}
		else
		{
			//LED��
			GPIO_SetBits(GPIOF, GPIO_Pin_9);
		}
	}
}
