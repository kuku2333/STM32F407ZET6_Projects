/*作    者:粤嵌.温工 */

#include "includes.h"



/*创建配置结构体*/
static NVIC_InitTypeDef NVIC_InitStructure;
static EXTI_InitTypeDef  EXTI_InitStructure;
static GPIO_InitTypeDef GPIO_InitStructure;

void tp_sda_pin_mode(GPIOMode_TypeDef pin_mode)
{
#if TP_PIN_DEF == TP_PIN_DEF_1		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;		//第14根引脚
	GPIO_InitStructure.GPIO_Mode = pin_mode;	//设置输出/输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	//开漏模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置IO的速度为100MHz，频率越高性能越好，频率越低，功耗越低
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//不需要上拉电阻
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
	
#endif 
	
	
#if TP_PIN_DEF == TP_PIN_DEF_2	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		//第7根引脚
	GPIO_InitStructure.GPIO_Mode = pin_mode;	//设置输出/输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	//开漏模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//设置IO的速度为100MHz，频率越高性能越好，频率越低，功耗越低
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//不需要上拉电阻
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
#endif

}
void tp_i2c_start(void)
{
	//保证SDA引脚为输出模式
	tp_sda_pin_mode(GPIO_Mode_OUT);
	
	TP_SCL_W=1;
	TP_SDA_W=1;
	delay_us(1);
	
	TP_SDA_W=0;
	delay_us(1);

	TP_SCL_W=0;
	delay_us(1);
}


void tp_i2c_stop(void)
{
	//保证SDA引脚为输出模式
	tp_sda_pin_mode(GPIO_Mode_OUT);
	
	TP_SCL_W=1;
	TP_SDA_W=0;
	delay_us(1);
	
	TP_SDA_W=1;
	delay_us(1);
}

void tp_i2c_send_byte(uint8_t byte)
{
	int32_t i;
	
	//保证SDA引脚为输出模式
	tp_sda_pin_mode(GPIO_Mode_OUT);
	
	TP_SCL_W=0;
	TP_SDA_W=0;
	delay_us(1);
	
	for(i=7; i>=0; i--)
	{
		if(byte & (1<<i))
			TP_SDA_W=1;
		else
			TP_SDA_W=0;
	
		delay_us(1);
	
		TP_SCL_W=1;
		delay_us(1);
		
		TP_SCL_W=0;
		delay_us(1);		
	}
}

void tp_i2c_ack(uint8_t ack)
{
	//保证SDA引脚为输出模式
	tp_sda_pin_mode(GPIO_Mode_OUT);
	
	TP_SCL_W=0;
	TP_SDA_W=0;
	delay_us(1);
	
	if(ack)
		TP_SDA_W=1;
	else
		TP_SDA_W=0;

	delay_us(1);

	TP_SCL_W=1;
	delay_us(1);
	
	TP_SCL_W=0;
	delay_us(1);		
}

uint8_t tp_i2c_wait_ack(void)
{
	uint8_t ack;
	//保证SDA引脚为输入模式
	tp_sda_pin_mode(GPIO_Mode_IN);

	//紧接着第九个时钟周期，将SCL拉高
	TP_SCL_W=1;
	delay_us(1);
	
	if(TP_SDA_R)
		ack=1;
	else
		ack=0;
	
	//继续保持占用总线
	TP_SCL_W=0;
	delay_us(1);

	return ack;
}

uint8_t tp_i2c_recv_byte(void)
{

	uint8_t d=0;
	int32_t i;
	
	
	//保证SDA引脚为输入模式
	tp_sda_pin_mode(GPIO_Mode_IN);

	
	for(i=7; i>=0; i--)
	{
		TP_SCL_W=1;
		delay_us(1);
		
		if(TP_SDA_R)
			d|=1<<i;
		
		//继续保持占用总线
		TP_SCL_W=0;
		delay_us(1);	
	}

	return d;
}




