/****************************************************************
*��    ��:�������Գ���1.69Ӣ������
*��    ��:��Ƕ.�¹�
*��������:2024/1/17
*֪ ʶ ��:
	1.Ӳ��SPI��ʽ����tft����ʾ
	2.֧�ִ�������
		.��TP_PIN_DEF�����˴������Ų�ͬ�����ӷ�ʽ�����Դ���ǰ����˽�ú�
*˵  ��:
	1.��ʾ������оƬΪST7789V
	2.����������оƬΪCST816T
	3.������Ļʵʱ��ʾ����ֵ�����
*ע  ��:
*****************************************************************/
#include "includes.h"

GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
SPI_InitTypeDef SPI_InitStructure;



// ������
int main(void)
{
	uint8_t buf[32]={0};
	
	uint16_t tp_x,tp_y;
	
	uint8_t tp_finger_num=0;
	
	uint8_t tp_sta;
	
	usart1_init(115200);
	
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

				delay_ms(10);
			}
			
			delay_ms(2000);
			
			/* ���� */
			lcd_fill(0,0,g_lcd_width,g_lcd_height,WHITE);
			
			/* ��ʾ���� */
			lcd_show_string(30,140,"TP Test By Teacher.Wen",RED,WHITE,16,0);
		}		
	}
	return 0;
}
