/**
   ******************************************************************************
   * @file    STM32F407ZET6_SPI_W25Q128\HARDWARE\W25Q128.h
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-15
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#ifndef __W25Q128_H
#define __W25Q128_H

#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include "sys.h"

/*********************
 *	MACRO
 *********************/
 
#define W25Q128_CS_STATUS(n)     (n) ? GPIO_SetBits(GPIOB, GPIO_Pin_14) : GPIO_ResetBits(GPIOB, GPIO_Pin_14)

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
 
uint16_t W25Q128_Send_Byte(uint8_t byte);
uint16_t W25Q128_Get_ID(void);
void W25Q128_Config(void);
void W25Q128_Write(uint32_t addr, uint8_t *msg);
void W25Q128_Read(uint32_t addr, uint8_t *buf, uint32_t len);
void W25Q128_Erase_Sector(uint32_t sector_addr);
uint8_t W25Q128_Read_Status1(void);
void W25Q128_Write_Disable(void);
void W25Q128_Write_Enable(void);
 
 /*********************
 *	FUNCTION
 *********************/
#endif
