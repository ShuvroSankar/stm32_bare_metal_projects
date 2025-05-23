#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include "uart.h"

#define GPIOAEN			(1U<<0)
#define LED_PIN			(1U<<5)


//int main(void){
//	char message[31] = "Hello from stm32 DMA transfer\n\r";
//	RCC->AHB1ENR |= GPIOAEN;
//	GPIOA->MODER |= (1U<<10);
//	GPIOA->MODER &= ~(1U<<11);
//    uart_tx_init();
//    dma1_stream6_init((uint32_t) message, (uint32_t) &USART2->DR, 31);
//    while(1){
//
//    }
//}

int main(void){
    char message[31] = "Hello from stm32 DMA transfer\n\r";

    RCC->AHB1ENR |= GPIOAEN;
    GPIOA->MODER |= (1U<<10);
    GPIOA->MODER &= ~(1U<<11);

    uart_tx_init();
    dma1_stream6_init((uint32_t)message, (uint32_t)&USART2->DR, 31);

    while(1){}
}


static void dma_callback(){
GPIOA->ODR |= LED_PIN;
}

//void DMA_Stream6_IRQHandler(void){
//// Test for transfer complete interrupt
//	if(DMA1->HISR & DMA_HISR_TCIf6){
////		Clear flag
//		DMA1->HIFCR |= DMA_HIFCR_CTCIF6;
////		do somethig
//		dma_callback();
//
//	}
//
//}
void DMA1_Stream6_IRQHandler(void){
    if (DMA1->HISR & DMA_HISR_TCIf6){
        // Clear transfer complete flag
        DMA1->HIFCR |= DMA_HIFCR_CTCIF6;

        // Call your callback
        dma_callback();
    }
}

