/**
   ******************************************************************************
   * @file    STM32F407ZET6_lvgl移植\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-19
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#include "stm32f4xx.h"                  // Device header
#include "includes.h"

int main(void)
{
	// 初始化lvgl
	lv_init();
	
	// 初始化lvgl显示设备
	lv_port_disp_init();
	
	// 初始化lvgl输入设备
	lv_port_indev_init();
	
	// 初始化lvgl demo
	lv_demo_widgets();
	
	// tim3初始化，定时周期为1ms
	tim3_init();
	
	while(1)
	{
		lv_task_handler();
		Delay_ms(5);
	}
}
