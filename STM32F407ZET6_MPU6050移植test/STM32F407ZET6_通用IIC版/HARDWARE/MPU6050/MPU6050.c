#include "MPU6050.h"

u8 MPU_Init(I2C_Port_t *port)
{
	u8 res;
	IIC_Init(port);//��ʼ��IIC����
	MPU_Write_Byte(port, MPU_PWR_MGMT1_REG,0X80);	//��λMPU6050
    Delay_ms(100);
	MPU_Write_Byte(port, MPU_PWR_MGMT1_REG,0X00);	//����MPU6050 
	MPU_Set_Gyro_Fsr(port, 3);					//�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(port, 0);					//���ٶȴ�����,��2g
	MPU_Set_Rate(port, 50);						//���ò�����50Hz
	MPU_Write_Byte(port, MPU_INT_EN_REG,0X00);	//�ر������ж�
	MPU_Write_Byte(port, MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
	MPU_Write_Byte(port, MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
	MPU_Write_Byte(port, MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
	res=MPU_Read_Byte(port, MPU_DEVICE_ID_REG);
	printf("RES=%X\r\n",res);
	if(res==MPU_ADDR)//����ID��ȷ
	{
		MPU_Write_Byte(port, MPU_PWR_MGMT1_REG,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
		MPU_Write_Byte(port, MPU_PWR_MGMT2_REG,0X00);	//���ٶ��������Ƕ�����
		MPU_Set_Rate(port, 50);						//���ò�����Ϊ50Hz
 	}else return 1;
	return 0;
}
//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Gyro_Fsr(I2C_Port_t *port, u8 fsr)
{
	return MPU_Write_Byte(port, MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
}
//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Accel_Fsr(I2C_Port_t *port, u8 fsr)
{
	return MPU_Write_Byte(port, MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}
//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_LPF(I2C_Port_t *port, u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(port, MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}
//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Rate(I2C_Port_t *port, u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(port, MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(port, rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
short MPU_Get_Temperature(I2C_Port_t *port)
{
//	printf("SDA pin is:%X\r\n", port->SDA_Pin);
//	printf("SCL pin is:%X\r\n", port->SCL_Pin);
//	printf("pin7 pin is:%X\r\n", GPIO_Pin_7);
//	printf("pin9 pin is:%X\r\n", GPIO_Pin_9);
	
    u8 buf[2]; 
    short raw;
	float temp;
	uint8_t relere;
	relere = MPU_Read_Len(port, MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf); 
//	if(relere == 0)
//	{
//		printf("relere success\r\n");
//	}
	printf("Buffer content: %X %X\n", buf[0], buf[1]);
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
	printf("raw is:%d\r\n", raw);
    return temp*100;;
}
//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Gyroscope(I2C_Port_t *port, short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(port, MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Accelerometer(I2C_Port_t *port, short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(port, MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
//IIC����д
//addr:������ַ 
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//    ����,�������
u8 MPU_Write_Len(I2C_Port_t *port, u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(port); 
	IIC_Send_Byte(port, (addr<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Slave_Ack(port))	//�ȴ�Ӧ��
	{
		IIC_Stop(port);		 
		return 1;		
	}
    IIC_Send_Byte(port, reg);	//д�Ĵ�����ַ
    IIC_Wait_Slave_Ack(port);		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(port, buf[i]);	//��������
		if(IIC_Wait_Slave_Ack(port))		//�ȴ�ACK
		{
			IIC_Stop(port);	 
			return 1;		 
		}		
	}    
    IIC_Stop(port);	 
	return 0;	
} 
//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
u8 MPU_Read_Len(I2C_Port_t *port, u8 addr,u8 reg,u8 len,u8 *buf)
{ 
//	printf("SDA pin is:%X\r\n", port->SDA_Pin);
//	printf("SCL pin is:%X\r\n", port->SCL_Pin);
//	printf("pin7 pin is:%X\r\n", GPIO_Pin_7);
//	printf("pin9 pin is:%X\r\n", GPIO_Pin_9);
	
 	IIC_Start(port); 
	IIC_Send_Byte(port, (addr<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Slave_Ack(port))	//�ȴ�Ӧ��
	{
		IIC_Stop(port);		 
		return 1;		
	}
    IIC_Send_Byte(port, reg);	//д�Ĵ�����ַ
    IIC_Wait_Slave_Ack(port);		//�ȴ�Ӧ��
    IIC_Start(port);
	IIC_Send_Byte(port, (addr<<1)|1);//����������ַ+������	
    IIC_Wait_Slave_Ack(port);		//�ȴ�Ӧ�� 
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(port, 1);//������,����nACK 
		else *buf=IIC_Read_Byte(port, 0);		//������,����ACK  
		len--;
		buf++; 
	}    
    IIC_Stop(port);	//����һ��ֹͣ���� 
	return 0;	
}
//IICдһ���ֽ� 
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
u8 MPU_Write_Byte(I2C_Port_t *port, u8 reg,u8 data) 				 
{ 
    IIC_Start(port); 
	IIC_Send_Byte(port, (MPU_ADDR<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Slave_Ack(port))	//�ȴ�Ӧ��
	{
		IIC_Stop(port);		 
		return 1;		
	}
    IIC_Send_Byte(port, reg);	//д�Ĵ�����ַ
    IIC_Wait_Slave_Ack(port);		//�ȴ�Ӧ�� 
	IIC_Send_Byte(port, data);//��������
	if(IIC_Wait_Slave_Ack(port))	//�ȴ�ACK
	{
		IIC_Stop(port);	 
		return 1;		 
	}		 
    IIC_Stop(port);	 
	return 0;
}
//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
u8 MPU_Read_Byte(I2C_Port_t *port, u8 reg)
{
	u8 res;
    IIC_Start(port); 
	IIC_Send_Byte(port, (MPU_ADDR<<1)|0);//����������ַ+д����	
	IIC_Wait_Slave_Ack(port);		//�ȴ�Ӧ�� 
    IIC_Send_Byte(port, reg);	//д�Ĵ�����ַ
    IIC_Wait_Slave_Ack(port);		//�ȴ�Ӧ��
    IIC_Start(port);
	IIC_Send_Byte(port, (MPU_ADDR<<1)|1);//����������ַ+������	
    IIC_Wait_Slave_Ack(port);		//�ȴ�Ӧ�� 
	res=IIC_Read_Byte(port, 1);//��ȡ����,����nACK 
    IIC_Stop(port);			//����һ��ֹͣ���� 
	return res;		
}
