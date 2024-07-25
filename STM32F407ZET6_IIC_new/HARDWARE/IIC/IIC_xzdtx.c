/**
   ******************************************************************************
   * @brief		
   ******************************************************************************
   *	
   ******************************************************************************
**/

#include "IIC_xzdtx.h"

/*	初始化时钟线和数据线*/
void IIC_Init(I2C_Port_t *port)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* 使能硬件时钟:端口硬件时钟 */
	RCC_AHB1PeriphClockCmd(port->SDA_RCC_AHBPeriph, ENABLE);
	RCC_AHB1PeriphClockCmd(port->SCL_RCC_AHBPeriph, ENABLE);	
	
	/* PB8 PB9初始化为开漏输出模式 */
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= 	port->SDA_Pin;
	GPIO_Init(port->SDA_GPIOx, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= 	port->SCL_Pin;
	GPIO_Init(port->SCL_GPIOx, &GPIO_InitStructure);
	
	/*	IIC协议，初始时两根线是高电平*/
//	SDA_W(Bit_SET);
//	SCL_W(Bit_SET);
	GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_SET);
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_SET);
}

/*	数据线需要收和发，所以需要看情况更改引脚模式*/
void SDA_Pin_Mode(I2C_Port_t *port, GPIOMode_TypeDef pin_mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode 	= pin_mode;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin 	= port->SDA_Pin;
	GPIO_Init(port->SDA_GPIOx, &GPIO_InitStructure);	
}

/* 起始位*/
void IIC_Start(I2C_Port_t *port)
{
	/*	开始是主机发给从机，所以是输出模式*/
	SDA_Pin_Mode(port, GPIO_Mode_OUT);
	
	/*	手册描述：当SCL为高电平时SDA的下降沿（高到低）
		叫做起始条件。
		所以，先把两根线都置高，再让数据线置低，这时时
		钟线还是高，就是在告诉从机开始了
	*/
//	IIC_Digital_WRITE(Bit_SET);//数据线高,为了确保SDA高
//	SDA_W(Bit_SET);//数据线高,为了确保SDA高
	GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_SET);
//	SCL_W(Bit_SET);//时钟线高
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_SET);
	Delay_us(DELAY_TIME);//用5us好
	
//	SDA_W(Bit_RESET);//数据线低，时钟线高，起始位生成
	GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_RESET);
	Delay_us(DELAY_TIME);
	
	/* 时钟线低，代表数据不稳定，不能进行数据的读取
		也就是进入数据准备阶段*/
//	SCL_W(Bit_RESET);//
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
	Delay_us(DELAY_TIME);
}

void IIC_Stop(I2C_Port_t *port)
{
	SDA_Pin_Mode(port, GPIO_Mode_OUT);//确保SDA引脚为输出模式
	
	/*
		手册描述：当SCL为高电平时SDA的上升沿（低到高）
		叫做起始条件。
	*/
	
	/* 1.先都拉低
	
		按停止位的时序来看，时钟线也可以拉高，然后在下一
		步中只拉高数据线就行了，但这样做有可能产生错误的
		脉冲，因为数据是在时钟线为高时进行收发的，所以先
		拉低好。而起始位的生成前不需要考虑这些，因为还没有
		产生数据的收发。
	*/
//	SDA_W(Bit_RESET);//数据线低
//	SCL_W(Bit_RESET);//时钟线低
	GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_RESET);
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
	Delay_us(DELAY_TIME);
	
	/* 2.再一起拉高*/
//	SCL_W(Bit_SET);//时钟线高
//	SDA_W(Bit_SET);//数据线高
	GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_SET);
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_SET);
	Delay_us(DELAY_TIME);
	
	/*通信结束了，不需要再对时钟线置低来表示进入数据准备阶段*/
}

