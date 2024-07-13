/**
   ******************************************************************************
   * @file    STM32F407ZET6_外设完备版\USER\main.c 
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

//前台程序就是中断服务程序，该程序是不需要手动调用的，当中断触发之后CPU会自动跳转过来执行该函数
void USART1_IRQHandler(void)
{
	uint8_t data;
  //判断中断是否发生
  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
  {   
		//从USART1中接收一个字节
		data = USART_ReceiveData(USART1);  //一次只能接收一个字节   
		
		//把接收到的数据转发出去
		USART_SendData(USART1,data);
  }
}

void USART1_Config(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//打开了GPIO端口时钟  PA9和PA10
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//打开USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//选择GPIO引脚的复用功能
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9 , GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	//配置GPIO引脚 注意：复用模式
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9|GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//配置串口参数+对串口初始化
	USART_InitStructure.USART_BaudRate = baud;																			//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;													//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;															//无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//收发模式
	USART_Init(USART1, &USART_InitStructure);

	//配置NVIC参数 + 对NVIC初始化
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//选择USART1的中断源，接收到数据则触发中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//打开串口
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

//利用串口发送一个字符串
void  USART1_SendString(const char *str)
{
	while(*str)
	{
		while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET );
		 USART_SendData(USART1,*str++);
		 		
	}
}



/*PG9先是默认高电平，有外部上拉电阻提供，
然后拉低18ms，再高电平30us，然后切换到
输入模式，检测DHT的信号，先80us的低电平，
再80us的高电平，然后进入数据接收阶段。*/
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
