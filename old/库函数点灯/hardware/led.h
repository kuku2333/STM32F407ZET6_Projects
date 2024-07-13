#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"                  // Device header

#define RCC_AHB1ENR      (*((volatile unsigned int *)(0x40023800+0x30)))
	
#define GPIOF_MODER      (*((volatile unsigned int *)(0x40021400+0x00)))	//ֵǿ��ת��Ϊ��ַ  ͨ����ַ�����÷��ʵ�ַ�ռ�
#define GPIOF_OTYPER     (*((volatile unsigned int *)(0x40021400+0x04)))	//ֵǿ��ת��Ϊ��ַ  ͨ����ַ�����÷��ʵ�ַ�ռ�
#define GPIOF_OSPEEDR    (*((volatile unsigned int *)(0x40021400+0x08)))	//ֵǿ��ת��Ϊ��ַ  ͨ����ַ�����÷��ʵ�ַ�ռ�
#define GPIOF_PUPDR      (*((volatile unsigned int *)(0x40021400+0x0C)))	//ֵǿ��ת��Ϊ��ַ  ͨ����ַ�����÷��ʵ�ַ�ռ�
#define GPIOF_ODR        (*((volatile unsigned int *)(0x40021400+0x14)))	//ֵǿ��ת��Ϊ��ַ  ͨ����ַ�����÷��ʵ�ַ�ռ�            

void Led_init(void);

#endif
