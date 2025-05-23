#include <stm32f4xx.h>

#define APB1ENR_TIM2EN		(1U<<0)
#define TIMx_CR1_CEN		(1U<<0)


void tim2_1hz_init(void){
//	enable clock acces to tim2
	RCC->APB1ENR |= APB1ENR_TIM2EN;
//	set prescaler value
	TIM2->PSC  = 16000-1; // 16000000/1600 = 10000
//	set auto reload value
	TIM2->ARR = 1000-1;
//	clear data
	TIM2->CNT = 0;
//	enable timer
	TIM2->CR1 |= TIMx_CR1_CEN;

}
