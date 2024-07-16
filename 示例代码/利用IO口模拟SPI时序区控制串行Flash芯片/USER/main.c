/**
   *********************************************************************************
   * @file    main.c 
   * @author  
   * @version 
   * @date    2024/07/16
   * @brief   利用IO口模拟SPI时序实现对存储IC进行读取操作，掌握SPI工作模式和时序分析
						  
					SCK  --  PB3  输出模式
					MOSI --  PB5  输出模式
					MISO --  PB4  输入模式
					CS   --  PB14 输出模式
												
   *********************************************************************************
**/

#include "stm32f4xx.h"  //必须包含
#include <stdio.h>
#include <stdbool.h>

//片选引脚
#define  W25Q128_CS(n)   	(n) ? GPIO_SetBits(GPIOB,GPIO_Pin_14) : GPIO_ResetBits(GPIOB,GPIO_Pin_14) 
#define  W25Q128_SCK(n)  	(n) ? GPIO_SetBits(GPIOB,GPIO_Pin_3)  : GPIO_ResetBits(GPIOB,GPIO_Pin_3) 
#define  W25Q128_MOSI(n)  (n) ? GPIO_SetBits(GPIOB,GPIO_Pin_5)  : GPIO_ResetBits(GPIOB,GPIO_Pin_5) 
#define  W25Q128_MISO		   GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)

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
	USART_InitStructure.USART_BaudRate = baud;														//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;										//数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;											//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;												//无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 				//无流控
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


//利用串口发送一个字符串
void  USART1_SendString(const char *str)
{
	while(*str)
	{
		 USART_SendData(USART1,*str++);
		 while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET );		
	}
}

//存储IC的初始化
void W25Q128_Config(void)
{
 
  GPIO_InitTypeDef GPIO_InitStructure;

	//打开GPIOB端口的时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
 
	//SCK -- PB3
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;					//输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //CS -- PB14   
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14;					  //输出模式 
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	//MOSI -- PB5
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5;					    //输出模式 
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//MISO -- PB4
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;					    //输入模式 
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  //片选引脚，空闲状态为高电平
  W25Q128_CS(1);
	
	//如果采用模式3，则SCK引脚空闲高电平
  W25Q128_SCK(1);
}

////发送一个字节，并且会得到一个字节    假设采用模式3   SCK引脚空闲高电平，第二个边沿锁存数据
//uint8_t W25Q128_SendByte(uint8_t byte)
//{
//	int i = 0; 
//	uint8_t data = 0;
//	
//	 //1.SCK引脚输出高电平
//	 W25Q128_SCK(1);
//	 delay_us(5);
//		
//	 //3.循环发送8次，每次发送一个bit  遵循MSB 高位先出
//	 for(i=0;i<8;i++)
//	 {
//		  //2.SCK引脚输出低电平，此时第一个边沿出现
//			W25Q128_SCK(0);
//			delay_us(5);
//		 
//		 //4.判断待发送的字节的最高位  ???? ???? & 1000 0000
//		 if( byte & 0x80 )
//		 {
//				W25Q128_MOSI(1);
//		 }
//		 else
//				W25Q128_MOSI(0);
//		 
//		 byte <<= 1;
//		 delay_us(5);
//		 
//		 //5.SCK引脚输出高电平，此时第二个边沿出现
//		 W25Q128_SCK(1);
//		 delay_us(5);
//		 
//		 //6.此时从机会响应一个bit，主机需要接收！
//		 data <<= 1;
//		 data |= W25Q128_MISO;
//		 delay_us(5);	
//	 }
//		
//	 return data;
//}

//发送一个字节，并且会得到一个字节    假设采用模式0   SCK引脚空闲低电平，第一个边沿锁存数据
uint8_t W25Q128_SendByte(uint8_t byte)
{
	int i = 0; 
	uint8_t data = 0;
	
	 //1.SCK引脚输出低电平
	 W25Q128_SCK(0);
	 delay_us(5);
		
	 //3.循环发送8次，每次发送一个bit  遵循MSB 高位先出
	 for(i=0;i<8;i++)
	 {
		 //4.判断待发送的字节的最高位  ???? ???? & 1000 0000
		 if( byte & 0x80 )
		 {
				W25Q128_MOSI(1);
		 }
		 else
				W25Q128_MOSI(0);
		 
		 byte <<= 1;
		 delay_us(5);
		 
	  //2.SCK引脚输出高电平，此时第一个边沿出现
		W25Q128_SCK(1);
		delay_us(5);

		 //6.此时从机会响应一个bit，主机需要接收！
		  data <<= 1;
		 data |= W25Q128_MISO;
		 delay_us(5);	
		 
		 //5.SCK引脚输出低电平，此时第二个边沿出现
		 W25Q128_SCK(0);
		 delay_us(5);
	 }
		
	 return data;
}


