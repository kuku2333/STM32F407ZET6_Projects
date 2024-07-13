#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f4xx.h"                  // Device header
#include "sys.h"
#include "Delay.h"

void dht11_init(void);
FunctionalState judge_DHT_resp(void);
uint32_t DHT_recv_data(void);

#endif
