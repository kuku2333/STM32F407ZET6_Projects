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
	//���͸�λ����ǰ��DQ�����
	GPIO_Mode_Out();
	
	uint16_t count = 0;
	uint8_t ack = 0;
	
	/*	��ʼ���͸�λ����,����ͣ�480-960us*/
	DS18B20_DQ_RESET;
	Delay_us(600);
	
	/*	��λ���巢����������������ģʽ���ͷ�����*/
	DS18B20_DQ_SET;
//	GPIO_In_Init(RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_6, ENABLE);
	
	/*	DS18B20��⵽�����أ��ڵȴ�15-60us�󣬴ӻ���������60-240us,
		�Ա�ʾ��������*/
	Delay_us(60);
	
	GPIO_Mode_In();
	/*	�ȴ��͵�ƽ�ĳ���*/
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
	
	/*	�����͵�ƽ���ֵ�ʱ��*/
	while(!DS18B20_DQ_In && count < 300)
	{
		count++;
		Delay_us(1);
	}
	
	/*	���������ʹ���ӻ�����*/
	if(count >= 60 && count <= 240)
	{
		ack = 1;
	}
	
	return ack;
}

void DS18B20_Master_Write_Slot(uint8_t value)
{	
	/*	����д��ģʽ���*/
	GPIO_Mode_Out();
	DS18B20_DQ_SET;
	Delay_us(3);
	
	/*	ע������дʱ϶֮��������1us�Ļָ�ʱ��*/
	if(value)
	{
		/*	����д1��Ҫ���������ߣ�ʱ��Ϊ15us�ڣ�Ȼ���ͷ����ߣ�
			�ٸĳ�����ģʽ��������������������ߣ�*/
//		Delay_us(3);
		
		DS18B20_DQ_RESET;
		Delay_us(3);
		DS18B20_DQ_SET;//�����ͷ�����
		Delay_us(3);
		GPIO_Mode_In();
		
		Delay_us(60);//�ȴ��ӻ�����
//		printf("w:1\r\n");
	}
	else
	{
		/*	����д0��60-120us��*/
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
	/*	Ҫ������ʱ϶����Ҫ����������������1us��Ȼ���ͷ����ߣ�
		ͨ�������ķ�ʽ���ߴӻ�������Ҫ���ˣ�������ʱ϶��Ҫ������
		1us�Ļָ�ʱ��*/
	
	GPIO_Mode_Out();
	DS18B20_DQ_SET;
	Delay_us(2);
	
	uint8_t value = 0;
//	DS18B20_DQ_SET;
	/*	������ʱ϶*/
	GPIO_Mode_Out();
	DS18B20_DQ_RESET;//����
	Delay_us(3);
	
	DS18B20_DQ_SET;//�����ͷ�����
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
