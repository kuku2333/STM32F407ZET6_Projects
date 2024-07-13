#ifndef _Uasrt_Send_String_H
#define _Uasrt_Send_String_H

#include "stm32f4xx.h"                  // Device header
#include <string.h>

#define REVSIZE 128



void Uasrt1_Init();
void Send_String(char *string);

#endif