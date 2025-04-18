#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"

#define GPIOAEN        (1U << 0)
#define DHT22_PIN      (1U << 5)
//#define LED_PIN        (1U << 5) // PA5 (optional LED)

short unsigned int delay = 2000000;

char buffer[50];
void TIM2_Init(void);
void Delay_us(uint16_t us);
uint8_t Read_DHT22_Byte(void);

int main(void) {


    // Initialize UART and Timer
    uart_rxtx_init(); // Initialize UART2 for TX/RX
    printf("UART Initialized!\r\n");
    TIM2_Init();

    // Enable GPIOA clock
    RCC->AHB1ENR |= GPIOAEN;

    // Configure PA5 (DHT22 data pin)
    GPIOA->MODER |= (1 << 10);     // PA5 as output
    GPIOA->OTYPER &= ~(1 << 5);    // Push-pull
    GPIOA->PUPDR &= ~(3 << 10);    // Disable internal pull-up

    while (1) {
        printf("Starting sensor read...\r\n"); // Add this line

        // Send start signal to DHT22
        GPIOA->ODR &= ~DHT22_PIN;
        Delay_us(1000);
        GPIOA->ODR |= DHT22_PIN;
        GPIOA->MODER &= ~(3 << 10);
        Delay_us(30);

        printf("Waiting for sensor response...\r\n"); // Add this line

        // Wait for DHT22 response
        uint32_t timeout = 1000000;
        while ((GPIOA->IDR & DHT22_PIN) && timeout--);
        if (!timeout) {
            printf("Sensor error: No response!\r\n"); // Enhanced error message
            continue;
        }
        timeout = 1000000;
        while (!(GPIOA->IDR & DHT22_PIN) && timeout--); // Wait until high

        // Read data
        uint8_t data[5];
        for (int i = 0; i < 5; i++) data[i] = Read_DHT22_Byte();

        // Validate checksum
        if (data[4] != (data[0] + data[1] + data[2] + data[3])) {
            printf("Checksum error!\r\n");
            continue;
        }

//         Calculate values
        int humidity = data[0] << 8 | data[1]; // e.g., 625 = 62.5%
        int temperature = data[2] << 8 | data[3]; // e.g., 234 = 23.4°C

        // Print data via UART
        printf("Temp: %d.%dC, Humidity: %d.%d%%\r\n",
               temperature / 10, temperature % 10,
               humidity / 10, humidity % 10);

        // Optional delay between readings
        Delay_us(delay); // 2-second delay
    }
}

// Timer2 Initialization (for µs delays)
void TIM2_Init(void) {
    RCC->APB1ENR |= (1U << 0);  // Enable TIM2 clock
    TIM2->PSC = 16 - 1;         // 16 MHz / 16 = 1 MHz (1 µs/tick)
    TIM2->ARR = 0xFFFF;
    TIM2->CR1 |= (1 << 0);      // Enable timer
}

// Microsecond delay
void Delay_us(uint16_t us) {
    TIM2->CNT = 0;
    while (TIM2->CNT < us);}

// Read a byte from DHT22
uint8_t Read_DHT22_Byte(void) {
    uint8_t data = 0;
    for (int i = 0; i < 8; i++) {
        while (!(GPIOA->IDR & DHT22_PIN)); // Wait for high
        Delay_us(50); // Wait 50 µs to read bit value
        if (GPIOA->IDR & DHT22_PIN) data |= (1 << (7 - i));
        while (GPIOA->IDR & DHT22_PIN);    // Wait for low
    }
    return data;
}
