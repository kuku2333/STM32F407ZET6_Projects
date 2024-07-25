/**
   ******************************************************************************
   * @file    STM32F407ZET6_IIC_new\HARDWARE\IIC_xzdtx.h 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-23
   * @brief   IICЭ��
   ******************************************************************************
   * 	ͨ�ð�IIC�ײ����
   *	ʹ��ʱ����I2C_Port_t���ͱ������磺
   *	I2C_Port_t port1 = {GPIOB, GPIOB�� 
   *						GPIO_Pin_8, GPIO_Pin_9, 
   *						RCC_AHB1Periph_GPIOB, RCC_AHB1Periph_GPIOB};
   *	Ȼ�󴫲μ��ɡ�
   *	ע�⣬IIC_Wait_Slave_Ack����1�Ƿ�Ӧ��0��Ӧ��
   *	IIC_Send_Ack����Ҳ�ǣ���1��ʾ��Ӧ��0Ӧ��
   *	IIC_Read_Byte��������д����Ӧ����䣬�����䴫1��ʾ��Ӧ��
   *	��0��ʾӦ��
   *	��IIC_Send_Byte����Ҫ������Ҫ���ڵ��ú��жϴӻ��Ƿ�Ӧ��
   ******************************************************************************
**/

#ifndef __IIC_XZDTX_H
#define __IIC_XZDTX_H

#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include <stdio.h>

/*********************
 *	MACRO
 *********************/
 
//#define SDA_W(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_9,(x))		//������д
//#define SDA_R		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) //�����߶�
//#define SCL_W(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_8,(x))		//ʱ����д

#define DELAY_TIME 	5 //��ʱʱ�䣬��λus

/*********************
 *	MACRO
 *********************/

/*********************
 *	GLOBAL VALUE
 *********************/

typedef struct {
    GPIO_TypeDef* SDA_GPIOx;    // GPIO�˿�
	GPIO_TypeDef* SCL_GPIOx;    // GPIO�˿�
    uint16_t SDA_Pin;       // ����������
    uint16_t SCL_Pin;       // ʱ��������
    uint32_t SDA_RCC_AHBPeriph; // ��Ӧ��RCCʱ��
	uint32_t SCL_RCC_AHBPeriph; // ��Ӧ��RCCʱ��
} I2C_Port_t;

/*********************
 *	GLOBAL VALUE
 *********************/

/*********************
 *	FUNCTION
 *********************/
 
void IIC_Read_Bytes(I2C_Port_t *port, uint8_t device_addr, uint8_t write_addr, uint8_t *data, uint8_t data_length);
void IIC_Write_Bytes(I2C_Port_t *port, uint8_t write_addr, uint8_t *data, uint8_t data_Length);
void IIC_Write_One_Byte(I2C_Port_t *port, uint8_t daddr,uint8_t addr,uint8_t data);
void IIC_Read_One_Byte(I2C_Port_t *port, uint8_t daddr, uint8_t addr, uint8_t *data);
uint8_t IIC_Read_Byte(I2C_Port_t *port, uint8_t ack);
void IIC_Send_Byte(I2C_Port_t *port, uint8_t byte);
void IIC_Send_Ack(I2C_Port_t *port, uint8_t ack);
uint8_t IIC_Wait_Slave_Ack(I2C_Port_t *port);
void IIC_Stop(I2C_Port_t *port);
void IIC_Start(I2C_Port_t *port);
void SDA_Pin_Mode(I2C_Port_t *port, GPIOMode_TypeDef pin_mode);
void IIC_Init(I2C_Port_t *port);

 /*********************
 *	FUNCTION
 *********************/

#endif
