#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"

#define GPIOAEN			(1U<<0)
#define LED_PIN			(1U<<5)

int main(void){
//	Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN	;
//	Set LED pin as output
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);
    uart_tx_init();


    while(1){

        printf("A second passed \r\n");
        GPIOA->ODR |= LED_PIN;
        systicDelayMs(1000);
    }
}



