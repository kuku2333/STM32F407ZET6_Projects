#include "RTC.h"

RTC_TimeTypeDef  	RTC_TimeStructure;
RTC_InitTypeDef  	RTC_InitStructure;
RTC_DateTypeDef 	RTC_DateStructure;

__IO uint32_t 		RTCTimeDisplay = 0;
uint8_t 			RTCShowTime[50] = {0};
uint8_t 			RTCShowDate[50] = {0};

/*	*/
void RTC_Calendar_Init(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	/* 使能PWR时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	/* 使能访问RTC域*/
	PWR_BackupAccessCmd(ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_ClearITPendingBit(EXTI_Line22);
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	RCC_LSEConfig(RCC_LSE_ON);
  
    /* Wait till LSE is ready */  
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
  
    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	RCC_RTCCLKCmd(ENABLE);
	
	RTC_WaitForSynchro();

	RTC_InitStructure.RTC_AsynchPrediv = 128 - 1;
    RTC_InitStructure.RTC_SynchPrediv = 256 - 1;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_Init(&RTC_InitStructure);

	/* Set the date: Friday January 11th 2013 */
    RTC_DateStructure.RTC_Year = 0x24;
    RTC_DateStructure.RTC_Month = RTC_Month_July;
    RTC_DateStructure.RTC_Date = 0x14;
    RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Sunday;
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
    
    /* Set the time to 05h 20mn 00s AM */
    RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours   = 0x12;
    RTC_TimeStructure.RTC_Minutes = 0x55;
    RTC_TimeStructure.RTC_Seconds = 0x00; 
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   
    
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits); //采用内部时钟 = 1HZ
	RTC_SetWakeUpCounter(1-1);
	
	RTC_ITConfig(RTC_IT_WUT, ENABLE);
	
	RTC_ClearITPendingBit(RTC_IT_WUT);

	RTC_WakeUpCmd(ENABLE); 
}

void RTC_WKUP_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
  {
     RTCTimeDisplay = 1;
	  
     RTC_ClearITPendingBit(RTC_IT_WUT);
     EXTI_ClearITPendingBit(EXTI_Line22);
  }
}

void RTC_ShowTime(void)
{	
	if (RTCTimeDisplay == 1)
		{
			//显示时间
			RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
			RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
			
			printf("%0.2d:%0.2d:%0.2d\r\n",	RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
			printf("20%0.2d-%0.2d-%0.2d\r\n", RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month,RTC_DateStructure.RTC_Date);			
			RTCTimeDisplay = 0;
		}	
}
