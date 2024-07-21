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

#include "Uasrt_Send_String.h"

/**
	*****************************************************************************
	*@brief 用串口1与电脑进行通信，映射的引脚为TX--PA9，RX--PA10
	*@steps 1.打开串口1的时钟和对应引脚的时钟
	*		2.用GPIO_PinAFConfig()函数配置引脚的复用功能
	*		3.配置引脚结构体，功能选服用，引脚初始化结构体
	*		4.配置串口结构体并用初始化函数进行初始化
	*		6.配置NVIC，然后要使用这个USART_ITConfig()函数
	*		7.使能串口
	*****************************************************************************
**/

uint8_t buffer[100]; // 根据需要调整大小
uint8_t buffer_index = 0;

/*	uart2:tx-->pa2	rx-->pa3d
	串口2连接蓝牙模块，手机蓝牙连接蓝牙模块后，发送数据到
	蓝牙模块，蓝牙模块再通过串口2把数据给MCU，MCU再通过串口
	1把数据发送到电脑。这是测试。
	最终只需要数据到了MCU后，存起来，然后解析指令，再判断需要
	做什么动作。注意，解析指令的动作在中断服务函数中，所以动作
	太长的话，记得用标志位。
*/
void Uasrt2_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

	/* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
    /* Enable USART */
    USART_Cmd(USART2, ENABLE);
}

void USART2_IRQHandler(void)
{
	uint8_t data;
	
	/* USART in Receiver mode */
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
		//printf("receive\r\n");
        /* Receive Transaction data */
		data = USART_ReceiveData(USART2);
		//printf("data: %c\r\n", data);
		//USART_SendData(USART1, data);
		
		// 读取接收到的字节
        //uint8_t data = USART_ReceiveData(USART2);

        // 检查是否为结束符，例如'\n'或'\r'
        if(data == 'n' || data == '\r') 
		{
            // 结束符，处理命令
            buffer[buffer_index] = '\0'; // 添加字符串结束符
            buffer_index = 0; // 重置缓冲区索引
			//printf("ok1\r\n");
            process_command(buffer); // 处理命令
        } 
		else 
		{
            // 将数据存储到缓冲区
            if(buffer_index < sizeof(buffer) - 1) 
			{
                buffer[buffer_index++] = data;
            }
		}	
    }
	
	//USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

/* 	判断是什么命令*/
void process_command(uint8_t *cmd)
{
	// 检查命令格式是否为"SETTIME:HH:MM:SS"
    if(strstr((char *)cmd, "SETTIME:") == cmd) 
	{
		uint8_t *pos = strstr((char *)cmd, "SETTIME:");
        // 解析时间并设置RTC
        //RTC_TimeTypeDef RTC_TimeStructure;
        // 假设函数parse_time成功解析时间并填充RTC_TimeStructure
        if(parse_time((char *)(pos + strlen("SETTIME:")), &RTC_TimeStructure)) 
		{
            RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
			RTC_ShowTime();
        }
		//RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
    }
}

/* 
	解析出字符串中的时间
	ex:SETTIME:125500
*/
bool parse_time(char *time_str, RTC_TimeTypeDef *RTC_TimeStructure)
{
	uint8_t hours, minutes, seconds;
	
	sscanf(time_str, "%2d%2d%2d", &hours, &minutes, &seconds);
	
	RTC_TimeStructure->RTC_Hours		=	hours; 
	RTC_TimeStructure->RTC_Minutes		=	minutes;
	RTC_TimeStructure->RTC_Seconds		=	seconds;
	
	return true;
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

void Uasrt1_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	
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
	uint8_t data;
	/* USART in Receiver mode */
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        /* Receive Transaction data */
		data = USART_ReceiveData(USART1);
		USART_SendData(USART1, data);
    }
}

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, ch);	
	while( USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
		
	return ch;
}
