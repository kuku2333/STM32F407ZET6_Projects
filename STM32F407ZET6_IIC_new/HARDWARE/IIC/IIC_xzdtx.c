/**
   ******************************************************************************
   * @brief		
   ******************************************************************************
   *	
   ******************************************************************************
**/

#include "IIC_xzdtx.h"

/*	��ʼ��ʱ���ߺ�������*/
void IIC_Init(I2C_Port_t *port)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* ʹ��Ӳ��ʱ��:�˿�Ӳ��ʱ�� */
	RCC_AHB1PeriphClockCmd(port->SDA_RCC_AHBPeriph, ENABLE);
	RCC_AHB1PeriphClockCmd(port->SCL_RCC_AHBPeriph, ENABLE);	
	
	/* PB8 PB9��ʼ��Ϊ��©���ģʽ */
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= 	port->SDA_Pin;
	GPIO_Init(port->SDA_GPIOx, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= 	port->SCL_Pin;
	GPIO_Init(port->SCL_GPIOx, &GPIO_InitStructure);
	
	/*	IICЭ�飬��ʼʱ�������Ǹߵ�ƽ*/
//	SDA_W(Bit_SET);
//	SCL_W(Bit_SET);
	GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_SET);
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_SET);
}

/*	��������Ҫ�պͷ���������Ҫ�������������ģʽ*/
void SDA_Pin_Mode(I2C_Port_t *port, GPIOMode_TypeDef pin_mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode 	= pin_mode;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= port->SDA_Pin;
	GPIO_Init(port->SDA_GPIOx, &GPIO_InitStructure);	
}

/* ��ʼλ*/
void IIC_Start(I2C_Port_t *port)
{
	/*	��ʼ�����������ӻ������������ģʽ*/
	SDA_Pin_Mode(port, GPIO_Mode_OUT);
	
	/*	�ֲ���������SCLΪ�ߵ�ƽʱSDA���½��أ��ߵ��ͣ�
		������ʼ������
		���ԣ��Ȱ������߶��øߣ������������õͣ���ʱʱ
		���߻��Ǹߣ������ڸ��ߴӻ���ʼ��
	*/
//	IIC_Digital_WRITE(Bit_SET);//�����߸�,Ϊ��ȷ��SDA��
//	SDA_W(Bit_SET);//�����߸�,Ϊ��ȷ��SDA��
	GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_SET);
//	SCL_W(Bit_SET);//ʱ���߸�
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_SET);
	Delay_us(DELAY_TIME);//��5us��
	
//	SDA_W(Bit_RESET);//�����ߵͣ�ʱ���߸ߣ���ʼλ����
	GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_RESET);
	Delay_us(DELAY_TIME);
	
	/* ʱ���ߵͣ��������ݲ��ȶ������ܽ������ݵĶ�ȡ
		Ҳ���ǽ�������׼���׶�*/
//	SCL_W(Bit_RESET);//
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
	Delay_us(DELAY_TIME);
}

void IIC_Stop(I2C_Port_t *port)
{
	SDA_Pin_Mode(port, GPIO_Mode_OUT);//ȷ��SDA����Ϊ���ģʽ
	
	/*
		�ֲ���������SCLΪ�ߵ�ƽʱSDA�������أ��͵��ߣ�
		������ʼ������
	*/
	
	/* 1.�ȶ�����
	
		��ֹͣλ��ʱ��������ʱ����Ҳ�������ߣ�Ȼ������һ
		����ֻ���������߾����ˣ����������п��ܲ��������
		���壬��Ϊ��������ʱ����Ϊ��ʱ�����շ��ģ�������
		���ͺá�����ʼλ������ǰ����Ҫ������Щ����Ϊ��û��
		�������ݵ��շ���
	*/
//	SDA_W(Bit_RESET);//�����ߵ�
//	SCL_W(Bit_RESET);//ʱ���ߵ�
	GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_RESET);
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
	Delay_us(DELAY_TIME);
	
	/* 2.��һ������*/
//	SCL_W(Bit_SET);//ʱ���߸�
//	SDA_W(Bit_SET);//�����߸�
	GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_SET);
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_SET);
	Delay_us(DELAY_TIME);
	
	/*ͨ�Ž����ˣ�����Ҫ�ٶ�ʱ�����õ�����ʾ��������׼���׶�*/
}

