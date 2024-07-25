/**
   ******************************************************************************
   * @file    STM32F407ZET6_外设完备版\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-09
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include "sys.h"
#include "Gpio.h"
#include "Tim.h"
#include "Uasrt_Send_String.h"
#include "MAX30102.h"
#include "Algorithm.h"

GPIO_InitTypeDef		GPIO_InitStruct;

uint32_t aun_ir_buffer[500]; //IR LED sensor data
int32_t n_ir_buffer_length;    //data length
uint32_t aun_red_buffer[500];    //Red LED sensor data
int32_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;   //heart rate value
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;

#define MAX_BRIGHTNESS 255

/*	先配数据线，再配时钟线
	PA4-->Camera5, PB7-->Camera3
	MAX30102的中断线配Camera7-->PG15
*/
I2C_Port_t max30102_port = {
								GPIOA, GPIOB,
								GPIO_Pin_4, GPIO_Pin_7, 
								RCC_AHB1Periph_GPIOA, RCC_AHB1Periph_GPIOB
							};

int main(void)
{
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//variables to calculate the on-board LED brightness that reflects the heartbeats
	uint32_t un_min, un_max, un_prev_data;  
	int32_t i;
	int32_t n_brightness;
	float f_temp;

	uint8_t temp[6];
	uint8_t dis_hr=0,dis_spo2=0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	Uasrt1_Init();
	
	max30102_init(&max30102_port);

	printf("This is max30102 test.\r\n");

//	uint8_t data = 0;
//	data = MAX30102_Bus_Read(0xFE);
//	printf("Revision ID：%d\r\n", data);
//	data = MAX30102_Bus_Read(0xFF);
//	printf("Part ID：%d\r\n", data);
//	data = MAX30102_Bus_Read(0x00);//清除A_FULL
//	printf("INT STATUS 1：%d\r\n", data);
//	data = MAX30102_Bus_Read(0x01);//清除A_FULL
//	printf("INT STATUS 2：%d\r\n", data);
//	while(MAX30102_INT==0)
//	{
//		printf("continue 1\r\n");
//	}

//	MAX30102_Reset();
//	
//	MAX30102_Bus_Write(REG_MODE_CONFIG,0x03);
//	MAX30102_Bus_Write(REG_INTR_ENABLE_2,0x02);
//	MAX30102_Bus_Write(REG_INTR_ENABLE_1,0x80);	// INTR setting
//	
//	MAX30102_Bus_Write(REG_INTR_ENABLE_1,0x40);	// INTR setting
	
	
//	for(uint32_t j = 0; j < 100; j++)
//	{
//		while(MAX30102_INT==1);
//		MAX30102_FIFO_Read_Bytes(REG_FIFO_DATA,temp);
//		for(uint8_t i = 0; i < 6; i++)
//		{
//			printf("temp %d is: %d\r\n", i, temp[i]);
//		}
//		
//		Delay_ms(1000);
//	}
	
	un_min=0x3FFFF;
	un_max=0;
	
	n_ir_buffer_length=500; //buffer length of 100 stores 5 seconds of samples running at 100sps
	//read the first 500 samples, and determine the signal range
    for(i=0;i<n_ir_buffer_length;i++)
    {
        while(MAX30102_INT==1);   //wait until the interrupt pin asserts
        
		max30102_FIFO_ReadBytes(&max30102_port, REG_FIFO_DATA,temp);
		aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
		aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
            
        if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];    //update signal min
        if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];    //update signal max
    }
	un_prev_data=aun_red_buffer[i];
	//calculate heart rate and SpO2 after first 500 samples (first 5 seconds of samples)
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
	
	while(1)
	{
		i=0;
        un_min=0x3FFFF;
        un_max=0;
		
		//dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
        for(i=100;i<500;i++)
        {
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];
            
            //update the signal min and max
            if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];
            if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];
        }
		//take 100 sets of samples before calculating the heart rate.
        for(i=400;i<500;i++)
        {
            un_prev_data=aun_red_buffer[i-1];
            while(MAX30102_INT==1);
            max30102_FIFO_ReadBytes(&max30102_port, REG_FIFO_DATA,temp);
			aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
			aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
        
            if(aun_red_buffer[i]>un_prev_data)
            {
                f_temp=aun_red_buffer[i]-un_prev_data;
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness-=(int)f_temp;
                if(n_brightness<0)
                    n_brightness=0;
            }
            else
            {
                f_temp=un_prev_data-aun_red_buffer[i];
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness+=(int)f_temp;
                if(n_brightness>MAX_BRIGHTNESS)
                    n_brightness=MAX_BRIGHTNESS;
            }
			//send samples and calculation result to terminal program through UART
			if(ch_hr_valid == 1 && n_heart_rate<120)//**/ ch_hr_valid == 1 && ch_spo2_valid ==1 && n_heart_rate<120 && n_sp02<101
			{
				dis_hr = n_heart_rate;
				dis_spo2 = n_sp02;
			}
			else
			{
				dis_hr = 0;
				dis_spo2 = 0;
			}
//				printf("HR=%i, ", n_heart_rate); 
//				printf("HRvalid=%i, ", ch_hr_valid);
//				printf("SpO2=%i, ", n_sp02);
//				printf("SPO2Valid=%i\r\n", ch_spo2_valid);
		}

		/* 计算心率和血氧饱和度 */
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
		
		/* 通过UART将样本和计算结果发送到终端程序 */
		//if((ch_hr_valid == 1)&& (n_heart_rate>=60) && (n_heart_rate<=130))
		{

			printf("心率=%d\r\n", dis_hr); 
		}

		
		//if((ch_spo2_valid ==1)&& (n_sp02>=95) && (n_sp02<=100))
		{
			//printf("心率=%d\r\n", dis_hr); 
			printf("血氧浓度=%d\r\n", dis_spo2); 
			printf("\r\n"); 
		}			
	
		Delay_ms(500);
	}
}
