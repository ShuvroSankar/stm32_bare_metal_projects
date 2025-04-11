#include<stm32f4xx.h>

#define SYSTICK_LOAD_VAL		16000
#define CTRL_ENABLE				(1U<<0)
#define CTRL_CLKSRC				(1U<<12)
#define CTRL_COUNTFLAG			(1U<<16)

void systicDelayMs(int delay){
//	Reload with the number of clock per milisecond
	SysTick->LOAD = SYSTICK_LOAD_VAL;
//	Clear systick current value register
	SysTick->VAL = 0;
//	Enable systick and select internal clk source
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC ;

	for (int i=0; i<delay; i++){
//		Wait untill the countflag is set
		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0){}
	}
	SysTick->CTRL = 0;



}
