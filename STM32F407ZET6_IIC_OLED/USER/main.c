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
//#include "AT24C02.h"
#include "Uasrt_Send_String.h"
#include "OLED.h"
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
	uint8_t t;
	OLED_Init();			//初始化OLED  
	OLED_Clear(); 
	t=' ';
	
	while(1)
	{
//		test();
		OLED_Clear();
		OLED_ShowString(8,2,"ZHONGJINGYUAN", 16);
		Delay_ms(8000);
		
	}
}
