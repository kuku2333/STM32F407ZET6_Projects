/**
   ******************************************************************************
   * @file    STM32F407ZET6_�����걸��\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-09
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include "sys.h"
#include "Gpio.h"
#include "DHT11.h"
#include <stdio.h>

//ǰ̨��������жϷ�����򣬸ó����ǲ���Ҫ�ֶ����õģ����жϴ���֮��CPU���Զ���ת����ִ�иú���
void USART1_IRQHandler(void)
{
	uint8_t data;
  //�ж��ж��Ƿ���
  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
  {   
		//��USART1�н���һ���ֽ�
		data = USART_ReceiveData(USART1);  //һ��ֻ�ܽ���һ���ֽ�   
		
		//�ѽ��յ�������ת����ȥ
		USART_SendData(USART1,data);
  }
}

void USART1_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//����GPIO�˿�ʱ��  PA9��PA10
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//��USART1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//ѡ��GPIO���ŵĸ��ù���
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9 , GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	//����GPIO���� ע�⣺����ģʽ
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9|GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//���ô��ڲ���+�Դ��ڳ�ʼ��
	USART_InitStructure.USART_BaudRate = baud;																			//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;													//ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;															//��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);

	//����NVIC���� + ��NVIC��ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//ѡ��USART1���ж�Դ�����յ������򴥷��ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//�򿪴���
	USART_Cmd(USART1, ENABLE);
}

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f)
{
	USART_SendData(USART1,ch);	
	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET );
		
	return ch;
}

//���ô��ڷ���һ���ַ���
void  USART1_SendString(const char *str)
{
	while(*str)
	{
		while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET );
		 USART_SendData(USART1,*str++);
		 		
	}
}



/*PG9����Ĭ�ϸߵ�ƽ�����ⲿ���������ṩ��
Ȼ������18ms���ٸߵ�ƽ30us��Ȼ���л���
����ģʽ�����DHT���źţ���80us�ĵ͵�ƽ��
��80us�ĸߵ�ƽ��Ȼ��������ݽ��ս׶Ρ�*/
int main(void)
{
	FunctionalState flag;
	uint32_t count;
	uint8_t data[5] ={0};
	
	USART1_Config(9600);
	
	GPIO_Out_Init(RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_9, ENABLE);
	dht11_init();
	GPIO_In_Init(RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_9, ENABLE);
	
	
	while(1)
	{
		flag = judge_DHT_resp();
		if(flag != DISABLE)
		{
			
		}
		else
		{
			continue;
		}
		
		for(uint8_t i=0; i < 5; i++)
		{
			for(uint8_t j=0; j < 8; j++)
			{
				count = DHT_recv_data();
				if(count >= 70)
				{
					data[j] |= (0x01 << j-1);
				}
			}
			
		}
		
		for(uint8_t i=0; i < 5; i++)
		{
			printf("data_%d is:%d\r\n", i, data[i]);
		}
		//printf("data_0 is:%d\r\n",data[1]);
		
	}
}
