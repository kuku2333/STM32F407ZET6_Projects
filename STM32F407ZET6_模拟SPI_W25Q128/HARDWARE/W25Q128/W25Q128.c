#include "W25Q128.h"

/***************************************************************************
	PB3-->CLK,PB4-->MISO,PB5-->MOSI,PB14-->CS
	ģʽ0
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
		//��ǰ���ú�MOSI���ŵ�ƽ
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
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
	W25Q128_CS_STATUS(0);	

	/* ����ָ��0x04 */
	W25Q128_Send_Byte(0x04);	
	
	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
	W25Q128_CS_STATUS(1);
}

uint8_t W25Q128_Read_Status1(void)
{
	uint8_t sta=0;
	
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
	W25Q128_CS_STATUS(0);
	
	/* ����ָ��0x05 */
	W25Q128_Send_Byte(0x05);	
	
	/* ��ȡ״̬�Ĵ���1������ */
	sta=W25Q128_Send_Byte(0xFF);	

	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
	W25Q128_CS_STATUS(1);
	
	return sta;
}

void W25Q128_Erase_Sector(uint32_t sector_addr)
{
	uint8_t sta;
	
	W25Q128_Write_Enable();
	
	
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
	W25Q128_CS_STATUS(0);

	/* ����ָ��0x20 */
	W25Q128_Send_Byte(0x20);	
	
	
	/* ����24bit��ַ */
	W25Q128_Send_Byte((sector_addr>>16)&0xFF);
	W25Q128_Send_Byte((sector_addr>>8)&0xFF);
	W25Q128_Send_Byte(sector_addr & 0xFF);	
	
	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
	W25Q128_CS_STATUS(1);

	
	//����״̬�Ĵ���1����⵱ǰ�Ƿ�������
	while(1)
	{
		sta=W25Q128_Read_Status1();
		//���BUSYλ�Ƿ���1->0
		if((sta & 0x01) == 0)
			break;
		
		Delay_ms(1);
	}

	//��spi flash ����
	W25Q128_Write_Disable();
}

void W25Q128_Read(uint32_t addr, uint8_t *buf, uint32_t len)
{
	uint8_t *p=buf;
	
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
	W25Q128_CS_STATUS(0);

	/* ����ָ��0x03 */
	W25Q128_Send_Byte(0x03);

	/* ����24bit��ַ */
	W25Q128_Send_Byte((addr>>16)&0xFF);
	W25Q128_Send_Byte((addr>>8) &0xFF);
	W25Q128_Send_Byte( addr&0xFF);


	/* ��ȡ���� */
	while(len--)
	{
		*p=W25Q128_Send_Byte(0x10);
		
		p++;
	}
	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
	W25Q128_CS_STATUS(1);
}

void W25Q128_Write(uint32_t addr, uint8_t *msg)
{
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
	W25Q128_CS_STATUS(0);
	
	// ����дʹ������
    W25Q128_Send_Byte(0x06);
	
	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
    W25Q128_CS_STATUS(1);
	
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
    W25Q128_CS_STATUS(0);
	
	/* ����ָ��0x02 */
	W25Q128_Send_Byte(0x02);
	
	/* ����24bit��ַ */
	W25Q128_Send_Byte((addr>>16)&0xFF);
	W25Q128_Send_Byte((addr>>8) &0xFF);
	W25Q128_Send_Byte( addr&0xFF);
	
	/* д����*/
	for(uint8_t i = 0; i < 64; i++)
	{
		W25Q128_Send_Byte(*(msg+i));	
	}
	
	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
	W25Q128_CS_STATUS(1);
	
	/* Ƭѡ���ŵ�ƽ���ͣ���ʼͨ�� */
	W25Q128_CS_STATUS(0);
	
	// ����дʹ������
    W25Q128_Send_Byte(0x04);
	
	/* Ƭѡ���ŵ�ƽ���ߣ�����ͨ�� */
    W25Q128_CS_STATUS(1);
}
