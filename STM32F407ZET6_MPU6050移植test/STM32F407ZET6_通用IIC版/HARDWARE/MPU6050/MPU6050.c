#include "MPU6050.h"

u8 MPU_Init(I2C_Port_t *port)
{
	u8 res;
	IIC_Init(port);//初始化IIC总线
	MPU_Write_Byte(port, MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
    Delay_ms(100);
	MPU_Write_Byte(port, MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	MPU_Set_Gyro_Fsr(port, 3);					//陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(port, 0);					//加速度传感器,±2g
	MPU_Set_Rate(port, 50);						//设置采样率50Hz
	MPU_Write_Byte(port, MPU_INT_EN_REG,0X00);	//关闭所有中断
	MPU_Write_Byte(port, MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	MPU_Write_Byte(port, MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	MPU_Write_Byte(port, MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	res=MPU_Read_Byte(port, MPU_DEVICE_ID_REG);
	printf("RES=%X\r\n",res);
	if(res==MPU_ADDR)//器件ID正确
	{
		MPU_Write_Byte(port, MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
		MPU_Write_Byte(port, MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
		MPU_Set_Rate(port, 50);						//设置采样率为50Hz
 	}else return 1;
	return 0;
}
//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Gyro_Fsr(I2C_Port_t *port, u8 fsr)
{
	return MPU_Write_Byte(port, MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Accel_Fsr(I2C_Port_t *port, u8 fsr)
{
	return MPU_Write_Byte(port, MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_LPF(I2C_Port_t *port, u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(port, MPU_CFG_REG,data);//设置数字低通滤波器  
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Rate(I2C_Port_t *port, u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(port, MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(port, rate/2);	//自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值(扩大了100倍)
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
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
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
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
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
//IIC连续写
//addr:器件地址 
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Len(I2C_Port_t *port, u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(port); 
	IIC_Send_Byte(port, (addr<<1)|0);//发送器件地址+写命令	
	if(IIC_Wait_Slave_Ack(port))	//等待应答
	{
		IIC_Stop(port);		 
		return 1;		
	}
    IIC_Send_Byte(port, reg);	//写寄存器地址
    IIC_Wait_Slave_Ack(port);		//等待应答
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(port, buf[i]);	//发送数据
		if(IIC_Wait_Slave_Ack(port))		//等待ACK
		{
			IIC_Stop(port);	 
			return 1;		 
		}		
	}    
    IIC_Stop(port);	 
	return 0;	
} 
//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Read_Len(I2C_Port_t *port, u8 addr,u8 reg,u8 len,u8 *buf)
{ 
//	printf("SDA pin is:%X\r\n", port->SDA_Pin);
//	printf("SCL pin is:%X\r\n", port->SCL_Pin);
//	printf("pin7 pin is:%X\r\n", GPIO_Pin_7);
//	printf("pin9 pin is:%X\r\n", GPIO_Pin_9);
	
 	IIC_Start(port); 
	IIC_Send_Byte(port, (addr<<1)|0);//发送器件地址+写命令	
	if(IIC_Wait_Slave_Ack(port))	//等待应答
	{
		IIC_Stop(port);		 
		return 1;		
	}
    IIC_Send_Byte(port, reg);	//写寄存器地址
    IIC_Wait_Slave_Ack(port);		//等待应答
    IIC_Start(port);
	IIC_Send_Byte(port, (addr<<1)|1);//发送器件地址+读命令	
    IIC_Wait_Slave_Ack(port);		//等待应答 
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(port, 1);//读数据,发送nACK 
		else *buf=IIC_Read_Byte(port, 0);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    IIC_Stop(port);	//产生一个停止条件 
	return 0;	
}
//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Byte(I2C_Port_t *port, u8 reg,u8 data) 				 
{ 
    IIC_Start(port); 
	IIC_Send_Byte(port, (MPU_ADDR<<1)|0);//发送器件地址+写命令	
	if(IIC_Wait_Slave_Ack(port))	//等待应答
	{
		IIC_Stop(port);		 
		return 1;		
	}
    IIC_Send_Byte(port, reg);	//写寄存器地址
    IIC_Wait_Slave_Ack(port);		//等待应答 
	IIC_Send_Byte(port, data);//发送数据
	if(IIC_Wait_Slave_Ack(port))	//等待ACK
	{
		IIC_Stop(port);	 
		return 1;		 
	}		 
    IIC_Stop(port);	 
	return 0;
}
//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
u8 MPU_Read_Byte(I2C_Port_t *port, u8 reg)
{
	u8 res;
    IIC_Start(port); 
	IIC_Send_Byte(port, (MPU_ADDR<<1)|0);//发送器件地址+写命令	
	IIC_Wait_Slave_Ack(port);		//等待应答 
    IIC_Send_Byte(port, reg);	//写寄存器地址
    IIC_Wait_Slave_Ack(port);		//等待应答
    IIC_Start(port);
	IIC_Send_Byte(port, (MPU_ADDR<<1)|1);//发送器件地址+读命令	
    IIC_Wait_Slave_Ack(port);		//等待应答 
	res=IIC_Read_Byte(port, 1);//读取数据,发送nACK 
    IIC_Stop(port);			//产生一个停止条件 
	return res;		
}
