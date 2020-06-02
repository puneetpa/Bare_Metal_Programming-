/*
Program_1: Develop the general purpose timer (TIM2) driver
Program_2: Configure the general purpose timer (TIM2) for output compare
						- Toggle LED at 1 Hz using TIM2 compare mode
Program_3: Configure the general purpose timer (TIM2) for input capture
						- Capture time (TIM3 - PA6) when LED (TIM2 - PA5) toggles
Program_4: Use input capture to measure frequency and period of input signal
						- Capture frequency and period of output signal (LED - PA5) at PA6
						
Program_5: Configure the General Purpose Timer for PWM output

*/

#include "stm32f4xx.h"					// Device header

int timeStamp = 0;
int period;
float frequency;

///////////////////// Start of Program_1 /////////////////////////
/*
int main (void)
{
	RCC->AHB1ENR |= 1;						// Enable clock to AHB1 bus			
	GPIOA->MODER |= 0x400;			  // Set LED as an output
	
	//Timer 2 (TIM2) Configuration to achieve 1 Hz cycle frequency
	RCC->APB1ENR |= 1;						// Enable TIM2
	TIM2->PSC = 1600 - 1;					// 16 MHz divided by 1600 = 10,000
	TIM2->ARR = 10000 - 1;        // 10,000 divided by 10,000 = 1 Hz output frequency
	TIM2->CNT = 0;								// Counter set to 0
	TIM2->CR1 = 1;								// Enable Tim2
	
	while(1)
	{
		while(!(TIM2->SR & 1))
		{
		}
		TIM2->SR &= ~1;
		GPIOA->ODR ^= 0x20;
	}
}
*/
///////////////// End of Program_1 ///////////////////////////////////////////////////////


///////////////// Start of Program_2 /////////////////////////////////////////////////////

//Toggle LED at 1 Hz using TIM2 compare mode
/*
int main (void)
{
	RCC->AHB1ENR |= 1;						// Enable clock to AHB1 bus			
	GPIOA->MODER |= 0x800;			  // Set PA5 to alternative function
	GPIOA->AFR[0] |=0x00100000;		// Set pin AF1 for TIM2
	
	RCC->APB1ENR |= 1;						// Enable TIM2
	TIM2->PSC = 1600 - 1;					// 16 MHz divided by 1600 = 10,000
	TIM2->ARR = 10000 - 1;        // 10,000 divided by 10,000 = 1 Hz output frequency
	TIM2->CCMR1 = 0x30;						// Set output to toggle on match
	TIM2->CCR1 = 0;								// Set match mode
	TIM2->CCER |= 1;							// Enable channel 1 compare mode
	TIM2->CNT = 0;								// Counter set to 0
	TIM2->CR1 = 1;								// Enable Tim2
	
	
	while(1)
	{
	}
}
*/
///////////////// End of Program_2 ///////////////////////////////////////////////////////

///////////////// Start of Program_3 /////////////////////////////////////////////////////
/*
int main (void)
{
	RCC->AHB1ENR |= 1;						// Enable clock to AHB1 bus			
	GPIOA->MODER |= 0x800;			  // Set PA5 to alternative function
	GPIOA->AFR[0] |=0x00100000;		// Set pin AF1 for TIM2
	
	RCC->APB1ENR |= 1;						// Enable TIM2
	TIM2->PSC = 1600 - 1;					// 16 MHz divided by 1600 = 10,000
	TIM2->ARR = 10000 - 1;        // 10,000 divided by 10,000 = 1 Hz output frequency
	TIM2->CCMR1 = 0x30;						// Set output to toggle on match
	TIM2->CCR1 = 0;								// Set match mode
	TIM2->CCER |= 1;							// Enable channel 1 compare mode
	TIM2->CNT = 0;								// Counter set to 0
	TIM2->CR1 = 1;								// Enable Tim2
	
	//Configure PA6 as input of TIM3 CH1
	RCC->AHB1ENR |= 1;						// Enable clock to AHB1 bus
	GPIOA->MODER |= 0x2000;				// SET PA6 to alternate function
	GPIOA->AFR[0] |=0x02000000;		// Set pin to AF2 for TIM3 CH1
	
	//Config TIM3
	RCC->APB1ENR |= 2;						// Enable clock to APB1 bus
	TIM3->PSC = 16000;						// Divide by 16000
	TIM3->CCMR1 = 0x41;						// Set CH1 to capture at every edge
	TIM3->CCER = 1;								// Capture mode enabled
	TIM3->CR1 = 1;								// TIM3 enabled
	
	while(1)
	{
		while(!(TIM3->SR &2))
		{
		}
		timeStamp = TIM3->CCR1;
	}
}
*/
///////////////// End of Program_3 ///////////////////////////////////////////////////////

