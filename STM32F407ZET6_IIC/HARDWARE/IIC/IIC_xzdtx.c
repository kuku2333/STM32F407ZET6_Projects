/**
   ******************************************************************************
   * @brief		
   ******************************************************************************
   *	
   ******************************************************************************
**/

#include "IIC_xzdtx.h"

/*	��ʼ��ʱ���ߺ�������*/
void IIC_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* ʹ��Ӳ��ʱ��:�˿�Ӳ��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	
	/* PB8 PB9��ʼ��Ϊ��©���ģʽ */
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_8 | GPIO_Pin_9;
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

void IIC_Send_Ack(uint8_t ack)
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

void IIC_Read_One_Byte(uint8_t daddr, uint8_t addr, uint8_t *data)
{				  	  	    																 
    IIC_Start();  
	
	IIC_Send_Byte(daddr);	   //����д����
	IIC_Wait_Slave_Ack();
	
	IIC_Send_Byte(addr);//���͵�ַ
	IIC_Wait_Slave_Ack();
	
	IIC_Start();  	 	   
	IIC_Send_Byte(daddr | 0X01);//�������ģʽ			   
	IIC_Wait_Slave_Ack();
	
    *data = IIC_Recv_Byte();
	IIC_Send_Ack(1);
	
    IIC_Stop();//����һ��ֹͣ����	    
}

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data)
{				   	  	    																 
    IIC_Start();  
	
	IIC_Send_Byte(daddr);	    //����д����
	IIC_Wait_Slave_Ack();
	
	IIC_Send_Byte(addr);//���͵�ַ
	IIC_Wait_Slave_Ack();	
	
	IIC_Send_Byte(data);     //�����ֽ�							   
	IIC_Wait_Slave_Ack();  
	
    i2c_stop();//����һ��ֹͣ���� 
	Delay_ms(10);	 
}

void IIC_Write_Bytes(uint8_t write_addr, uint8_t *data, uint8_t data_Length)
{		
    IIC_Start();  

	IIC_Send_Byte(write_addr);	    //����д����
	IIC_Wait_Slave_Ack();
	
	for(uint8_t i = 0; i < data_Length; i++)
	{
		IIC_Send_Byte(data[i]);
		IIC_Wait_Slave_Ack();
	}			
	
    IIC_Stop();//����һ��ֹͣ���� 
	Delay_ms(10);	 
}

void IIC_Read_Bytes(uint8_t device_addr, uint8_t write_addr, uint8_t *data, uint8_t data_length)
{		
    IIC_Start();  

	IIC_Send_Byte(device_addr);	    //����д����
	IIC_Wait_Slave_Ack();
	IIC_Send_Byte(write_addr);
	IIC_Wait_Slave_Ack();
	IIC_Send_Byte(device_addr | 0X01);//�������ģʽ			   
	IIC_Wait_Slave_Ack();
	
	for(uint8_t i = 0;i < dataLength - 1; i++)
	{
		data[i] = IIC_Recv_Byte();
		IIC_Send_Ack(0);
	}		
	data[data_length - 1] = IIC_Recv_Byte();	
	
	IIC_Send_Ack(1);
    IIC_Stop();//����һ��ֹͣ���� 
	Delay_ms(10);	 
}
