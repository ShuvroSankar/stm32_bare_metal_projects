#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include "uart.h"
#define GPIOAEN			(1U<<0)
#define PIN5			(1U<<5)
#define LED_PIN			PIN5
char key;



int main(void){
//	enable clock access
	RCC->AHB1ENR |= GPIOAEN;
//	activate led pin
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

    uart_rxtx_init();
    while(1){
    	key = uart2_read();
    	if(key == '1'){
    		GPIOA->ODR |= LED_PIN;
    	}else{
    		GPIOA->ODR &= ~LED_PIN;
    	}

    }
}



