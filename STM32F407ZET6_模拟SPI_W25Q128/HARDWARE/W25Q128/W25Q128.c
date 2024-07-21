#include "W25Q128.h"

/***************************************************************************
	PB3-->CLK,PB4-->MISO,PB5-->MOSI,PB14-->CS
	模式0
***************************************************************************/
void W25Q128_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_14;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    /*!< SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	PBout(14) = 1;
	PBout(3) = 0;
	PBout(5) = 1;
}

uint16_t W25Q128_Get_ID(void)
{	
	uint8_t  Temp0 = 0, Temp1 = 0;
	uint16_t Temp = 0;
	
	W25Q128_CS_STATUS(0);
	
	W25Q128_Send_Byte(0x90);

	W25Q128_Send_Byte(0x00);
	W25Q128_Send_Byte(0x00);
	W25Q128_Send_Byte(0x00);
	
	/*!< Read a byte from the FLASH */
    Temp0 = W25Q128_Send_Byte(0x00);
  
	/*!< Read a byte from the FLASH */
	Temp1 = W25Q128_Send_Byte(0x00);
  
    /*!< Deselect the FLASH: Chip Select high */
    W25Q128_CS_STATUS(1);
  
    Temp = (Temp0 << 8) | Temp1;
  
    return Temp;
}

uint16_t W25Q128_Send_Byte(uint8_t byte)
{
	int32_t i=0;
	uint8_t d=0;
	
	for(i=7; i>=0; i--)
	{
		//提前设置好MOSI引脚电平
		if(byte & (1<<i))
			PBout(5) = 1;
		else
			PBout(5) = 0;
		
		PBout(3) = 1;
		Delay_us(1);
		
		if(PBin(4))
			d|=1<<i;
		
		PBout(3) = 0;
		Delay_us(1);
	}
	
	return d;
}

void W25Q128_Write_Enable(void)
{
	W25Q128_CS_STATUS(0);
	
	W25Q128_Send_Byte(0x06);
	
	W25Q128_CS_STATUS(1);
}

void W25Q128_Write_Disable(void)
{
	/* 片选引脚电平拉低，开始通信 */
	W25Q128_CS_STATUS(0);	

	/* 发送指令0x04 */
	W25Q128_Send_Byte(0x04);	
	
	/* 片选引脚电平拉高，结束通信 */
	W25Q128_CS_STATUS(1);
}

uint8_t W25Q128_Read_Status1(void)
{
	uint8_t sta=0;
	
	/* 片选引脚电平拉低，开始通信 */
	W25Q128_CS_STATUS(0);
	
	/* 发送指令0x05 */
	W25Q128_Send_Byte(0x05);	
	
	/* 读取状态寄存器1的数据 */
	sta=W25Q128_Send_Byte(0xFF);	

	/* 片选引脚电平拉高，结束通信 */
	W25Q128_CS_STATUS(1);
	
	return sta;
}

void W25Q128_Erase_Sector(uint32_t sector_addr)
{
	uint8_t sta;
	
	W25Q128_Write_Enable();
	
	
	/* 片选引脚电平拉低，开始通信 */
	W25Q128_CS_STATUS(0);

	/* 发送指令0x20 */
	W25Q128_Send_Byte(0x20);	
	
	
	/* 发送24bit地址 */
	W25Q128_Send_Byte((sector_addr>>16)&0xFF);
	W25Q128_Send_Byte((sector_addr>>8)&0xFF);
	W25Q128_Send_Byte(sector_addr & 0xFF);	
	
	/* 片选引脚电平拉高，结束通信 */
	W25Q128_CS_STATUS(1);

	
	//访问状态寄存器1，检测当前是否擦除完毕
	while(1)
	{
		sta=W25Q128_Read_Status1();
		//检测BUSY位是否由1->0
		if((sta & 0x01) == 0)
			break;
		
		Delay_ms(1);
	}

	//对spi flash 上锁
	W25Q128_Write_Disable();
}

void W25Q128_Read(uint32_t addr, uint8_t *buf, uint32_t len)
{
	uint8_t *p=buf;
	
	/* 片选引脚电平拉低，开始通信 */
	W25Q128_CS_STATUS(0);

	/* 发送指令0x03 */
	W25Q128_Send_Byte(0x03);

	/* 发送24bit地址 */
	W25Q128_Send_Byte((addr>>16)&0xFF);
	W25Q128_Send_Byte((addr>>8) &0xFF);
	W25Q128_Send_Byte( addr&0xFF);


	/* 读取数据 */
	while(len--)
	{
		*p=W25Q128_Send_Byte(0x10);
		
		p++;
	}
	/* 片选引脚电平拉高，结束通信 */
	W25Q128_CS_STATUS(1);
}

void W25Q128_Write(uint32_t addr, uint8_t *msg)
{
	/* 片选引脚电平拉低，开始通信 */
	W25Q128_CS_STATUS(0);
	
	// 发送写使能命令
    W25Q128_Send_Byte(0x06);
	
	/* 片选引脚电平拉高，结束通信 */
    W25Q128_CS_STATUS(1);
	
	/* 片选引脚电平拉低，开始通信 */
    W25Q128_CS_STATUS(0);
	
	/* 发送指令0x02 */
	W25Q128_Send_Byte(0x02);
	
	/* 发送24bit地址 */
	W25Q128_Send_Byte((addr>>16)&0xFF);
	W25Q128_Send_Byte((addr>>8) &0xFF);
	W25Q128_Send_Byte( addr&0xFF);
	
	/* 写数据*/
	for(uint8_t i = 0; i < 64; i++)
	{
		W25Q128_Send_Byte(*(msg+i));	
	}
	
	/* 片选引脚电平拉高，结束通信 */
	W25Q128_CS_STATUS(1);
	
	/* 片选引脚电平拉低，开始通信 */
	W25Q128_CS_STATUS(0);
	
	// 发送写使能命令
    W25Q128_Send_Byte(0x04);
	
	/* 片选引脚电平拉高，结束通信 */
    W25Q128_CS_STATUS(1);
}
