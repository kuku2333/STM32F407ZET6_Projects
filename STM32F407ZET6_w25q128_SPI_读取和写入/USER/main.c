/**
   ******************************************************************************
   * @file    main.c 
   * @author  
   * @version 
   * @date    2024/06/28
   * @brief   ʵ������USART1ʵ�ֺͼ������������ͨ�ţ�����USART1��Ҫʹ��2��IO��
						  
							USART1_TX  -- PA9
							USART1_RX  -- PA10
							
   ******************************************************************************
**/

#include "stm32f4xx.h"  //�������
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
uint8_t SPI1_SendByte(uint8_t byte)
{
	/*�ȴ�SPI1���ͻ�����Ϊ�� */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/*ʹ��SPI1�����ֽ� */
  SPI_I2S_SendData(SPI1, byte);

  /*�ȴ������ֽ���� */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /*���ؽ��յ����ֽ� */
  return SPI_I2S_ReceiveData(SPI1);
}


void  w25q128_init(void)
{	
	SPI_InitTypeDef   SPI_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* ʹ��Ӳ��ʱ��:�˿�Ӳ��ʱ�ӡ�SPI1Ӳ��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	
	/* ����������Ϊ���ù���ģʽ�������ӵ�SPI1 */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3 , GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4 , GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5 , GPIO_AF_SPI1);

	
	/* ����о��Ƭѡ������������Ϊ���ģʽ����ʼ��ƽ״̬Ϊ�ߵ�ƽ���������ֲ� */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
	
	/* ����SPI1����ز�����CPOL��CPHA��ȫ˫�����ٶȡ����/�����Чλ���ȴ���.... */
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//����ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//������ɫ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//�������ֲᣬʹ��8bit
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//ʱ���������߿��е�ʱ��Ϊ�ߵ�ƽ��CPOL=1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//������ȡMISO���ŵ�ƽ�ڵڶ����أ�CPHA=1
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//ͨ���������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//ʱ���ߵĲ�����=84MHz/4=21MHz=21Mbps
	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//�������ֲᣬ�����Чλ���ȴ���
	SPI_Init(SPI1, &SPI_InitStructure);
	
	
	/* ʹ��SPI1Ӳ������ */
	SPI_Cmd(SPI1, ENABLE);
}


void w25q128_write_enable(void)
{
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);		

	/* ����ָ��0x06 */
	SPI1_SendByte(0x06);	
	
	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);		
}

void w25q128_write_disable(void)
{
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);		

	/* ����ָ��0x04 */
	SPI1_SendByte(0x04);	
	
	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);		
}

uint8_t w25q128_read_status1(void)
{
	uint8_t sta=0;
	
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);	
	
	/* ����ָ��0x05 */
	SPI1_SendByte(0x05);	
	
	/* ��ȡ״̬�Ĵ���1������ */
	sta=SPI1_SendByte(0xFF);	

	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
	
	return sta;
}

void w25q128_erase_sector(uint32_t sector_addr)
{
	uint8_t sta;
	
	w25q128_write_enable();
	
	
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);		

	/* ����ָ��0x20 */
	SPI1_SendByte(0x20);	
	
	
	/* ����24bit��ַ */
	SPI1_SendByte((sector_addr>>16)&0xFF);
	SPI1_SendByte((sector_addr>>8)&0xFF);
	SPI1_SendByte(sector_addr & 0xFF);	
	
	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);	

	
	//����״̬�Ĵ���1����⵱ǰ�Ƿ�������
	while(1)
	{
		sta=w25q128_read_status1();
		//���BUSYλ�Ƿ���1->0
		if((sta & 0x01) == 0)
			break;
		
		delay_ms(1);
	}

	//��spi flash ����
	w25q128_write_disable();
}


void w25q128_read_id(uint8_t *mid,uint8_t *did)
{
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);

	/* ����ָ��0x90 */
	SPI1_SendByte(0x90);
	
	
	/* ����24bit��ֵȫΪ0 */
	SPI1_SendByte(0x00);
	SPI1_SendByte(0x00);
	SPI1_SendByte(0x00);

	/* ��ȡ����ID�����ݲ�����������ֵ */
	*mid=SPI1_SendByte(0x10);
	
	/* ��ȡ�豸ID */
	*did=SPI1_SendByte(0x66);	
	
	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);	
}
void w25q128_read(uint32_t addr,uint8_t *buf,uint32_t len)
{
	uint8_t *p=buf;
	
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);

	/* ����ָ��0x03 */
	SPI1_SendByte(0x03);

	/* ����24bit��ַ */
	SPI1_SendByte((addr>>16)&0xFF);
	SPI1_SendByte((addr>>8) &0xFF);
	SPI1_SendByte( addr&0xFF);


	/* ��ȡ���� */
	while(len--)
	{
		*p=SPI1_SendByte(0x10);
		
		p++;
	}
	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);	
}

void w25q128_write(uint32_t addr, uint8_t *msg)
{
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
	
	// ����дʹ������
    SPI1_SendByte(0x06);
	
	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
    GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
	
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
    GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);
	
	/* ����ָ��0x02 */
	SPI1_SendByte(0x02);
	
	/* ����24bit��ַ */
	SPI1_SendByte((addr>>16)&0xFF);
	SPI1_SendByte((addr>>8) &0xFF);
	SPI1_SendByte( addr&0xFF);
	
	/* д����*/
	for(uint8_t i = 0; i < 64; i++)
	{
		SPI1_SendByte(*(msg+i));	
	}
	
	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);	
	
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
	
	// ����дʹ������
    SPI1_SendByte(0x04);
	
	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
    GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
}

int main()
{
	uint8_t mid,did;
	uint8_t buf[64]={0};
	uint8_t msg[64]={'H','e','l','l','o'};
	int32_t i;
	
	//1.Ӳ���ĳ�ʼ��
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




