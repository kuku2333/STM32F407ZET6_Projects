#ifndef __TIM_H__
#define __TIM_H__


extern volatile uint8_t  g_usart1_rx_buf[1280];
extern volatile uint32_t g_usart1_rx_cnt;
extern volatile uint32_t g_usart1_rx_end;

extern void tim3_init(void);
void Tim14_Init(void);



#endif

