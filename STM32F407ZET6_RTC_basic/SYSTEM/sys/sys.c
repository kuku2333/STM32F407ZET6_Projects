#include "sys.h"  

//********************************************************************************
//修改说明
//无
//////////////////////////////////////////////////////////////////////////////////  

/*THUMB指令不支持汇编内联
采用如下方法实现执行汇编指令WFI 
WFI是等待中断（Wait For Interrupt）的缩写，当执行这条指令时，
CPU会进入低功耗状态，直到有中断发生时才会唤醒*/
__asm void WFI_SET(void)
{
	WFI;		  
}

//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}

//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}

//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}

///* 重定向，可使用printf函数,需要根据情况改串口*/
//struct __FILE { int handle; /* Add whatever you need here */ };
//FILE __stdout;
//FILE __stdin;

//int fputc(int ch, FILE *f)
//{
//	USART_SendData(USART1,ch);	
//	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET );
//		
//	return ch;
//}
