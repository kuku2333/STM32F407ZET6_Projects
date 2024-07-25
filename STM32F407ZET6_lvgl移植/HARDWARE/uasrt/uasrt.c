/**
   ******************************************************************************
   * @file    STM32F407ZET6_串口通信_发送字符串\HARDWARE\Uasrt_Send_String.c
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-05
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#include "uasrt.h"

static USART_InitTypeDef   		USART_InitStructure;
static GPIO_InitTypeDef 		GPIO_InitStructure;
static NVIC_InitTypeDef   		NVIC_InitStructure;

volatile uint8_t  g_usart1_rx_buf[1280];
volatile uint32_t g_usart1_rx_cnt=0;
volatile uint32_t g_usart1_rx_end=0;

/**
	*****************************************************************************
	*@brief 用串口1与电脑进行通信，映射的引脚为TX--PA0，RX--PA10
	*@steps 1.打开串口1的时钟和对应引脚的时钟
	*		2.用GPIO_PinAFConfig()函数配置引脚的复用功能
	*		3.配置引脚结构体，功能选服用，引脚初始化结构体
	*		4.配置串口结构体并用初始化函数进行初始化
	*		6.配置NVIC，然后要使用这个USART_ITConfig()函数
	*		7.使能串口
	*****************************************************************************
**/
void Uasrt1_Init(void)
{
//	USART_InitTypeDef USART_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;
//    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

	/* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
    /* Enable USART */
    USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void)
{
	uint8_t d=0;
	
	//检测是否接收到数据
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		d=USART_ReceiveData(USART1);
		
		
		g_usart1_rx_buf[g_usart1_rx_cnt++]=d;
	
		if(g_usart1_rx_cnt >= sizeof g_usart1_rx_buf)
		{
			g_usart1_rx_end=1;
		}			
	
		//清空标志位，可以响应新的中断请求
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

void Send_String(char *string)
{
	uint8_t i = 0;
	char temp;
	
	while(1)
	{
		temp = string[i];
		USART_SendData(USART1, temp);
		i++;
		Delay_ms(10);
		if(temp == 0x00 || temp == 0xFF)
		{
			break;
		}	
	}
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
