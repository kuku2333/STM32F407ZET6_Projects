/**
   ******************************************************************************
   * @file    STM32F407ZET6_IIC\HARDWARE\IIC_xzdtx.h 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-14
   * @brief   IICЭ��
   ******************************************************************************
   * 	�ǵø����ţ���ֲʱ�ĺ�����
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

#define TOUCH_WRITE_PORT			GPIOB
#define TOUCH_WRITE_PIN				GPIO_Pin_8
#define TOUCH_READ_PORT				GPIOB
#define TOUCH_READ_PIN				GPIO_Pin_9

#define TOUCH_PINS					(GPIO_Pin_8 | GPIO_Pin_9)

#define TOUCH_IIC_SCK_PIN		(TOUCH_GPIO_PORT, TOUCH_GPIO_PIN)
#define TOUCH_IIC_SDA_PIN		(TOUCH_GPIO_PORT, TOUCH_GPIO_PIN)

#define IIC_DIGITAL_WRITE(_pin, _value) 	GPIO_WriteBit(_pin, _value ? Bit_RESET : Bit_SET)
#define IIC_DIGITAL_READ(_pin) 				GPIO_ReadInputDataBit(_pin)

#define IIC_SDA_MODE(_mode, _pin)			SDA_Pin_Mode(_mode ? GPIO_Mode_IN : GPIO_Mode_OUT, _pin)

#define IIC_INIT_FOR_TOUCH(_pin) 				IIC_Init(_pin)

#define DELAY_TIME 	5 //��ʱʱ�䣬��λus

//typedef TOUCH_IIC_SCK_PIN touch_iic_sck_pin;

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
 
void IIC_Init(uint32_t GPIOx);
void SDA_Pin_Mode(GPIOMode_TypeDef pin_mode, GPIO_TypeDef* GPIOx, uint32_t GPIOx);
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
