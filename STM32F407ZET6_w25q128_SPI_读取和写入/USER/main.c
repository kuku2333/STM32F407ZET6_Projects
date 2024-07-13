/**
   ******************************************************************************
   * @file    main.c 
   * @author  
   * @version 
   * @date    2024/06/28
   * @brief   实现利用USART1实现和计算机进行有线通信，所以USART1需要使用2个IO口
						  
							USART1_TX  -- PA9
							USART1_RX  -- PA10
							
   ******************************************************************************
**/

#include "stm32f4xx.h"  //必须包含
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




//延时微秒 注意：Systick是24bit的递减计数器  约等于798915us,所以参数不可以超过这个值
void delay_us(u32 nus)
{
	SysTick->CTRL = 0; 						 // 向控制状态寄存器中写入0，目的关闭系统嘀嗒定时器
	SysTick->LOAD = (nus * 21) - 1;// 指的是计数次数，定时时间 = 计数次数 * 计数周期
	SysTick->VAL  = 0; 						 // 清除当前数值寄存器的值
	SysTick->CTRL = 1; 						 // 开启了定时器，并且定时器的时钟源选择了21MHZ--> 计数周期 = 1/21us
	while ((SysTick->CTRL & 0x00010000)==0);//等待延时时间到达
	SysTick->CTRL = 0; 						 // 向控制状态寄存器中写入0，目的关闭系统嘀嗒定时器

}

//延时毫秒 注意：Systick是24bit的递减计数器  约等于798ms,所以参数不可以超过这个值
void delay_ms(u32 nms)
{
	SysTick->CTRL = 0; 						 			// 向控制状态寄存器中写入0，目的关闭系统嘀嗒定时器
	SysTick->LOAD = (nms * 21*1000) - 1;// 指的是计数次数，定时时间 = 计数次数 * 计数周期
	SysTick->VAL  = 0; 						 			// 清除当前数值寄存器的值
	SysTick->CTRL = 1; 						 			// 开启了定时器，并且定时器的时钟源选择了21MHZ--> 计数周期 = 1/21us
	while ((SysTick->CTRL & 0x00010000)==0);//等待延时时间到达
	SysTick->CTRL = 0; 						 			// 向控制状态寄存器中写入0，目的关闭系统嘀嗒定时器
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
uint8_t SPI1_SendByte(uint8_t byte)
{
	/*等待SPI1发送缓冲区为空 */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/*使用SPI1发送字节 */
  SPI_I2S_SendData(SPI1, byte);

  /*等待接收字节完成 */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /*返回接收到的字节 */
  return SPI_I2S_ReceiveData(SPI1);
}


void  w25q128_init(void)
{	
	SPI_InitTypeDef   SPI_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* 使能硬件时钟:端口硬件时钟、SPI1硬件时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	
	/* 将引脚配置为复用功能模式，并连接到SPI1 */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3 , GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4 , GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5 , GPIO_AF_SPI1);

	
	/* 将（芯）片选（择）引脚配置为输出模式，初始电平状态为高电平，看数据手册 */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
	
	/* 配置SPI1的相关参数：CPOL、CPHA、全双工、速度、最高/最低有效位优先传输.... */
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//两线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主机角色
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//看数据手册，使用8bit
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//时钟线在总线空闲的时候为高电平，CPOL=1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//主机读取MISO引脚电平在第二边沿，CPHA=1
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//通过软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//时钟线的波特率=84MHz/4=21MHz=21Mbps
	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//看数据手册，最高有效位优先传输
	SPI_Init(SPI1, &SPI_InitStructure);
	
	
	/* 使能SPI1硬件工作 */
	SPI_Cmd(SPI1, ENABLE);
}


