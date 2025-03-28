#include<stm32f4xx.h>

#define GPIOAEN			(1U<<0)
#define GPIOCEN			(1U<<2)

#define PIN5			(1U<<5)
#define PIN13			(1U<<13)

#define LED_PIN			PIN5
#define PUSH_BTN		PIN13

int main(void){
//	activate clock
	RCC->AHB1ENR |= GPIOAEN;
	RCC->AHB1ENR |= GPIOCEN;

//	set output pin
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER  &=~(1U<<11);
	while(1){

		if(GPIOA->IDR & PUSH_BTN){
//			set led
		GPIOA->BSRR = LED_PIN;
		for(int i=0; i<100000; i++){}
		}
		else{
//			reset led
		GPIOA->BSRR = (1U<<21);
		for(int i=0; i<100000; i++){}
		}
	}


}

