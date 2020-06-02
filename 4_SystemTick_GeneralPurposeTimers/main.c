/*
Program: Developing the SystemTick 
				 Timer driver. 
*/



#include "stm32f4xx.h"            // Device header

int main (void)
{
	//Enabling Port A
	RCC->AHB1ENR |= 1;							// Clock enable Port A
	GPIOA->MODER |= 0x400;					// Port A5 (LED) set as an output
	
	//Configure SysTick to generate 200 ms delay
	SysTick->LOAD = 3200000 - 1;		// Load 200 ms delay to load register
	SysTick->VAL  = 0;							// Current value of Systick downcounter
  SysTick->CTRL	= 5;							// Enable systick and set it as a processor clock
	
	while(1)
	{
		//Check count flag of the control register
		if(SysTick->CTRL & 0x10000)
		{
			GPIOA->ODR ^= 0x20;					//Using bitwise exclusive or operation to toggle LED
		}
	}
}

