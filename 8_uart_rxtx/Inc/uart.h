

#ifndef UART_H_
#define UART_H_
#include <stdint.h>
#include <stm32f4xx.h>


void uart_tx_init(void);
char uart2_read(void);
void uart_rxtx_init(void);


#endif /* UART_H_ */
