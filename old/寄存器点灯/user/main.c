#include "stm32f4xx.h"
#include "led.h"

//����ʱ
void delay(int n)
{
	int i, j;
	
	for(i=0; i<n; i++)
	{
		for(j=0; j<30000; j++)
		{
		
		}
	}
			
}

int main(void)
{
	Led_init();
	
	while(1)
	{
		//����
		GPIOF_ODR |= (0x01<<9);	//��9λ��1
		
		delay(500);
		
		//����
		GPIOF_ODR &= ~(0x01<<9);	//��9λ��0		
		
		delay(500);
	}
	
	return 0;
}
