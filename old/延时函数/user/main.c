#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"

int main(void)
{
	Led_init();
	
	while(1)
	{
		//����
		GPIOF_ODR |= (0x01<<9);	//��9λ��1
		
		delay_ms(500);
		
		//����
		GPIOF_ODR &= ~(0x01<<9);	//��9λ��0		
		
		delay_ms(500);
	}
	
	return 0;
}
