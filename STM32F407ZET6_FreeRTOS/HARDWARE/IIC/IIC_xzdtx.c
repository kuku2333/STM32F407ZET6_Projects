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
	
	/*	IICЭ�飬��ʼʱ�������Ǹߵ�ƽ*/
	SDA_W(Bit_SET);
	SCL_W(Bit_SET);
}

/*	��������Ҫ�պͷ���������Ҫ�������������ģʽ*/
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

/* ��ʼλ*/
void IIC_Start(void)
{
	/*	��ʼ�����������ӻ������������ģʽ*/
	SDA_Pin_Mode(GPIO_Mode_OUT);
	
	/*	�ֲ���������SCLΪ�ߵ�ƽʱSDA���½��أ��ߵ��ͣ�
		������ʼ������
		���ԣ��Ȱ������߶��øߣ������������õͣ���ʱʱ
		���߻��Ǹߣ������ڸ��ߴӻ���ʼ��
	*/
	SDA_W(Bit_SET);//�����߸�,Ϊ��ȷ��SDA��
	SCL_W(Bit_SET);//ʱ���߸�
	Delay_us(DELAY_TIME);//��5us��
	
	SDA_W(Bit_RESET);//�����ߵͣ�ʱ���߸ߣ���ʼλ����
	Delay_us(DELAY_TIME);
	
	/* ʱ���ߵͣ��������ݲ��ȶ������ܽ������ݵĶ�ȡ
		Ҳ���ǽ�������׼���׶�*/
	SCL_W(Bit_RESET);//
	Delay_us(DELAY_TIME);
}

void IIC_Stop(void)
{
	SDA_Pin_Mode(GPIO_Mode_OUT);//ȷ��SDA����Ϊ���ģʽ
	
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
	SDA_W(Bit_RESET);//�����ߵ�
	SCL_W(Bit_RESET);//ʱ���ߵ�
	Delay_us(DELAY_TIME);
	
	/* 2.��һ������*/
	SCL_W(Bit_SET);//ʱ���߸�
	SDA_W(Bit_SET);//�����߸�
	Delay_us(DELAY_TIME);
	
	/*ͨ�Ž����ˣ�����Ҫ�ٶ�ʱ�����õ�����ʾ��������׼���׶�*/
}

