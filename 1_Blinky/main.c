//Program: Blink LED 

//LED connected to PA5 pin
//Clock gating: Port A is connected to AHB1 bus.


//Register to enable clock
// - RCC -> AHB1ENR: 0x30

//Need direction and data registers
		// - Mode register (direction) GPIOx_MODER
		// - Data register: GPIOx_ODR
		
#include "stm32f4xx.h"                  // Device header

//Prototype
void delay_ms(int delay);

int main(void)
{
	RCC->AHB1ENR  |= 1;  			// enable GPIOA clock
	GPIOA ->MODER |= 0x400;		// PA5 set to 01 -> output direction
	
	
	while (1)
	{
		GPIOA->ODR = 0x20;			//Output PA5 bit value set to 1. 
		delay_ms(50);
		GPIOA->ODR &=~ 0x20;			//Output PA5 bit value set to 0.
		delay_ms(50);
	}
	
}

//Taking into account 16 MHz clock
void delay_ms(int delay)
{
	int i;
	for(; delay>0; delay--)
	{
		for(i = 0; i <3195;i++);
	}
}

