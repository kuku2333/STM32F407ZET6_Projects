/**
   ******************************************************************************
   * @file    USER/main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-06-28
   * @brief   �ÿ⺯����������������
   ******************************************************************************
   * �������ϵ�PF8�������������
   ******************************************************************************
 **/

#include "stm32f4xx.h"                  // Device header


int main(void)
{	
	//����PF8���š�
	GPIO_InitTypeDef    GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	while(1)
	{
		//�ߵ�ƽ�÷������졣
		GPIO_WriteBit(GPIOF, GPIO_Pin_8, Bit_SET);
	} 
}