/*	
	�������մӻ���Ӧ��IIC��Ӧ������ǣ�
	�͵�ƽ����Ӧ�𣬸ߵ�ƽ����ûӦ��
*/
uint8_t IIC_Wait_Slave_Ack(void)
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
	SDA_Pin_Mode(GPIO_Mode_IN);
	
	SCL_W(Bit_RESET);//ʱ���ߵͣ����ӻ�һ��׼�����ݵ�ʱ��
	Delay_us(DELAY_TIME);
	
	SCL_W(Bit_SET);//ʱ���߸�
	Delay_us(DELAY_TIME);
	
	while(SDA_R)
	{
		error_time++;
		if(error_time > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	
//	//��ȡSDA���ŵĵ�ƽ
//	if(SDA_R)
//		ack=1;//��Ӧ��
//	else
//		ack=0;//��Ӧ��
	
	//����ʱ���ߣ���ʾ���ܽ������ݵ��շ���
	SCL_W(Bit_RESET);
	Delay_us(DELAY_TIME);	
		
	return 0;
}

/* ��������Ӧ����ӻ�*/
void IIC_Send_Ack(uint8_t ack)
{
	/*	
		������ʱ���ߣ���ʾ���ݲ��ɶ�д���ٽ������ģʽ��
		Ȼ�����źţ��������ʱ���ߡ�
	*/
	
	SCL_W(Bit_RESET);
	
	SDA_Pin_Mode(GPIO_Mode_OUT);//ȷ��SDA����Ϊ���ģʽ
	
	/* ׼�����͵��ź�*/
	if(ack)
	{
		SDA_W(Bit_SET);
	}
	else
	{
		SDA_W(Bit_RESET);
	}
	Delay_us(DELAY_TIME);
	
	/* ʱ�������ߣ��ӻ��õ��ź�*/
	SCL_W(Bit_SET);
	Delay_us(DELAY_TIME);
	
	/* ʱ�������ͣ��������ݽ����׶Σ�Ҳ���Ǳ�ʾ���ݲ��ɶ�д��*/
	SCL_W(Bit_RESET);//����Ϊ���ͷ�ʱ���ߣ�׼����һ�����ݴ���
	Delay_us(DELAY_TIME);	
}

/* 
	��������һ���ֽڵ����ݸ��ӻ����ȷ���λ���ٷ���λ��
*/
void IIC_Send_Byte(uint8_t byte)
{	
	SDA_Pin_Mode(GPIO_Mode_OUT);//ȷ��SDA����Ϊ���ģʽ
	
	SCL_W(Bit_RESET);//���ͣ���������׼���׶Ρ�
	Delay_us(DELAY_TIME);
	
	for(int8_t i = 7; i >= 0; i--)
	{
		/* ׼��Ҫ���͵�bit��*/
		if(byte & (1 << i))//�ж϶�Ӧλ��1����0
			SDA_W(Bit_SET);
		else
			SDA_W(Bit_RESET);
		Delay_us(DELAY_TIME);
		
		//����ʱ���ߣ����߲ɼ�����ǰSDA���ŵĵ�ƽ�ǿɿ��ģ����Զ�ȡ����
		SCL_W(Bit_SET);
		Delay_us(DELAY_TIME);
	
		//����ʱ���ߣ����߲ɼ�����ǰSDA���ŵĵ�ƽ�ǲ��ɿ��ģ���Ϊ��ǰҪ���SDA���ŵĵ�ƽ
		SCL_W(Bit_RESET);
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
uint8_t IIC_Read_Byte(uint8_t ack)
{
	uint8_t data = 0;
	
	SDA_Pin_Mode(GPIO_Mode_IN);
	
//	/* ȷ�����������ݵ��շ������û��Ҫ����Ϊ��ʼλ����Ѿ������ˡ�*/
//	SCL_W(Bit_RESET);
//	Delay_us(DELAY_TIME);
	
	/* ��һ���ֽڵ�����Ӧ���ǣ�������ʱ���ߣ�׼�������ݣ���
		����ʱ���߽��ж����ݣ������ѡ����Ӧ���źŻ��Ƿ�Ӧ���źš�*/
	for(int32_t i = 7; i >= 0; i--)
	{
		SCL_W(Bit_RESET);
		Delay_us(DELAY_TIME);
		
		SCL_W(Bit_SET);
		Delay_us(DELAY_TIME);

		if(SDA_R)
		{
			data |= (0x01 << i); 
		}
		Delay_us(DELAY_TIME);
	}
	
	/* 1:��Ӧ��0��Ӧ��*/
	if(ack)
	{
		IIC_Send_Ack(1);
	}
	else
	{
		IIC_Send_Ack(0);
	}
	
	return data;
}

/*	
	ͨ���豸��ַ���ڲ��Ĵ�����ַ�������ݣ����浽data�С�
*/
void IIC_Read_One_Byte(uint8_t daddr, uint8_t addr, uint8_t *data)
{				  	  	    																 
    IIC_Start();  
	
	IIC_Send_Byte(daddr);	   //����д����
	IIC_Wait_Slave_Ack();
	
	IIC_Send_Byte(addr);//���͵�ַ
	IIC_Wait_Slave_Ack();
	
	IIC_Start();  	 	   
	IIC_Send_Byte(daddr | 0x01);//�������ģʽ			   
	IIC_Wait_Slave_Ack();
	
    *data = IIC_Read_Byte(1);
	
    IIC_Stop();//����һ��ֹͣ����	    
}

/*
	ͨ���豸��ַ���ڲ��Ĵ�����ַ����dataд����Ӧ��ַ�С�
*/
void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data)
{				   	  	    																 
    IIC_Start();  
	
	IIC_Send_Byte(daddr);	    //����д����
	IIC_Wait_Slave_Ack();
	
	IIC_Send_Byte(addr);//���͵�ַ
	IIC_Wait_Slave_Ack();	
	
	IIC_Send_Byte(data);     //�����ֽ�							   
	IIC_Wait_Slave_Ack();  
	
    IIC_Stop();//����һ��ֹͣ���� 
	Delay_ms(10);	 
}

/*
	������д����ֽڵ����ݡ�
*/
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

/*
	�����ض�����ֽڵ����ݡ�
*/
void IIC_Read_Bytes(uint8_t device_addr, uint8_t write_addr, uint8_t *data, uint8_t data_length)
{		
    IIC_Start();  

	IIC_Send_Byte(device_addr);	    //����д����
	IIC_Wait_Slave_Ack();
	IIC_Send_Byte(write_addr);
	IIC_Wait_Slave_Ack();
	IIC_Send_Byte(device_addr | 0X01);//�������ģʽ			   
	IIC_Wait_Slave_Ack();
	
	for(uint8_t i = 0;i < data_length - 1; i++)
	{
		data[i] = IIC_Read_Byte(0);
	}		
	data[data_length - 1] = IIC_Read_Byte(1);	

    IIC_Stop();//����һ��ֹͣ���� 
	Delay_ms(10);	 
}
