/**
   ******************************************************************************
   * @brief		������IO����ģ��IIC
   ******************************************************************************
   *	1.PB8->ʱ���ߣ�PB9->������
   *	2.�����AT24C02���ֲ�������α�д,�����ǿ�
   *	��Ӧ��ʱ��ͼ
   *	3.IICͨ��ʱ����Ҫ�����źţ��ɷֱ����װ�ɺ���
   *	�У���ʼ������ֹͣ����������Ӧ�𡢴ӻ�Ӧ��
   ******************************************************************************
**/

#include "AT24C02.h"

/*	��ʼ��ʱ���ߺ�������*/
void AT24C02_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* ʹ��Ӳ��ʱ��:�˿�Ӳ��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	
	/* PB8 PB9��ʼ��Ϊ��©���ģʽ */
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	/*	�ȶ����ͣ�Ϊ�����Ĳ�����һ����ʼ״̬*/
	SDA_W(Bit_SET);
	SCL_W(Bit_SET);
}

/*	��������Ҫ����д��������Ҫ�������������ģʽ*/
void SDA_Pin_Mode(GPIOMode_TypeDef pin_mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode 	= pin_mode;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void IIC_Start(void)
{
	/*	����д��ǰ��׼�������������ӻ������������*/
	SDA_Pin_Mode(GPIO_Mode_OUT);
	
	/*	�ֲ���������SCLΪ�ߵ�ƽʱSDA���½��أ��ߵ��ͣ�
		������ʼ������
		���ԣ��Ȱ������߶��øߣ���ʾ����״̬����������
		���õͣ���ʱʱ���߻��Ǹߣ������ڸ��ߴӻ���ʼ�ˣ�
		�������һ��ʱ�����õͲ�����
	*/
	SDA_W(Bit_SET);//�����߸�
	SCL_W(Bit_SET);//ʱ���߸�
	Delay_us(5);
	
	SDA_W(Bit_RESET);//�����ߵ�
	Delay_us(5);
	
	SCL_W(Bit_RESET);//ʱ���ߵ�
	Delay_us(5);
}

void IIC_Stop(void)
{
	SDA_Pin_Mode(GPIO_Mode_OUT);//ȷ��SDA����Ϊ���ģʽ
	
	/*
		�ֲ���������SCLΪ�ߵ�ƽʱSDA����ɽ�أ��͵��ߣ�
		������ʼ������
	*/
	SDA_W(Bit_RESET);//�����ߵ�
	SCL_W(Bit_SET);//ʱ���߸�
	Delay_us(5);
	
	SDA_W(Bit_SET);//�����߸�
	Delay_us(5);
	
	//ͨ�Ž����ˣ�����Ҫ�ٶ�ʱ�����õ�
}

uint8_t IIC_Wait_Slave_Ack(void)
{
	uint8_t ack;
	SDA_Pin_Mode(GPIO_Mode_IN);//ȷ��SDA����Ϊ����ģʽ
	
	/*
		�ֲ������������ϵĽ�����ÿ�յ�һ���ֽھͲ���
		һ��Ӧ���������������һ����Ӧ�Ķ����ʱ��
		����
		1.��˼��˵���ڽ���һ���ֽں�ʱ����Ҫ����һ�����壬
		ֱ������ʱ���ߣ�Ȼ��������ߣ��жϴӻ��Ƿ�Ӧ��
		�͵�ƽ������Ӧ�𣬸ߵ�ƽ����û��Ӧ��
		2.����ʱ���ߣ����߲ɼ�����ǰSDA���ŵĵ�ƽ�ǿɿ��ģ�
		���Զ�ȡ����
	*/
	SCL_W(Bit_SET);
	Delay_us(5);
	
	//��ȡSDA���ŵĵ�ƽ
	if(SDA_R)
		ack=1;//��Ӧ��
	else
		ack=0;//��Ӧ��
	
	//����ʱ����
	SCL_W(Bit_RESET);
	Delay_us(5);	
		
	return ack;
}

void IIC_SendAck(uint8_t ack)
{
	SDA_Pin_Mode(GPIO_Mode_OUT);//ȷ��SDA����Ϊ���ģʽ

	/*	
		�ڷ���Ӧ��ǰ��ʱ���߸ߵͶ�������������Ӧ��
		������ʱ��ͼ���øߺ���㣬�����߱������õͣ�
		Ȼ����Ӧ��
	*/
	SCL_W(Bit_SET);/*����Ϊ���ڷ���Ӧ���ź�֮ǰ��ͣʱ��
					���ݽ���ֻ��������ʱ����Ϊ�͵�ʱ��*/
	SDA_W(Bit_RESET);//����Ϊ��ȷ���ڷ���Ӧ���ź�֮ǰ���������ǵ͵�ƽ
	Delay_us(5);
	
	if(ack)
	{
		SDA_W(Bit_SET);
	}
	else
	{
		SDA_W(Bit_RESET);
	}
	
	Delay_us(5);
	
	SCL_W(Bit_SET);//����Ϊ�˽���Ӧ���źŵķ��͡�
	Delay_us(5);
	SCL_W(Bit_RESET);//����Ϊ���ͷ�ʱ���ߣ�׼����һ�����ݴ���
	Delay_us(5);	
}

void IIC_Send_Byte(uint8_t byte)
{	
	SDA_Pin_Mode(GPIO_Mode_OUT);//ȷ��SDA����Ϊ���ģʽ
	
	SCL_W(Bit_RESET);//���ͣ��������ݽ���
	Delay_us(5);
	
	for(int32_t i = 7; i >= 0; i--)
	{
		//����ȥ����SDA���ŵĵ�ƽ
		if(byte & (1 << i))
			SDA_W(Bit_SET);
		else
			SDA_W(Bit_RESET);
	
		Delay_us(5);
		
		//����ʱ���ߣ����߲ɼ�����ǰSDA���ŵĵ�ƽ�ǿɿ��ģ����Զ�ȡ����
		SCL_W(Bit_SET);
		Delay_us(5);
	
		//����ʱ���ߣ����߲ɼ�����ǰSDA���ŵĵ�ƽ�ǲ��ɿ��ģ���Ϊ��ǰҪ���SDA���ŵĵ�ƽ
		SCL_W(Bit_RESET);
		Delay_us(5);	
	}
}

uint8_t IIC_Recv_Byte(void)
{
	uint8_t data = 0;
	
	SDA_Pin_Mode(GPIO_Mode_IN);
	
	SCL_W(Bit_RESET);
	Delay_us(5);
	
	for(int32_t i = 7; i >= 0; i--)
	{
		SCL_W(Bit_SET);
		Delay_us(5);

		if(SDA_R)
		{
			data |= (0x01 << i); 
		}
		Delay_us(5);
			
		SCL_W(Bit_RESET);
		Delay_us(5);
	}
	
	IIC_Send_Ack(0);
	
	return data;
}

int32_t AT24C02_Write(uint8_t word_addr, uint8_t *buf, uint8_t len)
{
	uint8_t ack;
	uint8_t *p = buf;
	
	//���Ϳ�ʼ�ź�
	IIC_Start();
	
	//��ʼ�豸Ѱַ
	IIC_Send_Byte(0xA0);
	
	//�ȴ��ӻ���Ӧ��
	ack = IIC_Wait_Slave_Ack();
	if(ack)
	{
		printf("device address fail\r\n");
		
		return -1;
	}
	
	//����word address�����ߴӻ����ĸ��洢��ַд������
	IIC_Send_Byte(word_addr);
	
	//�ȴ��ӻ���Ӧ��
	ack = IIC_Wait_Slave_Ack();
	if(ack)
	{
		printf("word address fail\r\n");
		
		return -2;
	}
	
	//ѭ������Ҫд����ֽ�
	while(len--)
	{
		//����һ���ֽ�
		IIC_Send_Byte(*p);
		
		//�ȴ�Ӧ��
		ack = IIC_Wait_Slave_Ack();
		if(ack)
		{
			printf("write data fail\r\n");
			
			return -3;
		}
		
		p++;
	}
	
	//����ֹͣ�ź�
	IIC_Stop();
	
	printf("write success\r\n");
	
	return 0;
}

void AT24C02_Read(uint8_t word_addr, uint8_t *buf, uint8_t len)
{
	uint8_t ack;
	
	IIC_Start();
	
	IIC_Send_Byte(0xA0);
	
	ack = IIC_Wait_Slave_Ack();
	if(ack)
	{
		printf("device address with write fail\r\n");
	}
	
	IIC_Send_Byte(word_addr);
	
	ack = IIC_Wait_Slave_Ack();
	if(ack)
	{
		printf("word address fail\r\n");
	}
	
	IIC_Start();
	
	IIC_Send_Byte(0xA1);
	
	ack = IIC_Wait_Slave_Ack();
	if(ack)
	{
		printf("device address with read fail\r\n");
	}
	
	for(int32_t i = 0; i < len - 1; i++)
	{
		buf[i] = IIC_Recv_Byte();
		printf("buf[%d]:%d\r\n", i, buf[i]);
	}
	
	buf[7] = IIC_Recv_Byte();
	IIC_Send_Ack(1);
	
	IIC_Stop();
}

/*
	���Դ��룺
	uint8_t buf[8]={1,2,3,4,5,6,7,8};
	uint8_t get_buf[8]={0};
	
	//1.Ӳ���ĳ�ʼ��
	Uasrt1_Init();
	
	//��ʼ��at24c02
	AT24C02_Init();
	
	//��at24c02д������
	AT24C02_Write(0,buf,sizeof buf);
	
	Delay_ms(10);
	
	AT24C02_Read(0, get_buf, sizeof get_buf);
	
	for(uint8_t i = 0; i < 8; i++)
	{
		printf("The value of data_%d is: %d\r\n", i, get_buf[i]);
	}
*/