///////////////// Start of Program_4 /////////////////////////////////////////////////////
/*
int main (void)
{
	int last_value = 0;
	int current_value = 0;
	
	RCC->AHB1ENR |= 1;						// Enable clock to AHB1 bus			
	GPIOA->MODER |= 0x800;			  // Set PA5 to alternative function
	GPIOA->AFR[0] |=0x00100000;		// Set pin AF1 for TIM2
	
	RCC->APB1ENR |= 1;						// Enable TIM2
	TIM2->PSC = 1600 - 1;					// 16 MHz divided by 1600 = 10,000
	TIM2->ARR = 10000 - 1;        // 10,000 divided by 10,000 = 1 Hz output frequency
	TIM2->CCMR1 = 0x30;						// Set output to toggle on match
	TIM2->CCR1 = 0;								// Set match mode
	TIM2->CCER |= 1;							// Enable channel 1 compare mode
	TIM2->CNT = 0;								// Counter set to 0
	TIM2->CR1 = 1;								// Enable Tim2
	
	//Configure PA6 as input of TIM3 CH1
	RCC->AHB1ENR |= 1;						// Enable clock to AHB1 bus
	GPIOA->MODER |= 0x2000;				// SET PA6 to alternate function
	GPIOA->AFR[0] |=0x02000000;		// Set pin to AF2 for TIM3 CH1
	
	//Config TIM3
	RCC->APB1ENR |= 2;						// Enable clock to APB1 bus
	TIM3->PSC = 16000-1;						// Divide by 16000
	TIM3->CCMR1 = 0x41;						// Set CH1 to capture at every edge
	TIM3->CCER = 0x0B;								// Capture mode enabled
	TIM3->CR1 = 1;								// TIM3 enabled
	
	while(1)
	{
		//Wait until edge is captured
		while(!(TIM3->SR &2))
		{
		}
		//timeStamp = TIM3->CCR1;
		current_value = TIM3->CCR1;
		period = current_value - last_value;
		last_value = current_value;
		frequency = 1000.0f/period;
	}
}
*/
///////////////// End of Program_4 ///////////////////////////////////////////////////////

///////////////// Start of Program_5 /////////////////////////////////////////////////////

//Configuring TIM2 for PWM output
int main (void)
{
	RCC->AHB1ENR |= 1;						// Enable clock to AHB1 bus			
	GPIOA->MODER |= 0x800;			  // Set PA5 to alternative function
	GPIOA->AFR[0] |=0x00100000;		// Set pin AF1 for TIM2
	
	RCC->APB1ENR |= 1;						// Enable TIM2
	TIM2->PSC = 10 - 1;					  // 16 MHz divided by 10
	TIM2->ARR = 26667 - 1;        // Divide by 26667
	TIM2->CCMR1 = 0x0060;					// Enable PWM mode
	TIM2->CNT = 0;								// Counter set to 0
	TIM2->CCER = 1;								// Enable PWM ch 1
	TIM2->CCR1 = 8889-1;					// Pulse width 1/3 the period
	TIM2->CR1 = 1;								// Enable Tim2
		
	while(1)
	{
	}
}

///////////////// End of Program_5 ///////////////////////////////////////////////////////