/*	
	�������մӻ���Ӧ��IIC��Ӧ������ǣ�
	�͵�ƽ����Ӧ�𣬸ߵ�ƽ����ûӦ��
*/
uint8_t IIC_Wait_Slave_Ack(I2C_Port_t *port)
{
	/*
		�ֲ������������ϵĽ�����ÿ�յ�һ���ֽھͲ���
		һ��Ӧ���������������һ����Ӧ�Ķ����ʱ��
		���塣
	
		��˼��˵���ڽ���һ���ֽں�ʱ����Ҫ����һ�����壬
		��������ʱ���ߣ�Ȼ��������ߣ��жϴӻ��Ƿ�Ӧ��
		�͵�ƽ������Ӧ�𣬸ߵ�ƽ����û��Ӧ��
	*/
	
	uint8_t error_time = 0;
	
	/* ȷ��SDA����Ϊ����ģʽ���Խ��մӻ���Ӧ���źš�*/
//	SDA_Pin_Mode(GPIO_Mode_IN);
	SDA_Pin_Mode(port, GPIO_Mode_IN);
	
//	SCL_W(Bit_RESET);//ʱ���ߵͣ����ӻ�һ��׼�����ݵ�ʱ��
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
	Delay_us(DELAY_TIME);
	
//	SCL_W(Bit_SET);//ʱ���߸�
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_SET);
	Delay_us(DELAY_TIME);
	
	while(GPIO_ReadInputDataBit(port->SDA_GPIOx, port->SDA_Pin))
	{
		error_time++;
		if(error_time > 250)
		{
			IIC_Stop(port);
			return 1;
		}
	}
	
//	//��ȡSDA���ŵĵ�ƽ
//	if(SDA_R)
//		ack=1;//��Ӧ��
//	else
//		ack=0;//��Ӧ��
	
	//����ʱ���ߣ���ʾ���ܽ������ݵ��շ���
//	SCL_W(Bit_RESET);
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
	Delay_us(DELAY_TIME);	
		
	return 0;
}

/* ��������Ӧ����ӻ�*/
void IIC_Send_Ack(I2C_Port_t *port, uint8_t ack)
{
	/*	
		������ʱ���ߣ���ʾ���ݲ��ɶ�д���ٽ������ģʽ��
		Ȼ�����źţ��������ʱ���ߡ�
	*/
	
//	SCL_W(Bit_RESET);
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
	
//	SDA_Pin_Mode(GPIO_Mode_OUT);//ȷ��SDA����Ϊ���ģʽ
	SDA_Pin_Mode(port, GPIO_Mode_OUT);
	
	/* ׼�����͵��ź�*/
	if(ack)
	{
//		SDA_W(Bit_SET);
		GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_SET);
	}
	else
	{
//		SDA_W(Bit_RESET);
		GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_RESET);
	}
	Delay_us(DELAY_TIME);
	
	/* ʱ�������ߣ��ӻ��õ��ź�*/
//	SCL_W(Bit_SET);
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_SET);
	Delay_us(DELAY_TIME);
	
	/* ʱ�������ͣ��������ݽ����׶Σ�Ҳ���Ǳ�ʾ���ݲ��ɶ�д��*/
//	SCL_W(Bit_RESET);//����Ϊ���ͷ�ʱ���ߣ�׼����һ�����ݴ���
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
	Delay_us(DELAY_TIME);	
}

/* 
	��������һ���ֽڵ����ݸ��ӻ����ȷ���λ���ٷ���λ��
*/
void IIC_Send_Byte(I2C_Port_t *port, uint8_t byte)
{	
//	SDA_Pin_Mode(GPIO_Mode_OUT);//ȷ��SDA����Ϊ���ģʽ
	SDA_Pin_Mode(port, GPIO_Mode_OUT);
	
//	SCL_W(Bit_RESET);//���ͣ���������׼���׶Ρ�
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
	Delay_us(DELAY_TIME);
	
	for(int8_t i = 7; i >= 0; i--)
	{
		/* ׼��Ҫ���͵�bit��*/
		if(byte & (1 << i))//�ж϶�Ӧλ��1����0
//			SDA_W(Bit_SET);
			GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_SET);
		else
//			SDA_W(Bit_RESET);
			GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_RESET);
		Delay_us(DELAY_TIME);
		
		//����ʱ���ߣ����߲ɼ�����ǰSDA���ŵĵ�ƽ�ǿɿ��ģ����Զ�ȡ����
//		SCL_W(Bit_SET);
		GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_SET);
		Delay_us(DELAY_TIME);
	
		//����ʱ���ߣ����߲ɼ�����ǰSDA���ŵĵ�ƽ�ǲ��ɿ��ģ���Ϊ��ǰҪ���SDA���ŵĵ�ƽ
//		SCL_W(Bit_RESET);
		GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
		Delay_us(DELAY_TIME);	
	}
}

