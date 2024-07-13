/**
   ******************************************************************************
   * @file    USER/main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-06-27
   * @brief   配置寄存器来让控制开发板上的四个LED灯亮
   ******************************************************************************
   * 开发板上的四个LED灯与MCP的PF9、PF10、PE13、PE14相连。
   ******************************************************************************
 **/

//去到端口模式选择寄存器对应内存空间
#define GPIOF_MODER  	( * ( volatile unsigned int *) ( 0x40021400 + 0x00) )
#define GPIOE_MODER  	( * ( volatile unsigned int *) ( 0x40021000 + 0x00) )

//去到端口输出类型寄存器对应内存空间
#define GPIOF_OTYPER  	( * ( volatile unsigned int *) ( 0x40021400 + 0x04) )
#define GPIOE_OTYPER  	( * ( volatile unsigned int *) ( 0x40021000 + 0x04) )
	
//去到端口输出速度寄存器对应内存空间
#define GPIOF_OSPEEDR  	( * ( volatile unsigned int *) ( 0x40021400 + 0x08) )
#define GPIOE_OSPEEDR  	( * ( volatile unsigned int *) ( 0x40021000 + 0x08) )

//去到端口上拉/下拉寄存器对应内存空间
#define GPIOF_PUPDR  	( * ( volatile unsigned int *) ( 0x40021400 + 0x0C) )
#define GPIOE_PUPDR  	( * ( volatile unsigned int *) ( 0x40021000 + 0x0C) )

//去到端口输出数据寄存器对应内存空间
#define GPIOF_ODR  		( * ( volatile unsigned int *) ( 0x40021400 + 0x14) )
#define GPIOE_ODR  		( * ( volatile unsigned int *) ( 0x40021000 + 0x14) )

//去到AHB1使能寄存器对应内存空间
#define RCC_AHB1ENR  	( * ( volatile unsigned int *) ( 0x40023800 + 0x30) )

int main(void)
{	
	//打开时钟，GPIOE和GPIOF对应4和5位，置位使能。
	RCC_AHB1ENR  |=  ( 0x01 << 4);
	RCC_AHB1ENR  |=  ( 0x01 << 5);
	                   
	//01为输出模式     
	GPIOF_MODER  &= ~( 0x01 << (2 * 9 + 1));
	GPIOF_MODER  |=  ( 0x01 << (2 * 9));
	                   
	GPIOF_MODER  &= ~( 0x01 << (2 * 10 + 1));
	GPIOF_MODER  |=  ( 0x01 << (2 * 10));
	                   
	GPIOE_MODER  &= ~( 0x01 << (2 * 13 + 1));
	GPIOE_MODER  |=  ( 0x01 << (2 * 13));
	                   
	GPIOE_MODER  &= ~( 0x01 << (2 * 14 + 1));
	GPIOE_MODER  |=  ( 0x01 << (2 * 14));
	                   
	//0为推挽输出      
	GPIOF_OTYPER &= ~( 0x01 << (2 * 9));
	GPIOF_OTYPER &= ~( 0x01 << (2 * 10));
	GPIOE_OTYPER &= ~( 0x01 << (2 * 13));
	GPIOE_OTYPER &= ~( 0x01 << (2 * 14));
	
	//11为最高速度
	GPIOF_OSPEEDR |=  ( 0x01 << (2 * 9 + 1));
	GPIOF_OSPEEDR |=  ( 0x01 << (2 * 9));
	                    
	GPIOF_OSPEEDR |=  ( 0x01 << (2 * 10 + 1));
	GPIOF_OSPEEDR |=  ( 0x01 << (2 * 10));
	                    
	GPIOE_OSPEEDR |=  ( 0x01 << (2 * 13 +1));
	GPIOE_OSPEEDR |=  ( 0x01 << (2 * 13));
	                    
	GPIOE_OSPEEDR |=  ( 0x01 << (2 * 14 +1));
	GPIOE_OSPEEDR |=  ( 0x01 << (2 * 14));
	
	//00为不选择上拉下拉
	GPIOF_PUPDR  &= ~( 0x01 << (2 * 9 + 1));
	GPIOF_PUPDR  &= ~( 0x01 << (2 * 9));
	                   
	GPIOF_PUPDR  &= ~( 0x01 << (2 * 10 + 1));
	GPIOF_PUPDR  &= ~( 0x01 << (2 * 10));
	                   
	GPIOE_PUPDR  &= ~( 0x01 << (2 * 13 + 1));
	GPIOE_PUPDR  &= ~( 0x01 << (2 * 13));
	                   
	GPIOE_PUPDR  &= ~( 0x01 << (2 * 14 + 1));
	GPIOE_PUPDR  &= ~( 0x01 << (2 * 14));
	
	while(1)
	{
		//复位点亮灯
		GPIOF_ODR &= ~( 0x01 << (2 * 9));
		GPIOF_ODR &= ~( 0x01 << (2 * 10));
		GPIOE_ODR &= ~( 0x01 << (2 * 13));
		GPIOE_ODR &= ~( 0x01 << (2 * 14));
	} 
}
