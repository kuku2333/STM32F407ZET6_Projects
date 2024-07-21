/**
   ******************************************************************************
   * @file    STM32F407ZET6_RTC\HARDWARE\RTC.h
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-13
   * @brief   
   ******************************************************************************
   * 
   ******************************************************************************
**/

#ifndef __RTC_H
#define __RTC_H

#include "stm32f4xx.h"                  // Device header
#include "Uasrt_Send_String.h"


void RTC_ShowTime(void);
void RTC_Calendar_Init(void);

#endif
