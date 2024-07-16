/**
   *********************************************************************************
   * @file    main.c 
   * @author  
   * @version 
   * @date    2024/07/16
   * @brief   ����IO��ģ��SPIʱ��ʵ�ֶԴ洢IC���ж�ȡ����������SPI����ģʽ��ʱ�����
						  
					SCK  --  PB3  ���ģʽ
					MOSI --  PB5  ���ģʽ
					MISO --  PB4  ����ģʽ
					CS   --  PB14 ���ģʽ
												
   *********************************************************************************
**/

#include "stm32f4xx.h"  //�������
#include <stdio.h>
#include <stdbool.h>

//Ƭѡ����
#define  W25Q128_CS(n)   	(n) ? GPIO_SetBits(GPIOB,GPIO_Pin_14) : GPIO_ResetBits(GPIOB,GPIO_Pin_14) 
#define  W25Q128_SCK(n)  	(n) ? GPIO_SetBits(GPIOB,GPIO_Pin_3)  : GPIO_ResetBits(GPIOB,GPIO_Pin_3) 
#define  W25Q128_MOSI(n)  (n) ? GPIO_SetBits(GPIOB,GPIO_Pin_5)  : GPIO_ResetBits(GPIOB,GPIO_Pin_5) 
#define  W25Q128_MISO		   GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)

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




//��ʱ΢�� ע�⣺Systick��24bit�ĵݼ�������  Լ����798915us,���Բ��������Գ������ֵ
void delay_us(u32 nus)
{
	SysTick->CTRL = 0; 						 // �����״̬�Ĵ�����д��0��Ŀ�Ĺر�ϵͳ��શ�ʱ��
	SysTick->LOAD = (nus * 21) - 1;// ָ���Ǽ�����������ʱʱ�� = �������� * ��������
	SysTick->VAL  = 0; 						 // �����ǰ��ֵ�Ĵ�����ֵ
	SysTick->CTRL = 1; 						 // �����˶�ʱ�������Ҷ�ʱ����ʱ��Դѡ����21MHZ--> �������� = 1/21us
	while ((SysTick->CTRL & 0x00010000)==0);//�ȴ���ʱʱ�䵽��
	SysTick->CTRL = 0; 						 // �����״̬�Ĵ�����д��0��Ŀ�Ĺر�ϵͳ��શ�ʱ��

}

//��ʱ���� ע�⣺Systick��24bit�ĵݼ�������  Լ����798ms,���Բ��������Գ������ֵ
void delay_ms(u32 nms)
{
	SysTick->CTRL = 0; 						 			// �����״̬�Ĵ�����д��0��Ŀ�Ĺر�ϵͳ��શ�ʱ��
	SysTick->LOAD = (nms * 21*1000) - 1;// ָ���Ǽ�����������ʱʱ�� = �������� * ��������
	SysTick->VAL  = 0; 						 			// �����ǰ��ֵ�Ĵ�����ֵ
	SysTick->CTRL = 1; 						 			// �����˶�ʱ�������Ҷ�ʱ����ʱ��Դѡ����21MHZ--> �������� = 1/21us
	while ((SysTick->CTRL & 0x00010000)==0);//�ȴ���ʱʱ�䵽��
	SysTick->CTRL = 0; 						 			// �����״̬�Ĵ�����д��0��Ŀ�Ĺر�ϵͳ��શ�ʱ��
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
	USART_InitStructure.USART_BaudRate = baud;														//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;										//����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;											//ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;												//��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 				//������
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


//���ô��ڷ���һ���ַ���
void  USART1_SendString(const char *str)
{
	while(*str)
	{
		 USART_SendData(USART1,*str++);
		 while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET );		
	}
}

//�洢IC�ĳ�ʼ��
void W25Q128_Config(void)
{
 
  GPIO_InitTypeDef GPIO_InitStructure;

	//��GPIOB�˿ڵ�ʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
 
	//SCK -- PB3
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;					//���ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //CS -- PB14   
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14;					  //���ģʽ 
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	//MOSI -- PB5
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5;					    //���ģʽ 
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//MISO -- PB4
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;					    //����ģʽ 
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  //Ƭѡ���ţ�����״̬Ϊ�ߵ�ƽ
  W25Q128_CS(1);
	
	//�������ģʽ3����SCK���ſ��иߵ�ƽ
  W25Q128_SCK(1);
}

