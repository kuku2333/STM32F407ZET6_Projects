/**
   ******************************************************************************
   * @file    STM32F407ZET6_外设完备版\HARDWARE\Gpio.h
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-09
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f4xx.h"                  // Device header

void GPIO_Out_Init(uint32_t RCC_AHB1Periph_GPIOx, GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x, FunctionalState NewState);
void GPIO_In_Init(uint32_t RCC_AHB1Periph_GPIOx, GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x, FunctionalState NewState);

#endif
