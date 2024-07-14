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

int main(void)
{
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	uint32_t fre = 0;
	
	Uasrt1_Init();
	
	//Tim14_PWM_Init();
	//GPIO_Out_Init(RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_9, DISABLE);
	//Tim13_InputCapture_Init();
	//Wave_Init();
	//Tim3_CH2_PWM_Init();
	Wave_Init();
	Tim13_InputCapture_Init();
	
	while(1)
	{
//		fre = Get_Frequence();
//		
//		printf("Frequence: %d\r\n", fre);
		//Breath_Light();
		Input_Capture_test();
	}
}
