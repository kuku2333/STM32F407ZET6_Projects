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

GPIO_InitTypeDef 		GPIO_InitStructure;
NVIC_InitTypeDef 		NVIC_InitStructure;
SPI_InitTypeDef 		SPI_InitStructure;

int main(void)
{
//	lcd_init();
//	
//	Uasrt1_Init();
//	
//	tp_init();
//	
//	/* 清屏为白色 */
//	lcd_clear(GREEN);	
//	
//	/* 显示方向， 竖屏 */
//	lcd_set_direction(0);
//	
////	lcd_draw_picture(0,0,240,280,YERSY_map);
////	lcd_fill(0,0,240,280,0x7F007F);
//	
//	uint16_t x = 0, y = 0;
//	tp_read(&x,&y);
//	printf("x:%d y:%d\r\n", x, y);
//	
//	while(1)
//	{
//		tp_read(&x,&y);
//		printf("x:%d y:%d\r\n", x, y);
//		
//		Delay_ms(500);
//	}
	Uasrt1_Init();
//	Tim14_Init();
//	spi1_tx_dma_init();
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
//		if (TP_IRQ)
//		{
//			PFout(9) = 1;
//		}
//		else
//		{
//			PFout(9) = 0;
//			Delay_s(3);
//		}
		
//		PBout(7) = 0;
//		Delay_ms(50);
//		PBout(7) = 1;
//		Delay_ms(50);
		lv_task_handler();
		Delay_ms(5);
	}
}
