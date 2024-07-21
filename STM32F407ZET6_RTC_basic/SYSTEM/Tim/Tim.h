/**
   ******************************************************************************
   * @file    STM32F407ZET6_外设完备版\HARDWARE\Tim.h
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-10
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#ifndef __TIM_H
#define __TIM_H

#include "stm32f4xx.h"                  // Device header
#include "Uasrt_Send_String.h"

void Tim14_PWM_Init(void);
void Tim13_InputCapture_Init(void);
void Input_Capture_test(void);

#endif
