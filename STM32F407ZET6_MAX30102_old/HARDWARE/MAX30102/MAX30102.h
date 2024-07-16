/**
   ******************************************************************************
   * @file    STM32F407ZET6_MAX30102\HARDWARE\MAX30102.h 
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-14
   * @brief   使用MAX30102测量心率和血氧度
   ******************************************************************************
   * 
   ******************************************************************************
**/

#ifndef __MAX30102_H
#define __MAX30102_H

#include "stm32f4xx.h"                  // Device header
#include "IIC_xzdtx.h"

/*********************
 *	MACRO
 *********************/

#define MAX30102_INT PCin(6)

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

#define MAX30102_WR_ADDRESS 0xAE

#define I2C_WRITE_ADDR 0xAE
#define I2C_READ_ADDR 0xAF

//register addresses
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_PILOT_PA 0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 0x1F
#define REG_TEMP_FRAC 0x20
#define REG_TEMP_CONFIG 0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF

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
 
void Maxim_MAX30102_Read_FIFO(uint32_t *pun_red_led, uint32_t *pun_ir_led);
uint8_t MAX30102_Bus_Write(uint8_t register_address, uint8_t word_data);
uint8_t MAX30102_Bus_Read(uint8_t register_address);
void MAX30102_FIFO_Read_Words(uint8_t register_address,uint16_t word_data[][2],uint8_t count);
void MAX30102_FIFO_Read_Bytes(uint8_t register_address, uint8_t *data);
void MAX30102_Init(void);
void MAX30102_Reset(void);
void Maxim_MAX30102_Write_REG(uint8_t uch_addr, uint8_t uch_data);
void Maxim_MAX30102_Read_REG(uint8_t uch_addr, uint8_t *puch_data);
						
 /*********************
 *	FUNCTION
 *********************/

#endif
