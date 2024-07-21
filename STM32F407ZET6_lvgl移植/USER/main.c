/**
   ******************************************************************************
   * @file    STM32F407ZET6_lvgl��ֲ\USER\main.c 
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
	// ��ʼ��lvgl
	lv_init();
	
	// ��ʼ��lvgl��ʾ�豸
	lv_port_disp_init();
	
	// ��ʼ��lvgl�����豸
	lv_port_indev_init();
	
	// ��ʼ��lvgl demo
	lv_demo_widgets();
	
	// tim3��ʼ������ʱ����Ϊ1ms
	tim3_init();
	
	while(1)
	{
		lv_task_handler();
		Delay_ms(5);
	}
}