////����һ���ֽڣ����һ�õ�һ���ֽ�    �������ģʽ3   SCK���ſ��иߵ�ƽ���ڶ���������������
//uint8_t W25Q128_SendByte(uint8_t byte)
//{
//	int i = 0; 
//	uint8_t data = 0;
//	
//	 //1.SCK��������ߵ�ƽ
//	 W25Q128_SCK(1);
//	 delay_us(5);
//		
//	 //3.ѭ������8�Σ�ÿ�η���һ��bit  ��ѭMSB ��λ�ȳ�
//	 for(i=0;i<8;i++)
//	 {
//		  //2.SCK��������͵�ƽ����ʱ��һ�����س���
//			W25Q128_SCK(0);
//			delay_us(5);
//		 
//		 //4.�жϴ����͵��ֽڵ����λ  ???? ???? & 1000 0000
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
//		 //5.SCK��������ߵ�ƽ����ʱ�ڶ������س���
//		 W25Q128_SCK(1);
//		 delay_us(5);
//		 
//		 //6.��ʱ�ӻ�����Ӧһ��bit��������Ҫ���գ�
//		 data <<= 1;
//		 data |= W25Q128_MISO;
//		 delay_us(5);	
//	 }
//		
//	 return data;
//}

//����һ���ֽڣ����һ�õ�һ���ֽ�    �������ģʽ0   SCK���ſ��е͵�ƽ����һ��������������
uint8_t W25Q128_SendByte(uint8_t byte)
{
	int i = 0; 
	uint8_t data = 0;
	
	 //1.SCK��������͵�ƽ
	 W25Q128_SCK(0);
	 delay_us(5);
		
	 //3.ѭ������8�Σ�ÿ�η���һ��bit  ��ѭMSB ��λ�ȳ�
	 for(i=0;i<8;i++)
	 {
		 //4.�жϴ����͵��ֽڵ����λ  ???? ???? & 1000 0000
		 if( byte & 0x80 )
		 {
				W25Q128_MOSI(1);
		 }
		 else
				W25Q128_MOSI(0);
		 
		 byte <<= 1;
		 delay_us(5);
		 
	  //2.SCK��������ߵ�ƽ����ʱ��һ�����س���
		W25Q128_SCK(1);
		delay_us(5);

		 //6.��ʱ�ӻ�����Ӧһ��bit��������Ҫ���գ�
		  data <<= 1;
		 data |= W25Q128_MISO;
		 delay_us(5);	
		 
		 //5.SCK��������͵�ƽ����ʱ�ڶ������س���
		 W25Q128_SCK(0);
		 delay_us(5);
	 }
		
	 return data;
}


//��ȡ�豸ID   0x17
uint8_t W25Q128_ReadDeviceID(void)
{
	uint8_t device_id = 0;
	
	//1.��Ƭѡ��������
	W25Q128_CS(0);
	
	//2.MCU����ָ��  0x90
	W25Q128_SendByte(0x90);
	
	//3.MCU���͵�ַ
	W25Q128_SendByte(0x00);
	W25Q128_SendByte(0x00);
	W25Q128_SendByte(0x00);

	//4.MCU�ṩ���壬�õ�����ID 
	W25Q128_SendByte(0xFF);
	
	//5.MCU�ṩ���壬�õ��豸ID
	device_id = W25Q128_SendByte(0xFF);
	
	//6.��Ƭѡ��������
	W25Q128_CS(1);
	
	return device_id;
}

//дʹ��ָ��
void W25Q128_WriteEnable(void)
{
	//1.��Ƭѡ��������
	W25Q128_CS(0);

	//2.����дʹ��ָ��
	W25Q128_SendByte(0x06);

	//3.��Ƭѡ��������
	W25Q128_CS(1);
}

//дʧ��ָ��
void W25Q128_WriteDisable(void)
{
	//1.��Ƭѡ��������
	W25Q128_CS(0);

	//2.����дʧ��ָ��
	W25Q128_SendByte(0x04);

	//3.��Ƭѡ��������
	W25Q128_CS(1);
}

