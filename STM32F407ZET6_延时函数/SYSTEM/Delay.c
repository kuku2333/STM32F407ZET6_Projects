#include "Delay.h"

u32 my_us = 21;   	//��21MHZ����21��������ʱ1us
u32 my_ms = 21000;   //��21MHZ����21000��������ʱ1ms

void Delay_Init(void)
{
	//ѡ��Systick��ʱ����ʱ�� = HCLK/8 = 168MHZ/8 = 21MHZ
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

void Delay_us(uint32_t xus)
{
	u32 temp = 0x00;
	
	//������װֵ  SysTick->LOAD��24λ
	SysTick->LOAD = my_us*xus - 1; //21000
	
	//���ü�����ֵ0
	SysTick->VAL = 0x00;
	
	//������ʱ�� SysTick->LOADֵװ��SysTick->VAL
	SysTick->CTRL |= (0x01<<0); 
	
	do
	{
		temp = SysTick->CTRL;
		
		//(temp & (0x01<<0))�ж϶�ʱ���Ƿ���������رգ����˳�
		//�ж�temp��16λ�����Ϊ1,��ʾ�������� SysTick->LOAD������0
	}while((temp & (0x01<<0)) && !(temp&(0x01<<16)) );
	

	//�رն�ʱ��
	SysTick->CTRL &= ~(0x01<<0); 	
}

void Delay_ms(uint32_t xms)
{
	u32 temp = 0x00;
	
	//������װֵ  SysTick->LOAD��24λ
	SysTick->LOAD = my_ms*xms - 1; 
	
	//���ü�����ֵ0
	SysTick->VAL = 0x00;
	
	//������ʱ�� SysTick->LOADֵװ��SysTick->VAL
	SysTick->CTRL |= (0x01<<0); 
	
	do
	{
		temp = SysTick->CTRL;
		
		//(temp & (0x01<<0))�ж϶�ʱ���Ƿ���������رգ����˳�
		//�ж�temp��16λ�����Ϊ1,��ʾ�������� SysTick->LOAD������0
	}while((temp & (0x01<<0)) && !(temp&(0x01<<16)) );
	
	//�رն�ʱ��
	SysTick->CTRL &= ~(0x01<<0); 	
}

void Delay_s(int ns)
{
	int i;
	
	for(i=0; i<ns; i++)
	{
		Delay_ms(500);
		Delay_ms(500);
	}
}
