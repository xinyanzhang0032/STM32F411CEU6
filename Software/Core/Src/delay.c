#include "delay.h"



uint32_t usTicks = 72;
uint32_t msTicks = 72000;

void delay_init()
{
	uint32_t SysFreq = HAL_RCC_GetHCLKFreq();
	msTicks = SysFreq/1000;
	usTicks = msTicks/1000;
}

void delay_us(uint32_t nus)
{
	uint32_t ticks = nus*usTicks;
	uint32_t old,now;
	now = old = SysTick->VAL;
	while((msTicks+old-now)%msTicks<ticks)
	{
		now = SysTick->VAL;
	}
}




void delay_ms(uint16_t nms)
{
	HAL_Delay(nms);
}

void delay_s(uint16_t ns)
{
	uint16_t n;
	for(n=0;n<ns;n++)
	{
		HAL_Delay(1000);
	}
}
