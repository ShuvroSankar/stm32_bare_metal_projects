

#ifndef UART_H_
#define UART_H_
#include <stdint.h>
#include <stm32f4xx.h>


void uart_tx_init(void);
//void uart_rx_init(void);
void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len);
#define DMA_HISR_TCIf6		(1U<<21)
#define DMA_HIFCR_CTCIF6	(1U<<21)
#endif /* UART_H_ */
