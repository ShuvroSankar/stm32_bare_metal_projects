/*
 * adc.h
 *
 *  Created on: Apr 4, 2025
 *      Author: Shuvro
 */

#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>
#include <stm32f4xx.h>

void pal_adc_init(void);
void start_convertion(void);
uint32_t adc_read(void);

#endif /* ADC_H_ */