/*	
	主机接收从机的应答，IIC的应答机制是：
	低电平代表应答，高电平代表没应答。
*/
uint8_t IIC_Wait_Slave_Ack(I2C_Port_t *port)
{
	/*
		手册描述：总线上的接收器每收到一个字节就产生
		一个应答，主器件必须产生一个对应的额外的时钟
		脉冲。
	
		意思是说，在接收一个字节后，时钟线要设置一个脉冲，
		就是拉高时钟线，然后读数据线，判断从机是否应答，
		低电平就是有应答，高电平就是没有应答。
	*/
	
	uint8_t error_time = 0;
	
	/* 确保SDA引脚为输入模式，以接收从机的应答信号。*/
//	SDA_Pin_Mode(GPIO_Mode_IN);
	SDA_Pin_Mode(port, GPIO_Mode_IN);
	
//	SCL_W(Bit_RESET);//时钟线低，给从机一点准备数据的时间
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
	Delay_us(DELAY_TIME);
	
//	SCL_W(Bit_SET);//时钟线高
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_SET);
	Delay_us(DELAY_TIME);
	
	while(GPIO_ReadInputDataBit(port->SDA_GPIOx, port->SDA_Pin))
	{
		error_time++;
		if(error_time > 250)
		{
			IIC_Stop(port);
			return 1;
		}
	}
	
//	//读取SDA引脚的电平
//	if(SDA_R)
//		ack=1;//无应答
//	else
//		ack=0;//有应答
	
	//拉低时钟线，表示不能进行数据的收发。
//	SCL_W(Bit_RESET);
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
	Delay_us(DELAY_TIME);	
		
	return 0;
}

/* 主机发送应答给从机*/
void IIC_Send_Ack(I2C_Port_t *port, uint8_t ack)
{
	/*	
		先拉低时钟线，表示数据不可读写，再进入输出模式，
		然后发送信号，最后拉低时钟线。
	*/
	
//	SCL_W(Bit_RESET);
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
	
//	SDA_Pin_Mode(GPIO_Mode_OUT);//确保SDA引脚为输出模式
	SDA_Pin_Mode(port, GPIO_Mode_OUT);
	
	/* 准备发送的信号*/
	if(ack)
	{
//		SDA_W(Bit_SET);
		GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_SET);
	}
	else
	{
//		SDA_W(Bit_RESET);
		GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_RESET);
	}
	Delay_us(DELAY_TIME);
	
	/* 时钟线拉高，从机得到信号*/
//	SCL_W(Bit_SET);
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_SET);
	Delay_us(DELAY_TIME);
	
	/* 时钟线拉低，进入数据交换阶段，也就是表示数据不可读写。*/
//	SCL_W(Bit_RESET);//这是为了释放时钟线，准备下一次数据传输
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
	Delay_us(DELAY_TIME);	
}

/* 
	主机发送一个字节的数据给从机，先发高位，再发低位。
*/
void IIC_Send_Byte(I2C_Port_t *port, uint8_t byte)
{	
//	SDA_Pin_Mode(GPIO_Mode_OUT);//确保SDA引脚为输出模式
	SDA_Pin_Mode(port, GPIO_Mode_OUT);
	
//	SCL_W(Bit_RESET);//拉低，进入数据准备阶段。
	GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
	Delay_us(DELAY_TIME);
	
	for(int8_t i = 7; i >= 0; i--)
	{
		/* 准备要发送的bit。*/
		if(byte & (1 << i))//判断对应位是1还是0
//			SDA_W(Bit_SET);
			GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_SET);
		else
//			SDA_W(Bit_RESET);
			GPIO_WriteBit(port->SDA_GPIOx, port->SDA_Pin, Bit_RESET);
		Delay_us(DELAY_TIME);
		
		//拉高时钟线，告诉采集方当前SDA引脚的电平是可靠的，可以读取访问
//		SCL_W(Bit_SET);
		GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_SET);
		Delay_us(DELAY_TIME);
	
		//拉低时钟线，告诉采集方当前SDA引脚的电平是不可靠的，因为当前要变更SDA引脚的电平
//		SCL_W(Bit_RESET);
		GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
		Delay_us(DELAY_TIME);	
	}
}

