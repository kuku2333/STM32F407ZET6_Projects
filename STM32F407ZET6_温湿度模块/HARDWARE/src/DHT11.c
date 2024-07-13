#include "DHT11.h"

void dht11_init(void)
{
	PGout(9) = 0;
	Delay_ms(20);
	PGout(9) = 1;
	Delay_us(30);
}

FunctionalState judge_DHT_resp(void)
{
	FunctionalState flag;
	
	if(PGin(9) == 0)
	{
		flag = ENABLE;
		while(PGin(9) == 0);
		if(PGin(9) == 1)
		{
			flag = ENABLE;
			while(PGin(9) == 1);
		}
		else
		{
			flag = DISABLE;
		}
	}
	else
	{
		flag = DISABLE;
	}
	
	return flag;
}

uint32_t DHT_recv_data(void)
{
	uint32_t count = 0;
	
	Delay_us(50);
	while(PGin(9) != 0);
	while(PGin(9) == 1)
	{
		count++;
	}
	
	return count;
}
