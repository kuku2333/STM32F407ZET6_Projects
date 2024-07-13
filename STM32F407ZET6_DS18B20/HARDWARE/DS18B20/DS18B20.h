/**
   ******************************************************************************
   * @file    STM32F407ZET6_DS18B20\HARDWARE\DS18B20.h
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-12
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f4xx.h"                  // Device header
#include "sys.h"
#include "Delay.h"
#include "Gpio.h"
#include "Uasrt_Send_String.h"

#define GPIO_Mode_In()	GPIO_In_Init(RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_6, ENABLE);
#define GPIO_Mode_Out() GPIO_Out_Init(RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_6, ENABLE);

#define DS18B20_DQ_In		PDin(6)
#define DS18B20_DQ_SET		PDout(6) = 1
#define DS18B20_DQ_RESET	PDout(6) = 0

uint8_t DS18B20_Master_Read_Byte(void);
uint8_t DS18B20_Master_Read_Slot(void);
void DS18B20_Master_Write_Command(uint8_t cmd);
void DS18B20_Master_Write_Slot(uint8_t value);
uint8_t DS18B20_Init(void);

void test(void);

#endif
