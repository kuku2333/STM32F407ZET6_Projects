/**
   ******************************************************************************
   * @file    STM32F407ZET6_ͨ��ICC��\USER\main.c 
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

//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void Uart0NimingReport(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X88;	//֡ͷ
	send_buf[1]=fun;	//������
	send_buf[2]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//��������
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//����У���	
	//for(i=0;i<len+4;i++)printf("send_buf%d is: %d\r\n", i, send_buf[i]);//�������ݵ�����1 
}

//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
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
	Uart0NimingReport(0XA1,tbuf,12);//�Զ���֡,0XA1
	
}	
//ͨ������0�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void Uart0ReportImu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//��0
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
	Uart0NimingReport(0XAF,tbuf,28);//�ɿ���ʾ֡,0XAF
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
	
	float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	short temp;					//�¶�		
	
	/* Configure the Priority Group to 2 bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//����1��ʼ�����Ҳ�����Ϊ115200bps
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
//			temp=MPU_Get_Temperature(&mpu6050_port);	//�õ��¶�ֵ
//			printf("temp is:%d\r\n", temp);
//			MPU_Get_Accelerometer(&mpu6050_port, &aacx,&aacy,&aacz);	//�õ����ٶȴ���������
//			MPU_Get_Gyroscope(&mpu6050_port, &gyrox,&gyroy,&gyroz);	//�õ�����������
//			MPU6050SendData(aacx,aacy,aacz,gyrox,gyroy,gyroz);//���Զ���֡���ͼ��ٶȺ�������ԭʼ����
//			//Uart0ReportImu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
//			printf("aacx:%d,aacy:%d,aacz:%d,gyrox:%d,gyroy:%d,gyroz:%d\r\n", aacx, aacy, aacz, gyrox, gyroy, gyroz);
		}	
		Delay_ms(500);
	}

}
