/**
   ******************************************************************************
   * @file    STM32F407ZET6_��������LED_�Ĵ���\USER\main.c 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-06-28
   * @brief   ��KEY0����LED0�����������ɿ����üĴ���ʵ�֡�
   ******************************************************************************
   * �������ϵ��ĸ�LED����MCU��PF9��PF10��PE13��PE14������
   * KEY0��PA0������
   ******************************************************************************
 **/
 
typedef unsigned short int		uint16_t;
typedef unsigned int 			uint32_t;
 
 //ȥ���˿�ģʽѡ��Ĵ�����Ӧ�ڴ�ռ�
#define GPIOF_MODER  	( * ( volatile unsigned int *) ( 0x40021400 + 0x00) )
#define GPIOA_MODER  	( * ( volatile unsigned int *) ( 0x40020000 + 0x00) )

//ȥ���˿�������ͼĴ�����Ӧ�ڴ�ռ�
#define GPIOF_OTYPER  	( * ( volatile unsigned int *) ( 0x40021400 + 0x04) )
#define GPIOA_OTYPER  	( * ( volatile unsigned int *) ( 0x40020000 + 0x04) )
	
//ȥ���˿�����ٶȼĴ�����Ӧ�ڴ�ռ�
#define GPIOF_OSPEEDR  	( * ( volatile unsigned int *) ( 0x40021400 + 0x08) )
#define GPIOA_OSPEEDR  	( * ( volatile unsigned int *) ( 0x40020000 + 0x08) )

//ȥ���˿�����/�����Ĵ�����Ӧ�ڴ�ռ�
#define GPIOF_PUPDR  	( * ( volatile unsigned int *) ( 0x40021400 + 0x0C) )
#define GPIOE_PUPDR  	( * ( volatile unsigned int *) ( 0x40020000 + 0x0C) )

//ȥ���˿�������ݼĴ�����Ӧ�ڴ�ռ�
#define GPIOF_ODR  		( * ( volatile unsigned int *) ( 0x40021400 + 0x14) )
#define GPIOE_ODR  		( * ( volatile unsigned int *) ( 0x40020000 + 0x14) )

//ȥ��AHB1ʹ�ܼĴ�����Ӧ�ڴ�ռ�
#define RCC_AHB1ENR  	( * ( volatile unsigned int *) ( 0x40023800 + 0x30) )

//ȥ���˿�����Ĵ�����Ӧ�ڴ�ռ�
#define GPIOA_IDR  		( * ( volatile unsigned int *) ( 0x40020000 + 0x10) )

//����0
#define GPIO_Pin_0      ((uint16_t)0x0001)

//��λ�͸�λ
#define Bit_RESET		0
#define Bit_SET			1

void LED_Init(void)
{
	//��ʱ�ӣ�GPIOF��Ӧ5����λʹ�ܡ�
	RCC_AHB1ENR  |=  ( 0x01 << 5);
	
	//01Ϊ���ģʽ     
	GPIOF_MODER  &= ~( 0x01 << (2 * 9 + 1));
	GPIOF_MODER  |=  ( 0x01 << (2 * 9));
	
	//0Ϊ�������      
	GPIOF_OTYPER &= ~( 0x01 << (2 * 9));
	
	//11Ϊ����ٶ�
	GPIOF_OSPEEDR |=  ( 0x01 << (2 * 9 + 1));
	GPIOF_OSPEEDR |=  ( 0x01 << (2 * 9));
	
	//00Ϊ��ѡ����������
	GPIOF_PUPDR  &= ~( 0x01 << (2 * 9 + 1));
	GPIOF_PUPDR  &= ~( 0x01 << (2 * 9));
	
	//Ĭ�ϲ���,��λ
	GPIOF_ODR |=  ( 0x01 << (9));
}

void KEY_Init(void)
{
	//��ʱ�ӣ�GPIOA��Ӧ0����λʹ�ܡ�
	RCC_AHB1ENR  |=  ( 0x01 << 2 * 0);
	
	//00Ϊ����ģʽ     
	GPIOF_MODER  &= ~( 0x01 << (2 * 0 + 1));
	GPIOF_MODER  &= ~( 0x01 << (2 * 0));
	
	//00Ϊ��ѡ����������
	GPIOF_PUPDR  &= ~( 0x01 << (2 * 0 + 1));
	GPIOF_PUPDR  &= ~( 0x01 << (2 * 0));
}

int main(void)
{
	LED_Init();//LED��ʼ��
	KEY_Init();//KEY��ʼ��
	
	while(1)
	{
		//ѭ�����GPIOA�Ķ˿�����Ĵ�����0λ�Ƿ�Ϊ0���������
		//���������£���LED0����
	 	if (!((GPIOA_IDR & GPIO_Pin_0) != (uint32_t)Bit_RESET))
		{
			GPIOF_ODR &= ~( 0x01 << (9));//ODR��16λ�ű�ʹ��
		}
		else
		{
			GPIOF_ODR |=  ( 0x01 << (9));
		}
	}
}
