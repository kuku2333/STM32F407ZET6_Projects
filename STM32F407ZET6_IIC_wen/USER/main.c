/**
   ******************************************************************************
   * @file    STM32F407ZET6_IIC_wen\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-11
   * @brief   
   ******************************************************************************
   * 宏需要定义在头文件中
   ******************************************************************************
**/

#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include "sys.h"
#include "Gpio.h"
#include "Tim.h"
#include "AT24C02.h"
#include "Uasrt_Send_String.h"
#include <stdio.h>

extern uint32_t g_cap_val;

int main(void)
{
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//Led_Out_Init(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_10, DISABLE);
	//Uasrt1_Init();
	//Tim14_PWM_Init();
	//Tim13_InputCapture_Init();
	//GPIO_Out_Init(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_9, DISABLE);
	
	uint8_t buf[8]={1,2,3,4,5,6,7,8};
	uint8_t get_buf[8]={0};
	
	//1.硬件的初始化
	Uasrt1_Init();
	
	//初始化at24c02
	AT24C02_Init();
	
	//往at24c02写入数据
	AT24C02_Write(0,buf,sizeof buf);
	
	Delay_ms(10);
	
	AT24C02_Read(0, get_buf, sizeof get_buf);
	
	for(uint8_t i = 0; i < 8; i++)
	{
		printf("The value of data_%d is: %d\r\n", i, get_buf[i]);
	}
	
	while(1)
	{
//		test();
		
	}
}
