/**
   ******************************************************************************
   * @file    STM32F407ZET6_IIC_wen\HARDWARE\AT24C02.h 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-11
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#ifndef __AT24C02_H
#define __AT24C02_H

#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include <stdio.h>

/*********************
 *	MACRO
 *********************/
 
#define SDA_W(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_9,(x))
#define SDA_R		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)
#define SCL_W(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_8,(x))

/*********************
 *	MACRO
 *********************/

/*********************
 *	GLOBAL VALUE
 *********************/



/*********************
 *	GLOBAL VALUE
 *********************/

/*********************
 *	FUNCTION
 *********************/
 
void AT24C02_Init(void);
void SDA_Pin_Mode(GPIOMode_TypeDef pin_mode);
void IIC_Start(void);
void IIC_Stop(void);
uint8_t IIC_Wait_Slave_Ack(void);
void IIC_SendAck(uint8_t ack);
void IIC_Send_Byte(uint8_t byte);
uint8_t IIC_Recv_Byte(void);
int32_t AT24C02_Write(uint8_t word_addr, uint8_t *buf, uint8_t len);
void AT24C02_Read(uint8_t word_addr, uint8_t *buf, uint8_t len);

 /*********************
 *	FUNCTION
 *********************/

#endif
