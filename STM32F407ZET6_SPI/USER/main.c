/****************************************************************
*名    称:触屏测试程序（1.69英寸屏）
*作    者:粤嵌.温工
*创建日期:2024/1/17
*知 识 点:
	1.硬件SPI方式驱动tft屏显示
	2.支持触摸控制
		.宏TP_PIN_DEF决定了触摸引脚不同的连接方式，测试代码前务必了解该宏
*说  明:
	1.显示的驱动芯片为ST7789V
	2.触摸的驱动芯片为CST816T
	3.滑动屏幕实时显示坐标值与描点
*注  意:
*****************************************************************/
#include "includes.h"

GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
SPI_InitTypeDef SPI_InitStructure;



// 主函数
int main(void)
{
	uint8_t buf[32]={0};
	
	uint16_t tp_x,tp_y;
	
	uint8_t tp_finger_num=0;
	
	uint8_t tp_sta;
	
	usart1_init(115200);
	
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

				delay_ms(10);
			}
			
			delay_ms(2000);
			
			/* 清屏 */
			lcd_fill(0,0,g_lcd_width,g_lcd_height,WHITE);
			
			/* 显示标题 */
			lcd_show_string(30,140,"TP Test By Teacher.Wen",RED,WHITE,16,0);
		}		
	}
	return 0;
}