void w25q128_write_enable(void)
{
	/* 片选引脚电平拉低，开始通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);		

	/* 发送指令0x06 */
	SPI1_SendByte(0x06);	
	
	/* 片选引脚电平拉高，结束通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);		
}

void w25q128_write_disable(void)
{
	/* 片选引脚电平拉低，开始通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);		

	/* 发送指令0x04 */
	SPI1_SendByte(0x04);	
	
	/* 片选引脚电平拉高，结束通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);		
}

uint8_t w25q128_read_status1(void)
{
	uint8_t sta=0;
	
	/* 片选引脚电平拉低，开始通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);	
	
	/* 发送指令0x05 */
	SPI1_SendByte(0x05);	
	
	/* 读取状态寄存器1的数据 */
	sta=SPI1_SendByte(0xFF);	

	/* 片选引脚电平拉高，结束通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
	
	return sta;
}

void w25q128_erase_sector(uint32_t sector_addr)
{
	uint8_t sta;
	
	w25q128_write_enable();
	
	
	/* 片选引脚电平拉低，开始通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);		

	/* 发送指令0x20 */
	SPI1_SendByte(0x20);	
	
	
	/* 发送24bit地址 */
	SPI1_SendByte((sector_addr>>16)&0xFF);
	SPI1_SendByte((sector_addr>>8)&0xFF);
	SPI1_SendByte(sector_addr & 0xFF);	
	
	/* 片选引脚电平拉高，结束通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);	

	
	//访问状态寄存器1，检测当前是否擦除完毕
	while(1)
	{
		sta=w25q128_read_status1();
		//检测BUSY位是否由1->0
		if((sta & 0x01) == 0)
			break;
		
		delay_ms(1);
	}

	//对spi flash 上锁
	w25q128_write_disable();
}


void w25q128_read_id(uint8_t *mid,uint8_t *did)
{
	/* 片选引脚电平拉低，开始通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);

	/* 发送指令0x90 */
	SPI1_SendByte(0x90);
	
	
	/* 发送24bit，值全为0 */
	SPI1_SendByte(0x00);
	SPI1_SendByte(0x00);
	SPI1_SendByte(0x00);

	/* 读取厂商ID，传递参数可以任意值 */
	*mid=SPI1_SendByte(0x10);
	
	/* 读取设备ID */
	*did=SPI1_SendByte(0x66);	
	
	/* 片选引脚电平拉高，结束通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);	
}
void w25q128_read(uint32_t addr,uint8_t *buf,uint32_t len)
{
	uint8_t *p=buf;
	
	/* 片选引脚电平拉低，开始通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);

	/* 发送指令0x03 */
	SPI1_SendByte(0x03);

	/* 发送24bit地址 */
	SPI1_SendByte((addr>>16)&0xFF);
	SPI1_SendByte((addr>>8) &0xFF);
	SPI1_SendByte( addr&0xFF);


	/* 读取数据 */
	while(len--)
	{
		*p=SPI1_SendByte(0x10);
		
		p++;
	}
	/* 片选引脚电平拉高，结束通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);	
}

void w25q128_write(uint32_t addr, uint8_t *msg)
{
	/* 片选引脚电平拉低，开始通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
	
	// 发送写使能命令
    SPI1_SendByte(0x06);
	
	/* 片选引脚电平拉高，结束通信 */
    GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
	
	/* 片选引脚电平拉低，开始通信 */
    GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);
	
	/* 发送指令0x02 */
	SPI1_SendByte(0x02);
	
	/* 发送24bit地址 */
	SPI1_SendByte((addr>>16)&0xFF);
	SPI1_SendByte((addr>>8) &0xFF);
	SPI1_SendByte( addr&0xFF);
	
	/* 写数据*/
	for(uint8_t i = 0; i < 64; i++)
	{
		SPI1_SendByte(*(msg+i));	
	}
	
	/* 片选引脚电平拉高，结束通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);	
	
	/* 片选引脚电平拉低，开始通信 */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
	
	// 发送写使能命令
    SPI1_SendByte(0x04);
	
	/* 片选引脚电平拉高，结束通信 */
    GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
}

int main()
{
	uint8_t mid,did;
	uint8_t buf[64]={0};
	uint8_t msg[64]={'H','e','l','l','o'};
	int32_t i;
	
	//1.硬件的初始化
	USART1_Config(9600);
	
	w25q128_init();
	
	w25q128_read_id(&mid,&did);
	
	printf("mid=%02X,did=%02X\r\n",mid,did);
	
	w25q128_read(0,buf,sizeof buf);
	
	printf("read data:");
	for(i=0; i<sizeof buf; i++)
	{
		printf("%02X ",buf[i]);
	}
	printf("\r\n");	
	
	w25q128_erase_sector(0);
	
	w25q128_write(0, msg);
	
	w25q128_read(0,buf,sizeof buf);
	
	printf("read data:");
	for(i=0; i<sizeof buf; i++)
	{
		printf("%02X ",buf[i]);
	}
	printf("\r\n");	
	
	while(1)
	{

	}
}




