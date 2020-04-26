//Program: LED turns on when an input button is pressed.


#include "stm32f4xx.h"                  // Device header

int main(void)
{
	//Enable clocks for LED and PUSH Button
	RCC->AHB1ENR |= 1;   //LED Port A pin 1
	RCC->AHB1ENR |= 4;   // Push Buttio Port C pin 13
	
	GPIOA->MODER |= 0x400;  //Set LED as an output
	//PC13 is set an input by default
	
	while(1)
	{
		if(GPIOC->IDR & 0x2000) //If PC 13 is high
		{
			GPIOA->BSRR = 0x002;  //Turn off LED
		}
		
		else
		{
			GPIOA->BSRR = 0x20;   //Turn on LED
		}
	}
		
}
