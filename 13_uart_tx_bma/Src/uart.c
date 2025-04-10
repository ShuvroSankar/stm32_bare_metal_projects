#include "uart.h"

#define GPIOAEN         (1U<<0)
#define UART2EN        (1U<<17)
#define CR1_TE          (1U<<3)
#define CR1_UE          (1U<<13)
#define SR_TXE          (1U<<7)
#define SYS_FREQ        16000000
#define APB1_CLK        SYS_FREQ
#define UART_BAUDRATE   115200
#define AHB1ENR_DMA1EN	(1U<<21)
#define DMA_SxCR_EN		(1U<<0)
#define DMA_SxCR_CHSEL	(1U<<27)
#define DMA_SxCR_MINC	(1U<<10)
#define DMA_SxCR_DIR	(1U<<16)
#define DMA_SxCR_TCIE	(1U<<14)
#define DMA_SxCR_EN2	(1U<<0)
#define USART_CR3_DMAT	(1U<<7)

void uart_tx_init(void);
static uint32_t calculate_uart_bd(uint32_t periphCLK, uint32_t baudrate);
void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periphCLK, uint32_t baudrate);
void uart2_write(int ch);

int __io_putchar(int ch){
	uart2_write(ch);
	return ch;
}

//void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len){
////	Enable clock access to DMA
//	RCC->AHB1ENR |= AHB1ENR_DMA1EN;
////	Disable DMA steeam6
//	DMA1_Stream6->CR &= ~DMA_SxCR_EN;
////	Clear all interrupt flags from steam6 (DMA_HIFCR)
//	DMA1->HIFCR |= (1U<<16);
//	DMA1->HIFCR |= (1U<<17);
//	DMA1->HIFCR |= (1U<<18);
//	DMA1->HIFCR |= (1U<<19);
//	DMA1->HIFCR |= (1U<<20);
//	DMA1->HIFCR |= (1U<<21);
////	Set the destination buffer
//	DMA1_Stream6->PAR = dst;
////	Set the source buffer
//	DMA1_Stream6->M0AR = src;
////	Set length
//	DMA1_Stream6->NDTR = len;
////	Select Steam6 CH4
//	DMA1_Stream6->CR = DMA_SxCR_CHSEL;
////	Enable memory increment
//	DMA1_Stream6->CR = DMA_SxCR_MINC;
////	Configure transfer direction --- memory to paripheral
//	DMA1_Stream6->CR = DMA_SxCR_DIR;
////	Enable DMA transfer complete interrupt
//	DMA1_Stream6->CR = DMA_SxCR_TCIE;
////	Enable direct mode and disable FIFO
//	DMA1_Stream6->FCR = 0;
////	Enable DMA1 Steam6
//	DMA1_Stream6->CR = DMA_SxCR_EN2;
////	Enable UART2 transmitter DMA
//	USART2->CR3 = USART_CR3_DMAT;
////	DMA Interupt enable in NVIC
//	NVIC_EnableIRQ(DMA1_Stream_IRQn);
//}

void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len){
    // Enable clock access to DMA1
    RCC->AHB1ENR |= AHB1ENR_DMA1EN;

    // Disable Stream6 before configuring
    DMA1_Stream6->CR &= ~DMA_SxCR_EN;
    while (DMA1_Stream6->CR & DMA_SxCR_EN); // Wait until it's disabled

    // Clear all interrupt flags
    DMA1->HIFCR |= (0x3D << 16); // Clear TCIF, HTIF, TEIF, DMEIF, FEIF for Stream6

    // Set peripheral and memory addresses
    DMA1_Stream6->PAR = dst;
    DMA1_Stream6->M0AR = src;
    DMA1_Stream6->NDTR = len;

    // Configure channel selection and transfer settings
    DMA1_Stream6->CR = (4U << 25)   // CH4 for USART2_TX
                     | DMA_SxCR_MINC
                     | DMA_SxCR_DIR   // Memory to peripheral
                     | DMA_SxCR_TCIE; // Transfer complete interrupt enable

    DMA1_Stream6->FCR = 0; // Direct mode

    // Enable interrupt in NVIC
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);

    // Enable USART2 DMA transmitter
    USART2->CR3 |= USART_CR3_DMAT;

    // Enable the DMA stream last
    DMA1_Stream6->CR |= DMA_SxCR_EN;
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
