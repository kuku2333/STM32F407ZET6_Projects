#include "Iwdg.h"

/**
	*****************************************************************************
	*@brief 使用独立看门狗功能
	*@steps 1.让IWDG_PR和IWDG_RLR寄存器可以写入，使用IWDG_WriteAccessCmd
	*		(IWDG_WriteAccess_Enable)函数
	*		2.配置内部看门狗预分频值，使用这个函数IWDG_SetPrescaler()
	*		3.使用这个IWDG_SetReload()函数配置计数器的值
	*		4.使用这个IWDG_Enable()函数开启独立看门狗
	*		5.使用这个IWDG_ReloadCounter()函数喂狗
	*****************************************************************************
**/

void Iwdg_Init(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(1000);
	IWDG_Enable();
}

