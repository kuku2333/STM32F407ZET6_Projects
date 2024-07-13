/**
   ******************************************************************************
   * @file    STM32F407ZET6_按键控制LED_寄存器\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-06-28
   * @brief   用KEY0控制LED0，按下亮，松开灭。用寄存器实现。
   ******************************************************************************
   * 开发板上的四个LED灯与MCU的PF9、PF10、PE13、PE14相连。
   * KEY0与PA0相连。
   ******************************************************************************
 **/
 
typedef unsigned short int		uint16_t;
typedef unsigned int 			uint32_t;
 
 //去到端口模式选择寄存器对应内存空间
#define GPIOF_MODER  	( * ( volatile unsigned int *) ( 0x40021400 + 0x00) )
#define GPIOA_MODER  	( * ( volatile unsigned int *) ( 0x40020000 + 0x00) )

//去到端口输出类型寄存器对应内存空间
#define GPIOF_OTYPER  	( * ( volatile unsigned int *) ( 0x40021400 + 0x04) )
#define GPIOA_OTYPER  	( * ( volatile unsigned int *) ( 0x40020000 + 0x04) )
	
//去到端口输出速度寄存器对应内存空间
#define GPIOF_OSPEEDR  	( * ( volatile unsigned int *) ( 0x40021400 + 0x08) )
#define GPIOA_OSPEEDR  	( * ( volatile unsigned int *) ( 0x40020000 + 0x08) )

//去到端口上拉/下拉寄存器对应内存空间
#define GPIOF_PUPDR  	( * ( volatile unsigned int *) ( 0x40021400 + 0x0C) )
#define GPIOE_PUPDR  	( * ( volatile unsigned int *) ( 0x40020000 + 0x0C) )

//去到端口输出数据寄存器对应内存空间
#define GPIOF_ODR  		( * ( volatile unsigned int *) ( 0x40021400 + 0x14) )
#define GPIOE_ODR  		( * ( volatile unsigned int *) ( 0x40020000 + 0x14) )

//去到AHB1使能寄存器对应内存空间
#define RCC_AHB1ENR  	( * ( volatile unsigned int *) ( 0x40023800 + 0x30) )

//去到端口输入寄存器对应内存空间
#define GPIOA_IDR  		( * ( volatile unsigned int *) ( 0x40020000 + 0x10) )

//引脚0
#define GPIO_Pin_0      ((uint16_t)0x0001)

//置位和复位
#define Bit_RESET		0
#define Bit_SET			1

void LED_Init(void)
{
	//打开时钟，GPIOF对应5，置位使能。
	RCC_AHB1ENR  |=  ( 0x01 << 5);
	
	//01为输出模式     
	GPIOF_MODER  &= ~( 0x01 << (2 * 9 + 1));
	GPIOF_MODER  |=  ( 0x01 << (2 * 9));
	
	//0为推挽输出      
	GPIOF_OTYPER &= ~( 0x01 << (2 * 9));
	
	//11为最高速度
	GPIOF_OSPEEDR |=  ( 0x01 << (2 * 9 + 1));
	GPIOF_OSPEEDR |=  ( 0x01 << (2 * 9));
	
	//00为不选择上拉下拉
	GPIOF_PUPDR  &= ~( 0x01 << (2 * 9 + 1));
	GPIOF_PUPDR  &= ~( 0x01 << (2 * 9));
	
	//默认不亮,置位
	GPIOF_ODR |=  ( 0x01 << (9));
}

void KEY_Init(void)
{
	//打开时钟，GPIOA对应0，置位使能。
	RCC_AHB1ENR  |=  ( 0x01 << 2 * 0);
	
	//00为输入模式     
	GPIOF_MODER  &= ~( 0x01 << (2 * 0 + 1));
	GPIOF_MODER  &= ~( 0x01 << (2 * 0));
	
	//00为不选择上拉下拉
	GPIOF_PUPDR  &= ~( 0x01 << (2 * 0 + 1));
	GPIOF_PUPDR  &= ~( 0x01 << (2 * 0));
}

int main(void)
{
	LED_Init();//LED初始化
	KEY_Init();//KEY初始化
	
	while(1)
	{
		//循环检测GPIOA的端口输入寄存器的0位是否为0，如果是则
		//代表按键按下，则LED0亮。
	 	if (!((GPIOA_IDR & GPIO_Pin_0) != (uint32_t)Bit_RESET))
		{
			GPIOF_ODR &= ~( 0x01 << (9));//ODR低16位才被使用
		}
		else
		{
			GPIOF_ODR |=  ( 0x01 << (9));
		}
	}
}
