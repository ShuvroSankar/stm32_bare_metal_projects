#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include "uart.h"
#include "timer.h"

#define LED_PIN			(1U<<5)
#define TIMx_SR_UIF		(1U<<0)

int main(void){
//	anable clock access to gpioa
	RCC->AHB1ENR |= (1U<<0);
//	set pa5 as o/p pin
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~(1U<<11);
	uart_tx_init();
	tim2_1hz_init();
    while(1){
//    	wait for UIF
    	while(!(TIM2->SR & TIMx_SR_UIF)){}
//    	Clear UIF
    	TIM2->SR &=~TIMx_SR_UIF;
        printf("A second passed....\r\n");
        GPIOA->ODR ^= LED_PIN;
    }
}



