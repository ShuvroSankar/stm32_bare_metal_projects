// Where is the led connector?
// Port:A
// Pin:5
// UL = Unsigned Link
#include <stdint.h>
#define PERIPH_BASE  		(0x40000000UL)

#define AHB1PERIPH_OFFSET    (0x00020000UL)
#define AHB1PERIPH_BASE      (PERIPH_BASE + AHB1PERIPH_OFFSET)
#define GPIOA_OFFSET 		 (0x0000U)
#define GPIOA_BASE			 (AHB1PERIPH_BASE + GPIOA_OFFSET)
#define RCC_OFFSET 			 (0x3800UL)
#define RCC_BASE 			 (AHB1PERIPH_BASE + RCC_OFFSET)
// 0x4002 0000 +  0x0002 0000    =0x4002 0000
//#define  AHB1ENN_R_OFFSET	 (0x30UL)
//#define RCC_AHB1ENN_R		 (*(volatile unsigned int *)( RCC_BASE + AHB1ENN_R_OFFSET))
//
//#define MODE_R_OFFSET		 (0x00UL)
//
//#define GPIOA_MODE_R		 (*(volatile unsigned int *)(GPIOA_BASE + MODE_R_OFFSET))
//#define OD_R_OFFSET          (0x14UL)
//#define GPIOA_OD_R           (*(volatile unsigned int *)(GPIOA_BASE + OD_R_OFFSET))

#define  GPIOAEN			 ( 1U<<0 )
#define PIN5				 (1U<<5)
#define LED_PIN 			 PIN5
#define __IO volatile

//typedef struct
//{
//}GPIO_TypeDef;





typedef struct
{

  volatile uint32_t DUMMY[12];
  volatile uint32_t AHB1ENR;      /*!< RCC AHB1 peripheral clock enable register, Address offset: 0x30 */

} RCC_TypeDef;

typedef struct
{
	volatile uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00 */

	volatile uint32_t DUMMY[4];
    volatile uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14 */

} GPIO_TypeDef;

#define RCC 		((RCC_TypeDef*)RCC_BASE)
#define GPIOA		((GPIO_TypeDef*)GPIOA_BASE)


//(1<<10) // Set bit 10 to 1
//
//&=~(1U<<11) // Set bit 11 to 0

int main(void){
	//1. Enable clock access to GPIOA
//	RCC_AHB1ENN_R |= GPIOAEN;    // Friendly programming
	RCC->AHB1ENR |= GPIOAEN;
	//2. Set PA5 as output pin
//	GPIOA_MODE_R |= (1<<10); // Set bit 10 to 1
//	GPIOA_MODE_R &=~(1U<<11); // Set bit 11 to 0
	GPIOA->MODER |= (1<<10); // Set bit 10 to 1
	GPIOA->MODER &=~(1U<<11); // Set bit 11 to 0
	while(1)
	{

		//3. Set  PA5  High
//		GPIOA_OD_R = LED_PIN;
		//4. Toggle PA5
//		GPIOA_OD_R ^= LED_PIN;
		GPIOA->ODR ^= LED_PIN;
		for(int i=0;i<1000000;i++){}
	}


}