/* 	
	������ȡ�ӻ�����һ���ֽڵ����ݣ�IICÿ�շ�һ��
	�ֽڵ����ݣ���Ҫһ��Ӧ���źţ���һ���������ݷ��ͺ�
	Ҫ��һ����Ӧ��Ȼ����ֹͣλ��ֹͣλ���ڱ�������ʹ�á�
	�����ͺ����У�ÿ����һ���ֽ�Ҳ���յ��ӻ���Ӧ���źţ�����
	�ڷ��ͺ�Ҫ�жϴӻ���û��Ӧ��
*/
uint8_t IIC_Read_Byte(I2C_Port_t *port, uint8_t ack)
{
	uint8_t data = 0;
	
//	SDA_Pin_Mode(GPIO_Mode_IN);
	SDA_Pin_Mode(port, GPIO_Mode_IN);
	
//	/* ȷ�����������ݵ��շ������û��Ҫ����Ϊ��ʼλ����Ѿ������ˡ�*/
//	SCL_W(Bit_RESET);
//	Delay_us(DELAY_TIME);
	
	/* ��һ���ֽڵ�����Ӧ���ǣ�������ʱ���ߣ�׼�������ݣ���
		����ʱ���߽��ж����ݣ������ѡ����Ӧ���źŻ��Ƿ�Ӧ���źš�*/
	for(int32_t i = 7; i >= 0; i--)
	{
//		SCL_W(Bit_RESET);
		GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
		Delay_us(DELAY_TIME);
		
//		SCL_W(Bit_SET);
		GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_SET);
		Delay_us(DELAY_TIME);

//		if(SDA_R)
//		{
//			data |= (0x01 << i); 
//		}
		if(GPIO_ReadInputDataBit(port->SDA_GPIOx, port->SDA_Pin))
		{
			data |= (0x01 << i); 
		}
		Delay_us(DELAY_TIME);
	}
	
	/* 1:��Ӧ��0��Ӧ��*/
	if(ack)
	{
		IIC_Send_Ack(port, 1);
	}
	else
	{
		IIC_Send_Ack(port, 0);
	}
	
	return data;
}

/*	
	ͨ���豸��ַ���ڲ��Ĵ�����ַ�������ݣ����浽data�С�
*/
void IIC_Read_One_Byte(I2C_Port_t *port, uint8_t daddr, uint8_t addr, uint8_t *data)
{				  	  	    																 
    IIC_Start(port);  
	
	IIC_Send_Byte(port, daddr);	   //����д����
	IIC_Wait_Slave_Ack(port);
	
	IIC_Send_Byte(port, addr);//���͵�ַ
	IIC_Wait_Slave_Ack(port);
	
	IIC_Start(port);  	 	   
	IIC_Send_Byte(port, daddr | 0x01);//�������ģʽ			   
	IIC_Wait_Slave_Ack(port);
	
    *data = IIC_Read_Byte(port, 1);
	
    IIC_Stop(port);//����һ��ֹͣ����	    
}

/*
	ͨ���豸��ַ���ڲ��Ĵ�����ַ����dataд����Ӧ��ַ�С�
*/
void IIC_Write_One_Byte(I2C_Port_t *port, uint8_t daddr,uint8_t addr,uint8_t data)
{				   	  	    																 
    IIC_Start(port);  
	
	IIC_Send_Byte(port, daddr);	    //����д����
	IIC_Wait_Slave_Ack(port);
	
	IIC_Send_Byte(port, addr);//���͵�ַ
	IIC_Wait_Slave_Ack(port);	
	
	IIC_Send_Byte(port, data);     //�����ֽ�							   
	IIC_Wait_Slave_Ack(port);  
	
    IIC_Stop(port);//����һ��ֹͣ���� 
	Delay_ms(10);	 
}

/*
	������д����ֽڵ����ݡ�
*/
void IIC_Write_Bytes(I2C_Port_t *port, uint8_t write_addr, uint8_t *data, uint8_t data_Length)
{		
    IIC_Start(port);  

	IIC_Send_Byte(port, write_addr);	    //����д����
	IIC_Wait_Slave_Ack(port);
	
	for(uint8_t i = 0; i < data_Length; i++)
	{
		IIC_Send_Byte(port, data[i]);
		IIC_Wait_Slave_Ack(port);
	}			
	
    IIC_Stop(port);//����һ��ֹͣ���� 
	Delay_ms(10);	 
}

/*
	�����ض�����ֽڵ����ݡ�
*/
void IIC_Read_Bytes(I2C_Port_t *port, uint8_t device_addr, uint8_t write_addr, uint8_t *data, uint8_t data_length)
{		
    IIC_Start(port);  

	IIC_Send_Byte(port, device_addr);	    //����д����
	IIC_Wait_Slave_Ack(port);
	IIC_Send_Byte(port, write_addr);
	IIC_Wait_Slave_Ack(port);
	IIC_Send_Byte(port, device_addr | 0X01);//�������ģʽ			   
	IIC_Wait_Slave_Ack(port);
	
	for(uint8_t i = 0;i < data_length - 1; i++)
	{
		data[i] = IIC_Read_Byte(port, 0);
	}		
	data[data_length - 1] = IIC_Read_Byte(port, 1);	

    IIC_Stop(port);//����һ��ֹͣ���� 
	Delay_ms(10);	 
}
