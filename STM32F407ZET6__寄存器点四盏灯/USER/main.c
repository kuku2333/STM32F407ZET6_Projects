/**
   ******************************************************************************
   * @file    USER/main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-06-27
   * @brief   ���üĴ������ÿ��ƿ������ϵ��ĸ�LED����
   ******************************************************************************
   * �������ϵ��ĸ�LED����MCP��PF9��PF10��PE13��PE14������
   ******************************************************************************
 **/

//ȥ���˿�ģʽѡ��Ĵ�����Ӧ�ڴ�ռ�
#define GPIOF_MODER  	( * ( volatile unsigned int *) ( 0x40021400 + 0x00) )
#define GPIOE_MODER  	( * ( volatile unsigned int *) ( 0x40021000 + 0x00) )

//ȥ���˿�������ͼĴ�����Ӧ�ڴ�ռ�
#define GPIOF_OTYPER  	( * ( volatile unsigned int *) ( 0x40021400 + 0x04) )
#define GPIOE_OTYPER  	( * ( volatile unsigned int *) ( 0x40021000 + 0x04) )
	
//ȥ���˿�����ٶȼĴ�����Ӧ�ڴ�ռ�
#define GPIOF_OSPEEDR  	( * ( volatile unsigned int *) ( 0x40021400 + 0x08) )
#define GPIOE_OSPEEDR  	( * ( volatile unsigned int *) ( 0x40021000 + 0x08) )

//ȥ���˿�����/�����Ĵ�����Ӧ�ڴ�ռ�
#define GPIOF_PUPDR  	( * ( volatile unsigned int *) ( 0x40021400 + 0x0C) )
#define GPIOE_PUPDR  	( * ( volatile unsigned int *) ( 0x40021000 + 0x0C) )

//ȥ���˿�������ݼĴ�����Ӧ�ڴ�ռ�
#define GPIOF_ODR  		( * ( volatile unsigned int *) ( 0x40021400 + 0x14) )
#define GPIOE_ODR  		( * ( volatile unsigned int *) ( 0x40021000 + 0x14) )

//ȥ��AHB1ʹ�ܼĴ�����Ӧ�ڴ�ռ�
#define RCC_AHB1ENR  	( * ( volatile unsigned int *) ( 0x40023800 + 0x30) )

int main(void)
{	
	//��ʱ�ӣ�GPIOE��GPIOF��Ӧ4��5λ����λʹ�ܡ�
	RCC_AHB1ENR  |=  ( 0x01 << 4);
	RCC_AHB1ENR  |=  ( 0x01 << 5);
	                   
	//01Ϊ���ģʽ     
	GPIOF_MODER  &= ~( 0x01 << (2 * 9 + 1));
	GPIOF_MODER  |=  ( 0x01 << (2 * 9));
	                   
	GPIOF_MODER  &= ~( 0x01 << (2 * 10 + 1));
	GPIOF_MODER  |=  ( 0x01 << (2 * 10));
	                   
	GPIOE_MODER  &= ~( 0x01 << (2 * 13 + 1));
	GPIOE_MODER  |=  ( 0x01 << (2 * 13));
	                   
	GPIOE_MODER  &= ~( 0x01 << (2 * 14 + 1));
	GPIOE_MODER  |=  ( 0x01 << (2 * 14));
	                   
	//0Ϊ�������      
	GPIOF_OTYPER &= ~( 0x01 << (2 * 9));
	GPIOF_OTYPER &= ~( 0x01 << (2 * 10));
	GPIOE_OTYPER &= ~( 0x01 << (2 * 13));
	GPIOE_OTYPER &= ~( 0x01 << (2 * 14));
	
	//11Ϊ����ٶ�
	GPIOF_OSPEEDR |=  ( 0x01 << (2 * 9 + 1));
	GPIOF_OSPEEDR |=  ( 0x01 << (2 * 9));
	                    
	GPIOF_OSPEEDR |=  ( 0x01 << (2 * 10 + 1));
	GPIOF_OSPEEDR |=  ( 0x01 << (2 * 10));
	                    
	GPIOE_OSPEEDR |=  ( 0x01 << (2 * 13 +1));
	GPIOE_OSPEEDR |=  ( 0x01 << (2 * 13));
	                    
	GPIOE_OSPEEDR |=  ( 0x01 << (2 * 14 +1));
	GPIOE_OSPEEDR |=  ( 0x01 << (2 * 14));
	
	//00Ϊ��ѡ����������
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
		//��λ������
		GPIOF_ODR &= ~( 0x01 << (2 * 9));
		GPIOF_ODR &= ~( 0x01 << (2 * 10));
		GPIOE_ODR &= ~( 0x01 << (2 * 13));
		GPIOE_ODR &= ~( 0x01 << (2 * 14));
	} 
}