//读取设备ID   0x17
uint8_t W25Q128_ReadDeviceID(void)
{
	uint8_t device_id = 0;
	
	//1.把片选引脚拉低
	W25Q128_CS(0);
	
	//2.MCU发送指令  0x90
	W25Q128_SendByte(0x90);
	
	//3.MCU发送地址
	W25Q128_SendByte(0x00);
	W25Q128_SendByte(0x00);
	W25Q128_SendByte(0x00);

	//4.MCU提供脉冲，得到厂商ID 
	W25Q128_SendByte(0xFF);
	
	//5.MCU提供脉冲，得到设备ID
	device_id = W25Q128_SendByte(0xFF);
	
	//6.把片选引脚拉高
	W25Q128_CS(1);
	
	return device_id;
}

//写使能指令
void W25Q128_WriteEnable(void)
{
	//1.把片选引脚拉低
	W25Q128_CS(0);

	//2.发送写使能指令
	W25Q128_SendByte(0x06);

	//3.把片选引脚拉高
	W25Q128_CS(1);
}

//写失能指令
void W25Q128_WriteDisable(void)
{
	//1.把片选引脚拉低
	W25Q128_CS(0);

	//2.发送写失能指令
	W25Q128_SendByte(0x04);

	//3.把片选引脚拉高
	W25Q128_CS(1);
}

//读取状态寄存器1  
uint8_t  W25Q128_ReadStatusRegister1(void)
{	
	uint8_t status = 0;
	
	//1.把片选引脚拉低
	W25Q128_CS(0);
	
	//2.发送读取状态寄存器1的指令
	W25Q128_SendByte(0x05);
	
	//3.MCU提供脉冲，得到寄存器的值
	status = W25Q128_SendByte(0xFF);
	
	//4.把片选引脚拉高
	W25Q128_CS(1);
	
	return status;
}


//擦除扇区
void W25Q128_EraseSector(uint32_t SectorAddr)
{
  //1.发送写使能指令
  W25Q128_WriteEnable();

  //2.把片选引脚拉低
  W25Q128_CS(0);
	
  //3.发送写使能指令
  W25Q128_SendByte(0x20);
	
  //4.发送待擦除的扇区地址，分3次发 MSB
  W25Q128_SendByte((SectorAddr & 0xFF0000) >> 16);
  W25Q128_SendByte((SectorAddr & 0xFF00) >> 8);
  W25Q128_SendByte(SectorAddr & 0xFF);
	
  //5.把片选引脚拉高
  W25Q128_CS(1);

  //6.等待擦除完成  BUSY = 1 表示忙碌  BUSY = 0 表示空闲
  while( W25Q128_ReadStatusRegister1() & 0x01 );

  //7.发送写失能指令
  W25Q128_WriteDisable();
}

//页编程指令           待写入的数据缓冲区   可编程页的地址         待写入的数据大小  <= 256  
void W25Q128_PageProgram(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	//1.发送写使能指令
	W25Q128_WriteEnable();

	//2.把片选引脚拉低
	W25Q128_CS(0);

	//3.发送写使能指令
	W25Q128_SendByte(0x02);

	//4.发送待编程的可编程页地址，分3次发 MSB
	W25Q128_SendByte((WriteAddr & 0xFF0000) >> 16);
	W25Q128_SendByte((WriteAddr & 0xFF00) >> 8);
	W25Q128_SendByte(WriteAddr & 0xFF);

	//5.循环写入数据
	while(NumByteToWrite--)
	{
		W25Q128_SendByte(*pBuffer++);
	}
	
	//6.把片选引脚拉高
	W25Q128_CS(1);
	
	//7.等待写入完成  BUSY = 1 表示忙碌  BUSY = 0 表示空闲
	while( W25Q128_ReadStatusRegister1() & 0x01 );
	
	//8.发送写失能指令
	W25Q128_WriteDisable();
}	

//读取数据              存储数据的缓冲区   存储单元地址        需要读取的字节数量
void W25Q128_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  
	//1.把片选引脚拉低
	W25Q128_CS(0);

	//2.发送写使能指令
	W25Q128_SendByte(0x03);

	//3.发送待编程的可编程页地址，分3次发 MSB
	W25Q128_SendByte((ReadAddr & 0xFF0000) >> 16);
	W25Q128_SendByte((ReadAddr & 0xFF00) >> 8);
	W25Q128_SendByte(ReadAddr & 0xFF);
	
	//4.循环读取
	while (NumByteToRead--) 
	{
		//5.MCU提供脉冲，目的是读取从机的数据
		*pBuffer = W25Q128_SendByte(0xFF);
		pBuffer++; //缓冲区地址向后偏移
	}

	//6.把片选引脚拉高
	W25Q128_CS(1);
}


int main()
{
	uint8_t readbuf[11] = {0};
	
	//1.硬件的初始化
	USART1_Config(9600);
	W25Q128_Config();
	
	//2.擦除一个扇区  0x 00  0  0  00
	W25Q128_EraseSector(0x000000);
	
	//3.向擦除的扇区中的第0页写入数据
	W25Q128_PageProgram("hello world",0x000000,11);
	
	//4.从存储IC的指定地址下读取数据
	W25Q128_ReadBuffer(readbuf,0x000000,11);
	
	//5.输出读取的数据
	USART1_SendString((char *)readbuf);
	
	while(1)
	{
		
	}
}




