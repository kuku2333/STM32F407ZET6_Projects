/**
   ******************************************************************************
   * @file    STM32F407ZET6_SPI_W25Q128\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-15
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include "sys.h"
#include "Uasrt_Send_String.h"
#include "W25Q128.h"

int main(void)
{
	uint16_t id;
	
	W25Q128_Config();
	Uasrt1_Init();
	
	while(1)
	{
		id = W25Q128_Get_ID();
		
		printf("id: %d\r\n", id);
	}
}
