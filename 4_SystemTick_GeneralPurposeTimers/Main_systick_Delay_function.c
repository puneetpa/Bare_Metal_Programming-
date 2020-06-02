/*
Program: Creating a delay function using 
					the SystemTick Timer
*/

#include "stm32f4xx.h"                  // Device header

//Prototypes
void SysTickDelayMs (int n);

int main (void)
{
	//Enabling Port A
	RCC->AHB1ENR |= 1;							// Clock enable Port A
	GPIOA->MODER |= 0x400;					// Port A5 (LED) set as an output
	
	while(1)
	{
		SysTickDelayMs (50);
		GPIOA->ODR ^= 0x20;
	}
}


//SystemTick Delay Function
void SysTickDelayMs (int n)
{
	SysTick->LOAD = 16000;					// Number of clock cycles in one millisecond
	SysTick->VAL  = 0;							// Clear current value of the timer
	SysTick->CTRL = 0x5;						// Enable SystemTick timer
	
	for(int i = 0; i < n; i++)
	{
		//Wait until the COUNT flag is set
		while( (SysTick->CTRL & 0x10000) == 0)
		{
		}
	}
	SysTick->CTRL = 0;						// Disable SystemTick 
}

