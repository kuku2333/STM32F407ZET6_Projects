#include "Iwdg.h"

/**
	*****************************************************************************
	*@brief ʹ�ö������Ź�����
	*@steps 1.��IWDG_PR��IWDG_RLR�Ĵ�������д�룬ʹ��IWDG_WriteAccessCmd
	*		(IWDG_WriteAccess_Enable)����
	*		2.�����ڲ����Ź�Ԥ��Ƶֵ��ʹ���������IWDG_SetPrescaler()
	*		3.ʹ�����IWDG_SetReload()�������ü�������ֵ
	*		4.ʹ�����IWDG_Enable()���������������Ź�
	*		5.ʹ�����IWDG_ReloadCounter()����ι��
	*****************************************************************************
**/

void Iwdg_Init(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(1000);
	IWDG_Enable();
}

