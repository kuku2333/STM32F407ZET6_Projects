/**
   ******************************************************************************
   * @file    STM32F407ZET6_RTC\HARDWARE\RTC.h
   * @author  xzdtx2333@outlook.com
   * @version V1.0.0
   * @date    2024-07-18
   * @brief   
   ******************************************************************************
   * 	用RTC得到日期，年月日、时分秒
   ******************************************************************************
**/

#ifndef __RTC_H
#define __RTC_H

/*********************
 *	MACRO
 *********************/
 
#include "stm32f4xx.h"                  // Device header
#include "Uasrt_Send_String.h"

/*********************
 *	MACRO
 *********************/
 
 /*********************
 *	GLOBAL VALUE
 *********************/
 
extern RTC_TimeTypeDef  	RTC_TimeStructure;
extern RTC_InitTypeDef  	RTC_InitStructure;
extern RTC_DateTypeDef 		RTC_DateStructure;

//extern __IO uint32_t 		RTCTimeDisplay = 0;
//extern uint8_t 				RTCShowTime[50] = {0};
//extern uint8_t 				RTCShowDate[50] = {0};
 
 /*********************
 *	GLOBAL VALUE
 *********************/
 
 /*********************
 *	FUNCTION
 *********************/
 
void RTC_ShowTime(void);
void RTC_Calendar_Init(void);

/*********************
 *	FUNCTION
 *********************/
#endif
