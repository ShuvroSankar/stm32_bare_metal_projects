#include "adc.h"

#define GPIOAENR			(1U<<0)
#define APB2ENR_ADC1EN	(1U<<8)
#define SQR3_CH1		(1U<<0)
#define AEQ_LEN_1		0x00
#define CR2_ADON		(1U<<0)
#define CR2_SWSTART		(1U<<30)
#define SR_EOC			(1U<<1)
#define CR2_CONT		(1U<<1)

void pal_adc_init(void){
//	enable clock access to GPIOA
 RCC->AHB1ENR = GPIOAENR;
// set the mode of PA1 to analog
 GPIOA->MODER |= (1U<<2);
 GPIOA->MODER |= (1U<<3);
// enable clock access to ADC1
 RCC->APB2ENR |= APB2ENR_ADC1EN;
// Convertion seq start
 ADC1->SQR3 = SQR3_CH1;
// Conversion seq lemngth
 ADC1->SQR3 = AEQ_LEN_1;
// Enable ADC module
 ADC1->CR2 |= CR2_ADON;

}

void start_convertion(void){
//	Enable continious conversion
	ADC1->CR2 |= CR2_CONT;
//	Start adc conversion
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void){
//	wait convertion to be complete
	while(!(ADC1->SR & SR_EOC)){}
//	Read convertion result
	return (ADC1->DR);
	}


