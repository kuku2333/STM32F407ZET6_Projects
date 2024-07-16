#include "W25Q128.h"

void W25Q128_Config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
    
  
    /*!< Enable the SPI clock */
    //sFLASH_SPI_CLK_INIT(sFLASH_SPI_CLK, ENABLE);
  
    /*!< Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    /*!< SPI pins configuration *************************************************/
  
    /*!< Connect SPI pins to AF5 */  
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
          
    /*!< SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    /*!< SPI MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    /*!< SPI MISO pin configuration */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    /*!< Configure sFLASH Card CS pin in output pushpull mode ********************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

      
    /*!< Deselect the FLASH: Chip Select high */
    W25Q128_CS_STATUS(1);
  
    /*!< SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    //SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
  
    /*!< Enable the sFLASH_SPI  */
    SPI_Cmd(SPI1, ENABLE);
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
	/*!< Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  
    /*!< Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(SPI1, byte);
  
    /*!< Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  
    /*!< Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);
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
