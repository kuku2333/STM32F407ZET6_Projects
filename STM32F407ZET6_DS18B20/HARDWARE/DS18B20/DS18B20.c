#include "DS18B20.h"

/**
   ******************************************************************************
   * @brief		
   ******************************************************************************
   * 
   ******************************************************************************
**/

uint8_t DS18B20_Init(void)
{
	//发送复位脉冲前，DQ输出高
	GPIO_Mode_Out();
	
	uint16_t count = 0;
	uint8_t ack = 0;
	
	/*	开始发送复位脉冲,输出低，480-960us*/
	DS18B20_DQ_RESET;
	Delay_us(600);
	
	/*	复位脉冲发送完后，主机进入接收模式，释放总线*/
	DS18B20_DQ_SET;
//	GPIO_In_Init(RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_6, ENABLE);
	
	/*	DS18B20检测到上升沿，在等待15-60us后，从机拉低总线60-240us,
		以表示存在脉冲*/
	Delay_us(60);
	
	GPIO_Mode_In();
	/*	等待低电平的出现*/
	while(DS18B20_DQ_In && count < 200)
	{
		count++;
		Delay_us(1);
	}
	
	if(count >= 200)
	{
		return ack;
	}
	else
	{
		count = 0;
	}
	
	/*	计数低电平出现的时间*/
	while(!DS18B20_DQ_In && count < 300)
	{
		count++;
		Delay_us(1);
	}
	
	/*	满足条件就代表从机存在*/
	if(count >= 60 && count <= 240)
	{
		ack = 1;
	}
	
	return ack;
}

void DS18B20_Master_Write_Slot(uint8_t value)
{	
	/*	主机写，模式输出*/
	GPIO_Mode_Out();
	DS18B20_DQ_SET;
	Delay_us(3);
	
	/*	注：两个写时隙之间至少有1us的恢复时间*/
	if(value)
	{
		/*	主机写1，要先拉低总线，时间为15us内，然后释放总线，
			再改成输入模式，让上拉电阻把总线拉高，*/
//		Delay_us(3);
		
		DS18B20_DQ_RESET;
		Delay_us(3);
		DS18B20_DQ_SET;//主机释放总线
		Delay_us(3);
		GPIO_Mode_In();
		
		Delay_us(60);//等待从机采样
//		printf("w:1\r\n");
	}
	else
	{
		/*	主机写0，60-120us，*/
//		Delay_us(3);
		
		DS18B20_DQ_RESET;
		Delay_us(80);
		
//		DS18B20_DQ_SET;
//		Delay_us(2);
//		printf("w:0\r\n");
	}
}

void DS18B20_Master_Write_Command(uint8_t cmd)
{
	for(int8_t i = 0; i < 8; i++)
	{
		if(cmd & 0x01)
		{
			DS18B20_Master_Write_Slot(1);
//			printf("w:1\r\n");
		}
		else
		{
			DS18B20_Master_Write_Slot(0);
//			printf("w:0\r\n");
		}
		
		cmd >>= 0x01;
	}
}

uint8_t DS18B20_Master_Read_Slot(void)
{
	/*	要产生读时隙，需要主机拉低总线至少1us，然后释放总线，
		通过这样的方式告诉从机，主机要读了，两个读时隙间要有至少
		1us的恢复时间*/
	
	GPIO_Mode_Out();
	DS18B20_DQ_SET;
	Delay_us(2);
	
	uint8_t value = 0;
//	DS18B20_DQ_SET;
	/*	产生读时隙*/
	GPIO_Mode_Out();
	DS18B20_DQ_RESET;//拉低
	Delay_us(3);
	
	DS18B20_DQ_SET;//主机释放总线
//	GPIO_Mode_In();
	
	Delay_us(5);
	GPIO_Mode_In();
	
	if(DS18B20_DQ_In)
	{
		value = 1;
		//printf("r:1\r\n");
	}
	//printf("r:0\r\n");
	
	Delay_us(65);
	
	return value;
}

uint8_t DS18B20_Master_Read_Byte(void)
{	
	uint8_t data = 0;
	
	for(int8_t i = 0; i < 8; i++)
	{
		if(DS18B20_Master_Read_Slot())
		{
			data |= (0x01 << i);
		}
	}
	
	return data;
}

uint8_t DS18B20_Get_Temp(uint8_t MS, uint8_t LS)
{
	uint8_t temp;
	uint8_t MS_temp = MS;
	
	for(uint8_t i = 0; i < 3; i++)
	{
		if(MS_temp & 0x01)
		{
			temp |= (0x01 << i);
		}
		
		MS_temp >>= 0x01;
	}
	
	temp <<= 4;
	
	LS &= 0xF0;
	LS >>= 4;
	
	for(uint8_t i = 0; i < 4; i++)
	{
		if(LS & 0x01)
		{
			temp |= (0x01 << i);
		}
		
		LS >>= 0x01;
	}
	
	
	
//	if(MS < 7)
//	{
//		
//	}
	
	return temp;
}

void test(void)
{
//	uint8_t ack = 0;
//	
//	ack = DS18B20_Init();
//	if(ack == 1)
//	{
//		printf("init ack!\r\n");
//		ack = 0;
//	}
//	
//	DS18B20_Master_Write_Command(0xCC);
//	DS18B20_Master_Write_Command(0x44);
//	
//	ack = DS18B20_Master_Read_Slot();
//	if(ack == 1)
//	{
//		printf("answer ack!\r\n");
//		ack = 0;
//	}
	
	uint8_t data[32] = {0};
	uint8_t ack = 0;
	
	ack = DS18B20_Init();
	if(ack == 1)
	{
		printf("ack!\r\n");
		ack = 0;
	}

	DS18B20_Master_Write_Command(0xCC);
	printf("\r\n");
	DS18B20_Master_Write_Command(0x4E);
	printf("\r\n");
	
	DS18B20_Master_Write_Command(0x16);
	printf("\r\n");
	DS18B20_Master_Write_Command(0x21);
	printf("\r\n");
	DS18B20_Master_Write_Command(0x2C);
	printf("\r\n");
	DS18B20_Master_Write_Command(0x37);
	printf("\r\n");
	DS18B20_Master_Write_Command(0x42);
	printf("\r\n");
	
	ack = DS18B20_Init();
	if(ack == 1)
	{
		printf("ack!\r\n");
		ack = 0;
	}
	
	DS18B20_Master_Write_Command(0xCC);
	printf("\r\n");
	DS18B20_Master_Write_Command(0xBE);
	printf("\r\n");
	
	for(int32_t i = 0; i < 9; i++)
	{
		data[i] = DS18B20_Master_Read_Byte();
		printf("\r\n");
		
		printf("data %d is: %d\r\n", i, data[i]);
	}
	
	uint8_t temp = 0;
	temp = DS18B20_Get_Temp(data[1], data[0]);
	
	printf("The value of temperture is:%d\r\n", temp);
	
//	ack = DS18B20_Init();
//	if(ack == 1)
//	{
//		printf("ack!\r\n");
//		ack = 0;
//	}
//	
//	DS18B20_Master_Write_Command(0xCC);
//	DS18B20_Master_Write_Command(0x48);
}
