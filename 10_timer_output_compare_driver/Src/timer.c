#include <stm32f4xx.h>

#define GPIOAEN             (1U << 0)
#define APB1ENR_TIM2EN      (1U << 0)
#define TIMx_CR1_CEN        (1U << 0)
#define TIMx_CCER_CC1E      (1U << 0)
#define TIMx_CCMR1_OC1M_TOGGLE  ((1U << 4) | (1U << 5))  // Toggle Mode (011)
#define TIMx_CCMR1_OC1PE    (1U << 3) // Output Compare Preload Enable

void tim2_pa5_output_compare(void) {
    // Enable clock access to GPIOA
    RCC->AHB1ENR |= GPIOAEN;

    // Set PA5 to alternate function mode
    GPIOA->MODER &= ~(1U << 10);  // Clear bit 10
    GPIOA->MODER |= (1U << 11);   // Set bit 11 (AF mode)

    // Set PA5 alternate function to AF1 (TIM2_CH1)
    GPIOA->AFR[0] &= ~(0xF << 20);  // Clear bits 20-23
    GPIOA->AFR[0] |= (1U << 20);    // AF1 (TIM2_CH1)

    // Enable clock access to TIM2
    RCC->APB1ENR |= APB1ENR_TIM2EN;

    // Set prescaler value
    TIM2->PSC = 16000 - 1;  // 16MHz / 16000 = 1kHz

    // Set auto-reload value
    TIM2->ARR = 1000 - 1;   // 1kHz / 1000 = 1Hz (1s)

    // Set Output Compare Toggle mode on Channel 1
    TIM2->CCMR1 |= TIMx_CCMR1_OC1M_TOGGLE;  // Set OC1M = 011 (toggle mode)
    TIM2->CCMR1 |= TIMx_CCMR1_OC1PE;        // Enable preload (recommended)

    // Enable Output Compare Channel 1
    TIM2->CCER |= TIMx_CCER_CC1E;

    // Clear counter
    TIM2->CNT = 0;

    // Enable timer
    TIM2->CR1 |= TIMx_CR1_CEN;
}

void tim2_1hz_init(void) {
    // Enable clock access to TIM2
    RCC->APB1ENR |= APB1ENR_TIM2EN;

    // Set prescaler value
    TIM2->PSC = 16000 - 1;  // 16MHz / 16000 = 1kHz

    // Set auto-reload value
    TIM2->ARR = 1000 - 1;   // 1kHz / 1000 = 1Hz

    // Clear counter
    TIM2->CNT = 0;

    // Enable timer
    TIM2->CR1 |= TIMx_CR1_CEN;
}
