
#define RCC_AHB1RSTR    *(volatile unsigned int *)(0x40023800 + 0x30)

#define GPIOE_MODER  *(volatile unsigned int *)(0X40021000+0x00)//GPIO 端口模式寄存器

#define GPIOE_OSPEEDR  *(volatile unsigned int *)(0X40021000+0x08)//GPIO 端口输出速度寄存器

#define GPIOE_IDR  *(volatile unsigned int *)(0X40021000+0x10)//GPIO 端口输入数据寄存器

#define GPIOE_ODR  *(volatile unsigned int *)(0X40021000+0x14)//GPIO 端口输出数据寄存器

#define GPIO_PIN_KEY 4

#define GPIO_PIN_LED 14

void key_init()
{
	RCC_AHB1RSTR |= (1<<4);
											   //输入,按下为低电平
	GPIOE_OSPEEDR |= (1<<((2*GPIO_PIN_KEY)+1));//高速
	GPIOE_OSPEEDR |= (1<<(2*GPIO_PIN_KEY));
	
}

void led_init()
{
	RCC_AHB1RSTR |= (1<<4); //E
	
	GPIOE_MODER &= ~(1<<((2*GPIO_PIN_LED)+1));//输出
	GPIOE_MODER |=  (1<<(2*GPIO_PIN_LED));
	
	
	GPIOE_OSPEEDR |=  (1<<(2*GPIO_PIN_LED+1));//高速
	GPIOE_OSPEEDR |=  (1<<(2*GPIO_PIN_LED));	
	
	GPIOE_ODR |= (1<<GPIO_PIN_LED); //初始电平为高，灭
	
}
int main()
{
	key_init();
	led_init();
	
	
	while(1)
	{
	
	unsigned int odr_value = GPIOE_IDR;

	
	unsigned int bit4_value = (odr_value >> 4) & 0x1;  // 第4位对应于位索引3，因为从0开始数

	
	if (bit4_value == 0) 
	{
		GPIOE_ODR &= ~(1<<GPIO_PIN_LED);

	} else 
	{
		GPIOE_ODR |= (1<<GPIO_PIN_LED);

}
	}

}