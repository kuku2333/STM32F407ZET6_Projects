
#define RCC_AHB1RSTR    *(volatile unsigned int *)(0x40023800 + 0x30)

#define GPIOE_MODER  *(volatile unsigned int *)(0X40021000+0x00)//GPIO �˿�ģʽ�Ĵ���

#define GPIOE_OSPEEDR  *(volatile unsigned int *)(0X40021000+0x08)//GPIO �˿�����ٶȼĴ���

#define GPIOE_IDR  *(volatile unsigned int *)(0X40021000+0x10)//GPIO �˿��������ݼĴ���

#define GPIOE_ODR  *(volatile unsigned int *)(0X40021000+0x14)//GPIO �˿�������ݼĴ���

#define GPIO_PIN_KEY 4

#define GPIO_PIN_LED 14

void key_init()
{
	RCC_AHB1RSTR |= (1<<4);
											   //����,����Ϊ�͵�ƽ
	GPIOE_OSPEEDR |= (1<<((2*GPIO_PIN_KEY)+1));//����
	GPIOE_OSPEEDR |= (1<<(2*GPIO_PIN_KEY));
	
}

void led_init()
{
	RCC_AHB1RSTR |= (1<<4); //E
	
	GPIOE_MODER &= ~(1<<((2*GPIO_PIN_LED)+1));//���
	GPIOE_MODER |=  (1<<(2*GPIO_PIN_LED));
	
	
	GPIOE_OSPEEDR |=  (1<<(2*GPIO_PIN_LED+1));//����
	GPIOE_OSPEEDR |=  (1<<(2*GPIO_PIN_LED));	
	
	GPIOE_ODR |= (1<<GPIO_PIN_LED); //��ʼ��ƽΪ�ߣ���
	
}
int main()
{
	key_init();
	led_init();
	
	
	while(1)
	{
	
	unsigned int odr_value = GPIOE_IDR;

	
	unsigned int bit4_value = (odr_value >> 4) & 0x1;  // ��4λ��Ӧ��λ����3����Ϊ��0��ʼ��

	
	if (bit4_value == 0) 
	{
		GPIOE_ODR &= ~(1<<GPIO_PIN_LED);

	} else 
	{
		GPIOE_ODR |= (1<<GPIO_PIN_LED);

}
	}

}