void tp_lowlevel_init(void)
{
	
#if TP_PIN_DEF == TP_PIN_DEF_1	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_14;		//第0 4 14根引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				//设置输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;				//开漏模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//设置IO的速度为100MHz，频率越高性能越好，频率越低，功耗越低
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//不需要上拉电阻
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	//只要是输出模式，有初始电平状态
	TP_SCL_W=1;
	TP_SDA_W=1;	
	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOF , &GPIO_InitStructure);
	
	
	//初始化
	TP_RST=1;
	
#endif


#if TP_PIN_DEF == TP_PIN_DEF_2

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;		//第6 7根引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				//设置输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;				//开漏模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//设置IO的速度为100MHz，频率越高性能越好，频率越低，功耗越低
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//不需要上拉电阻
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	//只要是输出模式，有初始电平状态
	TP_SCL_W=1;
	TP_SDA_W=1;	

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOC , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOC , &GPIO_InitStructure);
	
	
	//初始化
	TP_RST=1;
	
#endif
}

void tp_send_byte(uint8_t addr,uint8_t* data)
{
	tp_i2c_start();
	tp_i2c_send_byte(0x2A);tp_i2c_wait_ack();
	tp_i2c_send_byte(addr);tp_i2c_wait_ack();
	tp_i2c_send_byte(*data);tp_i2c_wait_ack();
	tp_i2c_stop();
}

void tp_recv_byte(uint8_t addr,uint8_t* data)
{
	tp_i2c_start();
	tp_i2c_send_byte(0x2A);tp_i2c_wait_ack();
	tp_i2c_send_byte(addr);tp_i2c_wait_ack();
	tp_i2c_start();
	tp_i2c_send_byte(0x2B);tp_i2c_wait_ack();
	*data=tp_i2c_recv_byte();
	tp_i2c_ack(1);
	tp_i2c_stop();
}

void tp_reset(void)
{
	TP_RST=0;
	delay_ms(10);
	
	TP_RST=1;
	delay_ms(60);
}

void tp_init(void)
{
	uint8_t Data=0;
	uint8_t ChipID=0;
	uint8_t FwVersion=0;
	

	tp_lowlevel_init();
	
	tp_reset();//芯片上电初始化
	//tp_recv_byte(0xa7,&ChipID);tp_i2c_wait_ack();
	//tp_recv_byte(0xa9,&FwVersion);tp_i2c_wait_ack();
	
	//printf("ChipID:%d\r\n",ChipID);
	//printf("FwVersion:%d\r\n",FwVersion);
	
	tp_reset();
	Data=1|1<<4;	//看寄存器手册配置单触发模式
	
	tp_send_byte(0xfa,&Data); 
}


uint8_t tp_read(uint16_t *screen_x,uint16_t *screen_y)
{
	uint8_t buf[5];
	uint16_t x=0,y=0,tmp;
	uint8_t action;
	
	tp_recv_byte(0x01,buf);
	tp_recv_byte(0x03,buf+1);
	tp_recv_byte(0x04,buf+2);
	tp_recv_byte(0x05,buf+3);
	tp_recv_byte(0x06,buf+4);
	
	//State=data[0];
	x=(uint16_t)((buf[1]&0x0F)<<8)|buf[2];
	y=(uint16_t)((buf[3]&0x0F)<<8)|buf[4];
	
	if((x<g_lcd_width) && (y<g_lcd_height))
	{
		
		if(lcd_get_direction()==1)
		{
		   tmp= x;
			x = y;
			y = g_lcd_height-tmp;		
		}			
		
		if(lcd_get_direction()==2)
		{
			x = g_lcd_width-x;
			y = g_lcd_height-y;		
		}	

		if(lcd_get_direction()==3)
		{
		   tmp= y;
			y = x;
			x = g_lcd_width-tmp;					
		}			
		
		*screen_x=x;
		*screen_y=y;
		
		/*
			0x00：无手势
			0x01：下滑
			0x02：上滑
			0x03：左滑
			0x04：右滑
			0x05：单击
			0x0B：双击
			0x0C：长按		
		*/
		tp_recv_byte(0x01,&action);

		return action;
	}  	
	
	return 0;
}



