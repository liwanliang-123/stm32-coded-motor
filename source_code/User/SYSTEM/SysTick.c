#include "SysTick.h"
#include "stm32f10x.h"

void MS_delay(uint32_t MS)
{
	uint32_t i;
	SysTick_Config(72000); //1ms
	for(i = 0;i < MS;i++){
		
		while( !((SysTick->CTRL) & (1<<16)));
	}
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}

void US_delay(uint32_t US)
{
	uint32_t i;
	SysTick_Config(72); //1us
	for(i = 0;i < US;i++){
		
		while( !((SysTick->CTRL) & (1<<16)));
	}
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}