/* 	
	主机读取从机发的一个字节的数据，IIC每收发一个
	字节的数据，后要一个应答信号，在一连串的数据发送后，
	要有一个非应答，然后是停止位。停止位不在本函数中使用。
	而发送函数中，每发送一个字节也会收到从机的应答信号，所以
	在发送后要判断从机有没有应答。
*/
uint8_t IIC_Read_Byte(I2C_Port_t *port, uint8_t ack)
{
	uint8_t data = 0;
	
//	SDA_Pin_Mode(GPIO_Mode_IN);
	SDA_Pin_Mode(port, GPIO_Mode_IN);
	
//	/* 确保不会有数据的收发，这句没必要，因为起始位最后已经拉低了。*/
//	SCL_W(Bit_RESET);
//	Delay_us(DELAY_TIME);
	
	/* 读一个字节的流程应该是：先拉低时钟线，准备读数据，再
		拉高时钟线进行读数据，最后再选择发送应答信号还是非应答信号。*/
	for(int32_t i = 7; i >= 0; i--)
	{
//		SCL_W(Bit_RESET);
		GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_RESET);
		Delay_us(DELAY_TIME);
		
//		SCL_W(Bit_SET);
		GPIO_WriteBit(port->SCL_GPIOx, port->SCL_Pin, Bit_SET);
		Delay_us(DELAY_TIME);

//		if(SDA_R)
//		{
//			data |= (0x01 << i); 
//		}
		if(GPIO_ReadInputDataBit(port->SDA_GPIOx, port->SDA_Pin))
		{
			data |= (0x01 << i); 
		}
		Delay_us(DELAY_TIME);
	}
	
	/* 1:非应答，0：应答*/
	if(ack)
	{
		IIC_Send_Ack(port, 1);
	}
	else
	{
		IIC_Send_Ack(port, 0);
	}
	
	return data;
}

/*	
	通过设备地址和内部寄存器地址来读数据，并存到data中。
*/
void IIC_Read_One_Byte(I2C_Port_t *port, uint8_t daddr, uint8_t addr, uint8_t *data)
{				  	  	    																 
    IIC_Start(port);  
	
	IIC_Send_Byte(port, daddr);	   //发送写命令
	IIC_Wait_Slave_Ack(port);
	
	IIC_Send_Byte(port, addr);//发送地址
	IIC_Wait_Slave_Ack(port);
	
	IIC_Start(port);  	 	   
	IIC_Send_Byte(port, daddr | 0x01);//进入接收模式			   
	IIC_Wait_Slave_Ack(port);
	
    *data = IIC_Read_Byte(port, 1);
	
    IIC_Stop(port);//产生一个停止条件	    
}

/*
	通过设备地址和内部寄存器地址，将data写进对应地址中。
*/
void IIC_Write_One_Byte(I2C_Port_t *port, uint8_t daddr,uint8_t addr,uint8_t data)
{				   	  	    																 
    IIC_Start(port);  
	
	IIC_Send_Byte(port, daddr);	    //发送写命令
	IIC_Wait_Slave_Ack(port);
	
	IIC_Send_Byte(port, addr);//发送地址
	IIC_Wait_Slave_Ack(port);	
	
	IIC_Send_Byte(port, data);     //发送字节							   
	IIC_Wait_Slave_Ack(port);  
	
    IIC_Stop(port);//产生一个停止条件 
	Delay_ms(10);	 
}

/*
	连续地写多个字节的数据。
*/
void IIC_Write_Bytes(I2C_Port_t *port, uint8_t write_addr, uint8_t *data, uint8_t data_Length)
{		
    IIC_Start(port);  

	IIC_Send_Byte(port, write_addr);	    //发送写命令
	IIC_Wait_Slave_Ack(port);
	
	for(uint8_t i = 0; i < data_Length; i++)
	{
		IIC_Send_Byte(port, data[i]);
		IIC_Wait_Slave_Ack(port);
	}			
	
    IIC_Stop(port);//产生一个停止条件 
	Delay_ms(10);	 
}

/*
	连续地读多个字节的数据。
*/
void IIC_Read_Bytes(I2C_Port_t *port, uint8_t device_addr, uint8_t write_addr, uint8_t *data, uint8_t data_length)
{		
    IIC_Start(port);  

	IIC_Send_Byte(port, device_addr);	    //发送写命令
	IIC_Wait_Slave_Ack(port);
	IIC_Send_Byte(port, write_addr);
	IIC_Wait_Slave_Ack(port);
	IIC_Send_Byte(port, device_addr | 0X01);//进入接收模式			   
	IIC_Wait_Slave_Ack(port);
	
	for(uint8_t i = 0;i < data_length - 1; i++)
	{
		data[i] = IIC_Read_Byte(port, 0);
	}		
	data[data_length - 1] = IIC_Read_Byte(port, 1);	

    IIC_Stop(port);//产生一个停止条件 
	Delay_ms(10);	 
}
