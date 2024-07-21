/**
   ******************************************************************************
   * @file    STM32F407ZET6_1.69存LCD\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-19
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#include "stm32f4xx.h"                  // Device header
#include "tft.h"
#include "Delay.h"
#include "sys.h"
#include "touch.h"
#include "Uasrt_Send_String.h"

//GPIO_InitTypeDef GPIO_InitStructure;

int main(void)
{
	uint8_t buf[32]={0};
	
	uint16_t tp_x,tp_y;
	
	uint8_t tp_finger_num=0;
	
	uint8_t tp_sta;
	
	Uasrt1_Init();
	
	/* lcd初始化 */
	lcd_init();
	
	/* 清屏为白色 */
	lcd_clear(WHITE);	
	
	/* 显示方向， 竖屏 */
	lcd_set_direction(2);
	
	
	tp_init();
	
	/* 显示标题 */
	lcd_show_string(30,140,"TP Test By Teacher.Wen",RED,WHITE,16,0);
	
	while (1)
	{
		/* 检测是否有按下 */
		if(TP_IRQ==0)
		{
			/* 持续访问 */
			while(1)
			{
				/* 获取点击的坐标值 */
				if(tp_sta=tp_read(&tp_x,&tp_y))
				{
					/* 显示描点 */
					lcd_fill(tp_x,tp_y,5,5,RED);
					
					/* 清除上次的坐标值显示区域 */
					lcd_fill(20,g_lcd_height-20,160,20,BLUE);
					
					/* 显示坐标值 */
					sprintf(buf,"x=%d y=%d sta=%d",tp_x,tp_y,tp_sta);
					lcd_show_string(20,g_lcd_height-20,buf,WHITE,BLUE,16,0);
					
				}		

				tp_recv_byte(0x02,&tp_finger_num);
				
				if(tp_finger_num == 0)
					break;				

				Delay_ms(10);
			}
			
			Delay_ms(2000);
			
			/* 清屏 */
			lcd_fill(0,0,g_lcd_width,g_lcd_height,WHITE);
			
			/* 显示标题 */
			lcd_show_string(30,140,"TP Test By Teacher.Wen",RED,WHITE,16,0);
		}		
	}
}
