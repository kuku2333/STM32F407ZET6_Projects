/**
   ******************************************************************************
   * @brief		用两个IO口来模拟IIC
   ******************************************************************************
   *	1.PB8->时钟线，PB9->数据线
   *	2.需根据AT24C02的手册来看如何编写,具体是看
   *	对应的时序图
   *	3.IIC通信时，需要几个信号，可分别将其封装成函数
   *	有：起始条件、停止条件、主机应答、从机应答
   ******************************************************************************
**/

#include "AT24C02.h"

/*	初始化时钟线和数据线*/
void AT24C02_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* 使能硬件时钟:端口硬件时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	
	/* PB8 PB9初始化为开漏输出模式 */
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	/*	先都拉低，为后续的操作给一个初始状态*/
	SDA_W(Bit_SET);
	SCL_W(Bit_SET);
}

/*	数据线需要读和写，所以需要看情况更改引脚模式*/
void SDA_Pin_Mode(GPIOMode_TypeDef pin_mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode 	= pin_mode;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void IIC_Start(void)
{
	/*	读和写的前期准备是主机发给从机，所以是输出*/
	SDA_Pin_Mode(GPIO_Mode_OUT);
	
	/*	手册描述：当SCL为高电平时SDA的下降沿（高到低）
		叫做起始条件。
		所以，先把两根线都置高，表示空闲状态，再让数据
		线置低，这时时钟线还是高，就是在告诉从机开始了，
		最后再做一个时钟线置低操作。
	*/
	SDA_W(Bit_SET);//数据线高
	SCL_W(Bit_SET);//时钟线高
	Delay_us(5);
	
	SDA_W(Bit_RESET);//数据线低
	Delay_us(5);
	
	SCL_W(Bit_RESET);//时钟线低
	Delay_us(5);
}

void IIC_Stop(void)
{
	SDA_Pin_Mode(GPIO_Mode_OUT);//确保SDA引脚为输出模式
	
	/*
		手册描述：当SCL为高电平时SDA的上山沿（低到高）
		叫做起始条件。
	*/
	SDA_W(Bit_RESET);//数据线低
	SCL_W(Bit_SET);//时钟线高
	Delay_us(5);
	
	SDA_W(Bit_SET);//数据线高
	Delay_us(5);
	
	//通信结束了，不需要再对时钟线置低
}

uint8_t IIC_Wait_Slave_Ack(void)
{
	uint8_t ack;
	SDA_Pin_Mode(GPIO_Mode_IN);//确保SDA引脚为输入模式
	
	/*
		手册描述：总线上的接收器每收到一个字节就产生
		一个应答，主器件必须产生一个对应的额外的时钟
		脉冲
		1.意思是说，在接收一个字节后，时钟线要设置一个脉冲，
		直接拉高时钟线，然后读数据线，判断从机是否应答，
		低电平就是有应答，高电平就是没有应答
		2.拉高时钟线，告诉采集方当前SDA引脚的电平是可靠的，
		可以读取访问
	*/
	SCL_W(Bit_SET);
	Delay_us(5);
	
	//读取SDA引脚的电平
	if(SDA_R)
		ack=1;//无应答
	else
		ack=0;//有应答
	
	//拉低时钟线
	SCL_W(Bit_RESET);
	Delay_us(5);	
		
	return ack;
}

void IIC_SendAck(uint8_t ack)
{
	SDA_Pin_Mode(GPIO_Mode_OUT);//确保SDA引脚为输出模式

	/*	
		在发送应答前，时钟线高低都可以正常发送应答，
		但根据时序图，置高合理点，数据线必须先置低，
		然后发送应答
	*/
	SCL_W(Bit_SET);/*这是为了在发送应答信号之前暂停时钟
					数据交换只允许发生在时钟线为低的时候*/
	SDA_W(Bit_RESET);//这是为了确保在发送应答信号之前，数据线是低电平
	Delay_us(5);
	
	if(ack)
	{
		SDA_W(Bit_SET);
	}
	else
	{
		SDA_W(Bit_RESET);
	}
	
	Delay_us(5);
	
	SCL_W(Bit_SET);//这是为了结束应答信号的发送。
	Delay_us(5);
	SCL_W(Bit_RESET);//这是为了释放时钟线，准备下一次数据传输
	Delay_us(5);	
}

void IIC_Send_Byte(uint8_t byte)
{	
	SDA_Pin_Mode(GPIO_Mode_OUT);//确保SDA引脚为输出模式
	
	SCL_W(Bit_RESET);//拉低，允许数据交换
	Delay_us(5);
	
	for(int32_t i = 7; i >= 0; i--)
	{
		//优先去设置SDA引脚的电平
		if(byte & (1 << i))
			SDA_W(Bit_SET);
		else
			SDA_W(Bit_RESET);
	
		Delay_us(5);
		
		//拉高时钟线，告诉采集方当前SDA引脚的电平是可靠的，可以读取访问
		SCL_W(Bit_SET);
		Delay_us(5);
	
		//拉低时钟线，告诉采集方当前SDA引脚的电平是不可靠的，因为当前要变更SDA引脚的电平
		SCL_W(Bit_RESET);
		Delay_us(5);	
	}
}

uint8_t IIC_Recv_Byte(void)
{
	uint8_t data = 0;
	
	SDA_Pin_Mode(GPIO_Mode_IN);
	
	SCL_W(Bit_RESET);
	Delay_us(5);
	
	for(int32_t i = 7; i >= 0; i--)
	{
		SCL_W(Bit_SET);
		Delay_us(5);

		if(SDA_R)
		{
			data |= (0x01 << i); 
		}
		Delay_us(5);
			
		SCL_W(Bit_RESET);
		Delay_us(5);
	}
	
	IIC_Send_Ack(0);
	
	return data;
}

int32_t AT24C02_Write(uint8_t word_addr, uint8_t *buf, uint8_t len)
{
	uint8_t ack;
	uint8_t *p = buf;
	
	//发送开始信号
	IIC_Start();
	
	//开始设备寻址
	IIC_Send_Byte(0xA0);
	
	//等待从机的应答
	ack = IIC_Wait_Slave_Ack();
	if(ack)
	{
		printf("device address fail\r\n");
		
		return -1;
	}
	
	//发送word address，告诉从机往哪个存储地址写入数据
	IIC_Send_Byte(word_addr);
	
	//等待从机的应答
	ack = IIC_Wait_Slave_Ack();
	if(ack)
	{
		printf("word address fail\r\n");
		
		return -2;
	}
	
	//循环发送要写入的字节
	while(len--)
	{
		//发送一个字节
		IIC_Send_Byte(*p);
		
		//等待应答
		ack = IIC_Wait_Slave_Ack();
		if(ack)
		{
			printf("write data fail\r\n");
			
			return -3;
		}
		
		p++;
	}
	
	//发送停止信号
	IIC_Stop();
	
	printf("write success\r\n");
	
	return 0;
}

void AT24C02_Read(uint8_t word_addr, uint8_t *buf, uint8_t len)
{
	uint8_t ack;
	
	IIC_Start();
	
	IIC_Send_Byte(0xA0);
	
	ack = IIC_Wait_Slave_Ack();
	if(ack)
	{
		printf("device address with write fail\r\n");
	}
	
	IIC_Send_Byte(word_addr);
	
	ack = IIC_Wait_Slave_Ack();
	if(ack)
	{
		printf("word address fail\r\n");
	}
	
	IIC_Start();
	
	IIC_Send_Byte(0xA1);
	
	ack = IIC_Wait_Slave_Ack();
	if(ack)
	{
		printf("device address with read fail\r\n");
	}
	
	for(int32_t i = 0; i < len - 1; i++)
	{
		buf[i] = IIC_Recv_Byte();
		printf("buf[%d]:%d\r\n", i, buf[i]);
	}
	
	buf[7] = IIC_Recv_Byte();
	IIC_Send_Ack(1);
	
	IIC_Stop();
}

/*
	调试代码：
	uint8_t buf[8]={1,2,3,4,5,6,7,8};
	uint8_t get_buf[8]={0};
	
	//1.硬件的初始化
	Uasrt1_Init();
	
	//初始化at24c02
	AT24C02_Init();
	
	//往at24c02写入数据
	AT24C02_Write(0,buf,sizeof buf);
	
	Delay_ms(10);
	
	AT24C02_Read(0, get_buf, sizeof get_buf);
	
	for(uint8_t i = 0; i < 8; i++)
	{
		printf("The value of data_%d is: %d\r\n", i, get_buf[i]);
	}
*/
