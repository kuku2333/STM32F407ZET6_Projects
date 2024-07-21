#ifndef _Uasrt_Send_String_H
#define _Uasrt_Send_String_H

#include "stm32f4xx.h"                  // Device header
#include "Delay.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/*********************
 *	MACRO
 *********************/
 
 
 
 /*********************
 *	MACRO
 *********************/

 /*********************
 *	GLOBAL VALUE
 *********************/
 
extern RTC_TimeTypeDef  	RTC_TimeStructure;
extern RTC_InitTypeDef  	RTC_InitStructure;
extern RTC_DateTypeDef 		RTC_DateStructure;

extern __IO uint32_t 		RTCTimeDisplay;
extern uint8_t 				RTCShowTime[50];
extern uint8_t 				RTCShowDate[50];
 
extern uint8_t buffer[100]; // 根据需要调整大小
//extern uint8_t buffer_index = 0;

 /*********************
 *	GLOBAL VALUE
 *********************/

 /*********************
 *	FUNCTION
 *********************/
 
void Uasrt1_Init(void);
void Send_String(char *string);
bool parse_time(char *time_str, RTC_TimeTypeDef *RTC_TimeStructure);
void process_command(uint8_t *cmd);
void Uasrt2_Init(void);

 /*********************
 *	FUNCTION
 *********************/
#endif
