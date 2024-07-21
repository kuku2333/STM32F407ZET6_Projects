/**
   ******************************************************************************
   * @file    STM32F407ZET6_1.69��LCD\USER\main.c 
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
	
	/* lcd��ʼ�� */
	lcd_init();
	
	/* ����Ϊ��ɫ */
	lcd_clear(WHITE);	
	
	/* ��ʾ���� ���� */
	lcd_set_direction(2);
	
	
	tp_init();
	
	/* ��ʾ���� */
	lcd_show_string(30,140,"TP Test By Teacher.Wen",RED,WHITE,16,0);
	
	while (1)
	{
		/* ����Ƿ��а��� */
		if(TP_IRQ==0)
		{
			/* �������� */
			while(1)
			{
				/* ��ȡ���������ֵ */
				if(tp_sta=tp_read(&tp_x,&tp_y))
				{
					/* ��ʾ��� */
					lcd_fill(tp_x,tp_y,5,5,RED);
					
					/* ����ϴε�����ֵ��ʾ���� */
					lcd_fill(20,g_lcd_height-20,160,20,BLUE);
					
					/* ��ʾ����ֵ */
					sprintf(buf,"x=%d y=%d sta=%d",tp_x,tp_y,tp_sta);
					lcd_show_string(20,g_lcd_height-20,buf,WHITE,BLUE,16,0);
					
				}		

				tp_recv_byte(0x02,&tp_finger_num);
				
				if(tp_finger_num == 0)
					break;				

				Delay_ms(10);
			}
			
			Delay_ms(2000);
			
			/* ���� */
			lcd_fill(0,0,g_lcd_width,g_lcd_height,WHITE);
			
			/* ��ʾ���� */
			lcd_show_string(30,140,"TP Test By Teacher.Wen",RED,WHITE,16,0);
		}		
	}
}
