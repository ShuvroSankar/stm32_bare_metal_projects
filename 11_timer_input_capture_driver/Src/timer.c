#include <stm32f4xx.h>

#define GPIOAEN             (1U << 0)
#define APB1ENR_TIM2EN      (1U << 0)
#define APB1ENR_TIM3EN      (1U << 1)
#define TIMx_CR1_CEN        (1U << 0)
#define TIMx_CCER_CC1E      (1U << 0)
#define TIMx_CCMR1_OC1M_TOGGLE  ((1U << 4) | (1U << 5))  // Toggle Mode (011)
#define TIMx_CCMR1_OC1PE    (1U << 3) // Output Compare Preload Enable
#define TIMx_CCMR1_CC1S     (1U << 0) // Select CH1 as input
#define GPIOx_AFRL_TIM3     (2U << 24) // AF2 for TIM3 on PA6

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

void tim3_pa6_input_capture(void) {
    // Enable clock access to GPIOA
    RCC->AHB1ENR |= GPIOAEN;

    // Set PA6 to alternate function mode
    GPIOA->MODER &= ~(1U << 12);  // Clear bit 12
    GPIOA->MODER |= (1U << 13);   // Set bit 13 (AF mode)

    // Set PA6 alternate function to AF2 (TIM3_CH1)
    GPIOA->AFR[0] &= ~(0xF << 24);  // Clear bits 24-27
    GPIOA->AFR[0] |= GPIOx_AFRL_TIM3; // Set AF2 (TIM3_CH1)

    // Enable clock access to TIM3
    RCC->APB1ENR |= APB1ENR_TIM3EN;

    // Set prescaler value
    TIM3->PSC = 16000 - 1;  // 16MHz / 16000 = 1kHz

    // Set CH1 as input capture
    TIM3->CCMR1 &= ~(0x3 << 0);  // Clear CC1S bits
    TIM3->CCMR1 |= TIMx_CCMR1_CC1S; // Set CH1 to input mode

    // Clear capture flag
    TIM3->SR = 0;

    // Set CH1 to capture rising edge (default)
    TIM3->CCER |= TIMx_CCER_CC1E;

    // Enable timer
    TIM3->CR1 |= TIMx_CR1_CEN;
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
