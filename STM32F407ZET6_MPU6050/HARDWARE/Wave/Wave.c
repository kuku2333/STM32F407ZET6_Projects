#include "Wave.h"

/* Triger->PC6-->Camera8,Echo->PC8-->Camera10*/
void Wave_Init(void)
{
	GPIO_InitTypeDef			GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	/* */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_RESET);
}

uint32_t Get_Distance(void)
{
	uint32_t count = 0;
	
	/* ���������ź�*/
	PCout(6) = 1;
	Delay_us(15);
	PCout(6) = 0;
	
	/* �ȴ��ߵ�ƽ�ĳ���,���ó�ʱʱ��*/
	while(!PCin(8) && count < 10000)
	{
		count++;
		Delay_us(1);
	}
	if(count >= 10000)
	{
		/* �����û�ȵ������źţ�����1*/
		return 1;
	}
	else
	{
		count = 0;
	}
	
	/* �����ߵ�ƽ���ֵ�ʱ�䣬������Ч����*/
	while(PCin(8) && count < 23520)
	{
		count++;
		Delay_us(1);
	}
	if(count >= 23520)
	{
		/* �����Чֵ����2*/
		return 2;
	}
	else
	{
		if(count < 88)
		{
			/* ��С��Чֵ����3*/
			return 3;
		}
		else
		{
			/* ������Чֵ,��λcm*/
			return ((count * 34) / 1000 / 2);
		}
	}
}