//��ȡ״̬�Ĵ���1  
uint8_t  W25Q128_ReadStatusRegister1(void)
{	
	uint8_t status = 0;
	
	//1.��Ƭѡ��������
	W25Q128_CS(0);
	
	//2.���Ͷ�ȡ״̬�Ĵ���1��ָ��
	W25Q128_SendByte(0x05);
	
	//3.MCU�ṩ���壬�õ��Ĵ�����ֵ
	status = W25Q128_SendByte(0xFF);
	
	//4.��Ƭѡ��������
	W25Q128_CS(1);
	
	return status;
}


//��������
void W25Q128_EraseSector(uint32_t SectorAddr)
{
  //1.����дʹ��ָ��
  W25Q128_WriteEnable();

  //2.��Ƭѡ��������
  W25Q128_CS(0);
	
  //3.����дʹ��ָ��
  W25Q128_SendByte(0x20);
	
  //4.���ʹ�������������ַ����3�η� MSB
  W25Q128_SendByte((SectorAddr & 0xFF0000) >> 16);
  W25Q128_SendByte((SectorAddr & 0xFF00) >> 8);
  W25Q128_SendByte(SectorAddr & 0xFF);
	
  //5.��Ƭѡ��������
  W25Q128_CS(1);

  //6.�ȴ��������  BUSY = 1 ��ʾæµ  BUSY = 0 ��ʾ����
  while( W25Q128_ReadStatusRegister1() & 0x01 );

  //7.����дʧ��ָ��
  W25Q128_WriteDisable();
}

//ҳ���ָ��           ��д������ݻ�����   �ɱ��ҳ�ĵ�ַ         ��д������ݴ�С  <= 256  
void W25Q128_PageProgram(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	//1.����дʹ��ָ��
	W25Q128_WriteEnable();

	//2.��Ƭѡ��������
	W25Q128_CS(0);

	//3.����дʹ��ָ��
	W25Q128_SendByte(0x02);

	//4.���ʹ���̵Ŀɱ��ҳ��ַ����3�η� MSB
	W25Q128_SendByte((WriteAddr & 0xFF0000) >> 16);
	W25Q128_SendByte((WriteAddr & 0xFF00) >> 8);
	W25Q128_SendByte(WriteAddr & 0xFF);

	//5.ѭ��д������
	while(NumByteToWrite--)
	{
		W25Q128_SendByte(*pBuffer++);
	}
	
	//6.��Ƭѡ��������
	W25Q128_CS(1);
	
	//7.�ȴ�д�����  BUSY = 1 ��ʾæµ  BUSY = 0 ��ʾ����
	while( W25Q128_ReadStatusRegister1() & 0x01 );
	
	//8.����дʧ��ָ��
	W25Q128_WriteDisable();
}	

//��ȡ����              �洢���ݵĻ�����   �洢��Ԫ��ַ        ��Ҫ��ȡ���ֽ�����
void W25Q128_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  
	//1.��Ƭѡ��������
	W25Q128_CS(0);

	//2.����дʹ��ָ��
	W25Q128_SendByte(0x03);

	//3.���ʹ���̵Ŀɱ��ҳ��ַ����3�η� MSB
	W25Q128_SendByte((ReadAddr & 0xFF0000) >> 16);
	W25Q128_SendByte((ReadAddr & 0xFF00) >> 8);
	W25Q128_SendByte(ReadAddr & 0xFF);
	
	//4.ѭ����ȡ
	while (NumByteToRead--) 
	{
		//5.MCU�ṩ���壬Ŀ���Ƕ�ȡ�ӻ�������
		*pBuffer = W25Q128_SendByte(0xFF);
		pBuffer++; //��������ַ���ƫ��
	}

	//6.��Ƭѡ��������
	W25Q128_CS(1);
}


int main()
{
	uint8_t readbuf[11] = {0};
	
	//1.Ӳ���ĳ�ʼ��
	USART1_Config(9600);
	W25Q128_Config();
	
	//2.����һ������  0x 00  0  0  00
	W25Q128_EraseSector(0x000000);
	
	//3.������������еĵ�0ҳд������
	W25Q128_PageProgram("hello world",0x000000,11);
	
	//4.�Ӵ洢IC��ָ����ַ�¶�ȡ����
	W25Q128_ReadBuffer(readbuf,0x000000,11);
	
	//5.�����ȡ������
	USART1_SendString((char *)readbuf);
	
	while(1)
	{
		
	}
}




