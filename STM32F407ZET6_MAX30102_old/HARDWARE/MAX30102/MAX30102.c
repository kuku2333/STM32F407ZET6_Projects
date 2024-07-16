#include "MAX30102.h"

uint8_t MAX30102_Bus_Write(uint8_t register_address, uint8_t word_data)
{

	/* 采用串行EEPROM随即读取指令序列，连续读取若干字节 */

	/* 第1步：发起I2C总线启动信号 */
	IIC_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(MAX30102_WR_ADDRESS | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (IIC_Wait_Slave_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址 */
	IIC_Send_Byte(register_address);
	if (IIC_Wait_Slave_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	/* 第5步：开始写入数据 */
	IIC_Send_Byte(word_data);

	/* 第6步：发送ACK */
	if (IIC_Wait_Slave_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 发送I2C总线停止信号 */
	IIC_Stop();
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	IIC_Stop();
	return 0;
}

uint8_t MAX30102_Bus_Read(uint8_t register_address)
{
	uint8_t data;

	/* 第1步：发起I2C总线启动信号 */
	IIC_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(MAX30102_WR_ADDRESS | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (IIC_Wait_Slave_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址， */
	IIC_Send_Byte((uint8_t)register_address);
	if (IIC_Wait_Slave_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	IIC_Start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(MAX30102_WR_ADDRESS | I2C_RD);	/* 此处是读指令 */

	/* 第8步：发送ACK */
	if (IIC_Wait_Slave_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第9步：读取数据 */
	{
		data = IIC_Recv_Byte();	/* 读1个字节 */

		IIC_Send_Ack(1);	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
	}
	/* 发送I2C总线停止信号 */
	IIC_Stop();
	return data;	/* 执行成功 返回data值 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	IIC_Stop();
	return 0;
}


void MAX30102_FIFO_Read_Words(uint8_t register_address,uint16_t word_data[][2],uint8_t count)
{
	u8 i=0;
	u8 no = count;
	u8 data1, data2;
	/* 第1步：发起I2C总线启动信号 */
	IIC_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(MAX30102_WR_ADDRESS | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (IIC_Wait_Slave_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址， */
	IIC_Send_Byte((uint8_t)register_address);
	if (IIC_Wait_Slave_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	

	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	IIC_Start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(MAX30102_WR_ADDRESS | I2C_RD);	/* 此处是读指令 */

	/* 第8步：发送ACK */
	if (IIC_Wait_Slave_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第9步：读取数据 */
	while (no)
	{
		data1 = IIC_Recv_Byte();	
		IIC_Send_Ack(0);
		data2 = IIC_Recv_Byte();
		IIC_Send_Ack(0);
		word_data[i][0] = (((u16)data1 << 8) | data2);  //

		
		data1 = IIC_Recv_Byte();	
		IIC_Send_Ack(0);
		data2 = IIC_Recv_Byte();
		if(1==no)
			IIC_Send_Ack(1);	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		else
			IIC_Send_Ack(0);
		word_data[i][1] = (((u16)data1 << 8) | data2); 

		no--;	
		i++;
	}
	/* 发送I2C总线停止信号 */
	IIC_Stop();

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	IIC_Stop();
}

void MAX30102_FIFO_Read_Bytes(uint8_t register_address, uint8_t *data)
{	
	MAX30102_Bus_Read(REG_INTR_STATUS_1);
	MAX30102_Bus_Read(REG_INTR_STATUS_2);
	
	/* 第1步：发起I2C总线启动信号 */
	IIC_Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(MAX30102_WR_ADDRESS | I2C_WR);	/* 此处是写指令 */

	/* 第3步：发送ACK */
	if (IIC_Wait_Slave_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址， */
	IIC_Send_Byte((uint8_t)register_address);
	if (IIC_Wait_Slave_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	

	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	IIC_Start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	IIC_Send_Byte(MAX30102_WR_ADDRESS | I2C_RD);	/* 此处是读指令 */

	/* 第8步：发送ACK */
	if (IIC_Wait_Slave_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第9步：读取数据 */
	data[0] = IIC_Recv_Byte();IIC_Send_Ack(0);		
	data[1] = IIC_Recv_Byte();IIC_Send_Ack(0);	
	data[2] = IIC_Recv_Byte();IIC_Send_Ack(0);	
	data[3] = IIC_Recv_Byte();IIC_Send_Ack(0);	
	data[4] = IIC_Recv_Byte();IIC_Send_Ack(0);	
	data[5] = IIC_Recv_Byte();IIC_Send_Ack(1);	
	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
	/* 发送I2C总线停止信号 */
	IIC_Stop();

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	IIC_Stop();
}

void MAX30102_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//端口E硬件时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//第6引脚Camera8
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;			//复用功能模式
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;		//引脚高速工作，收到指令立即工作；缺点：功耗高
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		//增加输出电流的能力
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;		//不需要上下拉电阻
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	IIC_Init();
	
	MAX30102_Reset();	
	
	MAX30102_Bus_Write(REG_INTR_ENABLE_1,0xc0);	// INTR setting
	MAX30102_Bus_Write(REG_INTR_ENABLE_2,0x00);
	MAX30102_Bus_Write(REG_FIFO_WR_PTR,0x00);  	//FIFO_WR_PTR[4:0]
	MAX30102_Bus_Write(REG_OVF_COUNTER,0x00);  	//OVF_COUNTER[4:0]
	MAX30102_Bus_Write(REG_FIFO_RD_PTR,0x00);  	//FIFO_RD_PTR[4:0]
	MAX30102_Bus_Write(REG_FIFO_CONFIG,0x0f);  	//sample avg = 1, fifo rollover=false, fifo almost full = 17
	MAX30102_Bus_Write(REG_MODE_CONFIG,0x03);  	//0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
	MAX30102_Bus_Write(REG_SPO2_CONFIG,0x27);  	// SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)  
	MAX30102_Bus_Write(REG_LED1_PA,0x24);   	//Choose value for ~ 7mA for LED1
	MAX30102_Bus_Write(REG_LED2_PA,0x24);   	// Choose value for ~ 7mA for LED2
	MAX30102_Bus_Write(REG_PILOT_PA,0x7f);   	// Choose value for ~ 25mA for Pilot LED									
}

void MAX30102_Reset(void)
{
	MAX30102_Bus_Write(REG_MODE_CONFIG,0x40);//REG_MODE_CONFIG 0x09
	MAX30102_Bus_Write(REG_MODE_CONFIG,0x40);
}

void Maxim_MAX30102_Write_REG(uint8_t uch_addr, uint8_t uch_data)
{
	IIC_Write_One_Byte(I2C_WRITE_ADDR, uch_addr, uch_data);
}

void Maxim_MAX30102_Read_REG(uint8_t uch_addr, uint8_t *puch_data)
{
	IIC_Read_One_Byte(I2C_WRITE_ADDR, uch_addr, puch_data);
}

void Maxim_MAX30102_Read_FIFO(uint32_t *pun_red_led, uint32_t *pun_ir_led)
{
	uint32_t un_temp;
	unsigned char uch_temp;
	char ach_i2c_data[6];
	*pun_red_led = 0;
	*pun_ir_led  = 0;

  Maxim_MAX30102_Read_REG(REG_INTR_STATUS_1, &uch_temp);
  Maxim_MAX30102_Read_REG(REG_INTR_STATUS_2, &uch_temp);
  
  IIC_Read_Bytes(I2C_WRITE_ADDR, REG_FIFO_DATA, (uint8_t *)ach_i2c_data,6);
  
  un_temp   =	(unsigned char) ach_i2c_data[0];
  un_temp <<=	16;
  *pun_red_led +=	un_temp;
  un_temp	=	(unsigned char) ach_i2c_data[1];
  un_temp	<<=	8;
  *pun_red_led += un_temp;
  un_temp = (unsigned char) ach_i2c_data[2];
  *pun_red_led += un_temp;
  
  un_temp = (unsigned char) ach_i2c_data[3];
  un_temp <<= 16;
  *pun_ir_led += un_temp;
  un_temp = (unsigned char) ach_i2c_data[4];
  un_temp <<= 8;
  *pun_ir_led += un_temp;
  un_temp = (unsigned char) ach_i2c_data[5];
  *pun_ir_led += un_temp;
  *pun_red_led &= 0x03FFFF;  //Mask MSB [23:18]
  *pun_ir_led &= 0x03FFFF;  //Mask MSB [23:18]
}
