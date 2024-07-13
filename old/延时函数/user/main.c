#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"

int main(void)
{
	Led_init();
	
	while(1)
	{
		//灯灭
		GPIOF_ODR |= (0x01<<9);	//第9位置1
		
		delay_ms(500);
		
		//灯亮
		GPIOF_ODR &= ~(0x01<<9);	//第9位清0		
		
		delay_ms(500);
	}
	
	return 0;
}
