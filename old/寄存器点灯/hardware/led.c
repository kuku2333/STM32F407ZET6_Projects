#include "led.h"

/*******************************

********************************/

void Led_init(void)
{
	//打开GPIOF组时钟(PF9属于GPIOF组时钟)
	RCC_AHB1ENR |= (0x01<<5);		//第5位置1
	
	//通用输出模式
	GPIOF_MODER &= ~(0x01<<19);		//第19位清0
	GPIOF_MODER |= (0x01<<18);		//第18位置1

	//输出推挽
	GPIOF_OTYPER &= ~(0x01<<9);		//第9位清0
	
	
	//输出速度25MHZ
	GPIOF_OSPEEDR &= ~(0x01<<19);		//第19位清0
	GPIOF_OSPEEDR |= (0x01<<18);		//第18位置1
	
	//设置为上拉
	GPIOF_PUPDR &= ~(0x01<<19);		//第19位清0
	GPIOF_PUPDR |= (0x01<<18);		//第18位置1	
	
}
