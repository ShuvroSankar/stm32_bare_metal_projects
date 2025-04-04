#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include "uart.h"
#include "timer.h"

volatile int timeStamp = 0;  // Make it volatile to be updated in debugging

// Set jumper wire PA5 to PA6
int main(void) {
    tim2_pa5_output_compare();
    tim3_pa6_input_capture();

    while (1) {
        // Wait until edge is captured
        while (!(TIM3->SR & TIMx_SR_CC1F)) {}

        // Read capture value from CCR1
        timeStamp = TIM3->CCR1;

        // Clear the capture flag
        TIM3->SR &= ~TIMx_SR_CC1F;
    }
}
