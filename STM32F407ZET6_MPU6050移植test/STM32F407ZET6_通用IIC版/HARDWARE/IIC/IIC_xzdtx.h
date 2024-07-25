/**
   ******************************************************************************
   * @file    STM32F407ZET6_IIC_new\HARDWARE\IIC_xzdtx.h 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-23
   * @brief   IIC协议
   ******************************************************************************
   * 	通用版IIC底层代码
   *	使用时，先I2C_Port_t类型变量，如：
   *	I2C_Port_t port1 = {GPIOB, GPIOB， 
   *						GPIO_Pin_8, GPIO_Pin_9, 
   *						RCC_AHB1Periph_GPIOB, RCC_AHB1Periph_GPIOB};
   *	然后传参即可。
   *	注意，IIC_Wait_Slave_Ack返回1是非应答，0是应答；
   *	IIC_Send_Ack函数也是，传1表示非应答，0应答；
   *	IIC_Read_Byte函数里已写发送应答语句，所以其传1表示非应答，
   *	传0表示应答。
   *	而IIC_Send_Byte函数要根据需要，在调用后判断从机是否应答。
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
 
//#define SDA_W(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_9,(x))		//数据线写
//#define SDA_R		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) //数据线读
//#define SCL_W(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_8,(x))		//时钟线写

#define DELAY_TIME 	5 //延时时间，单位us

/*********************
 *	MACRO
 *********************/

/*********************
 *	GLOBAL VALUE
 *********************/

typedef struct {
    GPIO_TypeDef* SDA_GPIOx;    // GPIO端口
	GPIO_TypeDef* SCL_GPIOx;    // GPIO端口
    uint16_t SDA_Pin;       // 数据线引脚
    uint16_t SCL_Pin;       // 时钟线引脚
    uint32_t SDA_RCC_AHBPeriph; // 对应的RCC时钟
	uint32_t SCL_RCC_AHBPeriph; // 对应的RCC时钟
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
