/**
   ******************************************************************************
   * @file    STM32F407ZET6_通用ICC版\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-24
   * @brief   
   ******************************************************************************
   * MPU6050
   ******************************************************************************
**/

#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include "sys.h"
#include "Uasrt_Send_String.h"
#include "MPU6050.h"

//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void Uart0NimingReport(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X88;	//帧头
	send_buf[1]=fun;	//功能字
	send_buf[2]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//复制数据
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和	
	//for(i=0;i<len+4;i++)printf("send_buf%d is: %d\r\n", i, send_buf[i]);//发送数据到串口1 
}

//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
void MPU6050SendData(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	Uart0NimingReport(0XA1,tbuf,12);//自定义帧,0XA1
	
}	
//通过串口0上报结算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void Uart0ReportImu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//清0
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	Uart0NimingReport(0XAF,tbuf,28);//飞控显示帧,0XAF
}

I2C_Port_t mpu6050_port = 	{
								GPIOC, GPIOC,
								GPIO_Pin_7, GPIO_Pin_9, 
								RCC_AHB1Periph_GPIOC, RCC_AHB1Periph_GPIOC
							};


int main(void)
{
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uint8_t res;
	
	float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	short temp;					//温度		
	
	/* Configure the Priority Group to 2 bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//串口1初始化，且波特率为115200bps
	Uasrt1_Init();

	u8 inre;
	inre = MPU_Init(&mpu6050_port);
	if(inre == 0)
	{
		printf("mpu init success\r\n");
	}

	while(1)
	{
		if(res==0)
		{ 
			u8 buf[2];
			MPU_Read_Len(&mpu6050_port, MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
			printf("Buffer content: %X %X\n", buf[0], buf[1]);
//			temp=MPU_Get_Temperature(&mpu6050_port);	//得到温度值
//			printf("temp is:%d\r\n", temp);
//			MPU_Get_Accelerometer(&mpu6050_port, &aacx,&aacy,&aacz);	//得到加速度传感器数据
//			MPU_Get_Gyroscope(&mpu6050_port, &gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
//			MPU6050SendData(aacx,aacy,aacz,gyrox,gyroy,gyroz);//用自定义帧发送加速度和陀螺仪原始数据
//			//Uart0ReportImu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
//			printf("aacx:%d,aacy:%d,aacz:%d,gyrox:%d,gyroy:%d,gyroz:%d\r\n", aacx, aacy, aacz, gyrox, gyroy, gyroz);
		}	
		Delay_ms(500);
	}

}
