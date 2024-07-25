/**
   ******************************************************************************
   * @file    STM32F407ZET6_IIC\HARDWARE\IIC_xzdtx.h 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-14
   * @brief   IIC协议
   ******************************************************************************
   * 	记得改引脚，移植时改函数名
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
 
#define SDA_W(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_9,(x))		//数据线写
#define SDA_R		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) //数据线读
#define SCL_W(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_8,(x))		//时钟线写

#define DELAY_TIME 	5 //延时时间，单位us

#define TOUCH_IIC_SCK_PIN					TOUCH_GPIO_Port,TOUCH_GPIO_Pin

#define IIC_Digital_WRITE(_pin, _value) 	GPIO_WriteBit(_pin, _value == 0 ? Bit_RESET : Bit_SET)
#define IIC_Digital_READ(_pin) 				GPIO_ReadInputDataBit(_pin)
#define IIC_CLOCK_WRITE(_pin, _value)		GPIO_WriteBit(_pin, _value == 0 ? Bit_RESET : Bit_SET)

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
 
void IIC_Init(void);
void SDA_Pin_Mode(GPIOMode_TypeDef pin_mode);
void IIC_Start(void);
void IIC_Stop(void);
uint8_t IIC_Wait_Slave_Ack(void);
void IIC_Send_Ack(uint8_t ack);
void IIC_Send_Byte(uint8_t byte);
uint8_t IIC_Read_Byte(uint8_t ack);
void IIC_Read_One_Byte(uint8_t daddr, uint8_t addr, uint8_t *data);
void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
void IIC_Write_Bytes(uint8_t write_addr, uint8_t *data, uint8_t data_Length);
void IIC_Read_Bytes(uint8_t device_addr, uint8_t write_addr, uint8_t *data, uint8_t data_length);

 /*********************
 *	FUNCTION
 *********************/

#endif
