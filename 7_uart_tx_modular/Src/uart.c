#include "uart.h"

#define GPIOAEN         (1U<<0)
#define UART2EN        (1U<<17)
#define CR1_TE          (1U<<3)
#define CR1_UE          (1U<<13)
#define SR_TXE          (1U<<7)
#define SYS_FREQ        16000000
#define APB1_CLK        SYS_FREQ
#define UART_BAUDRATE   115200

void uart_tx_init(void);
static uint32_t calculate_uart_bd(uint32_t periphCLK, uint32_t baudrate);
void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periphCLK, uint32_t baudrate);
void uart2_write(int ch);

int __io_putchar(int ch){
	uart2_write(ch);
	return ch;
}



void uart_tx_init(void){
    // Enable clock for GPIOA
    RCC->AHB1ENR |= GPIOAEN;

    // Set PA2 mode to Alternate Function (AF)
    GPIOA->MODER |= (1U<<5);
    GPIOA->MODER &= ~(1U<<4);

    // Set PA2 Alternate Function to AF7 (USART2 TX)
    GPIOA->AFR[0] &= ~(0xF << 8); // Clear bits 8-11 for PA2
    GPIOA->AFR[0] |= (7U << 8);  // AF7 is 0111

    // Enable clock access to UART2
    RCC->APB1ENR |= UART2EN;

    // Set baud rate
    uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

    // Enable Transmitter
    USART2->CR1 |= CR1_TE;

    // Enable UART Module
    USART2->CR1 |= CR1_UE;
}

void uart2_write(int ch){
    // Wait until transmit data register is empty
    while (!(USART2->SR & SR_TXE)) {}

    // Write to transmit data register
    USART2->DR = (ch & 0xFF);
}

// Set baud rate
void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periphCLK, uint32_t baudrate){
    USARTx->BRR = calculate_uart_bd(periphCLK, baudrate);
}

static uint32_t calculate_uart_bd(uint32_t periphCLK, uint32_t baudrate){
    return ((periphCLK + (baudrate / 2U)) / baudrate);
}
