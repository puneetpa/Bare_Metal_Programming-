/*
Program_1: Develop the GPIO Interrupt Driver
					- Toggles LED (PA5) twice when the user input push button (PC13) is pressed
Program_2: Working with multiple GPIO Interrupts
					- Toggles LED (PA5) twice when the user input push button (PC13) is pressed 
					  and once when PB10 push button is pressed.
Program_3: Develop the UART Interrupt Driver

*/

#include "stm32f4xx.h"          // Device header

void SysTickDelayMs (int n);
void UART2_Init(void);
void LED_play(int value);

int pb10_counter = 0;
int pc13_counter = 0;

////////////////////////////////////////// Start of Program_1/////////////////////////////////////////////////
/*
int main(void)
{
	__disable_irq();							// Always disable interrupts before doing anything
	RCC->AHB1ENR |= 1;						// Enable Port A clock (LED) -- PA5
	RCC->AHB1ENR |= 4;						// Enable Port C clock (Push button) -- PC13
	RCC->APB2ENR |= 0x4000;				// Enable SYSCFG clock - bit 14
	
	GPIOA->MODER |= 0x400;				// LED configured as output; button configured as input by default
	SYSCFG->EXTICR[3] = 0x0020;   // Select port C for EXTI13
	
	EXTI->IMR |= 0x2000;					// Unmask EXTI13
	EXTI->FTSR |= 0x2000;					// Select falling edge trigger
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	__enable_irq();								// Enable interrupt 
	
	while(1){}
}

// This interrupt service routine could be anything.
void EXTI15_10_IRQHandler(void)
{
	//Blinking LED twice
	GPIOA->BSRR = 0x20; 					// LED ON
	SysTickDelayMs(300);
	GPIOA->BSRR = 0x00200000;          // LED OFF
	SysTickDelayMs(300);
	GPIOA->BSRR = 0x20; 					// LED ON
	SysTickDelayMs(300);
	GPIOA->BSRR = 0x00200000;          // LED OFF
	SysTickDelayMs(300);
	
	EXTI->PR = 0x2000;						// Clearing interrupt flag
}
*/
////////////////////////////////////////// End of Program_1 ////////////////////////////////////////////

////////////////////////////////////////// Start of Program_2 //////////////////////////////////////////
/*
int main (void)
{
	__disable_irq();							// Always disable interrupts before doing anything
	RCC->AHB1ENR |= 7;						// Enable Port A clock (LED) -- PA5, Port B (PB10) and Port C (PC13)
	RCC->APB2ENR |= 0x4000;				// Enable SYSCFG clock - bit 14
	
	GPIOA->MODER |= 0x400;				// LED configured as output; button configured as input by default
	SYSCFG->EXTICR[3] = 0x0020;   // Select Port C for EXTI13
	
	EXTI->IMR |= 0x2000;					// Unmask EXTI13
	EXTI->FTSR |= 0x2000;					// Select falling edge trigger
	
	GPIOB->PUPDR |= 00100000;			// Pull-up register for PB10
	SYSCFG->EXTICR[2] = 0x0100;   // Select Port B for EXT10
	EXTI->IMR |=0x0400;						// Unmask EXT10
	EXTI->FTSR |=0x0400;					// Select falling edge trigger 
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	__enable_irq();								// Enable interrupt 
	
	while(1){}
}
// IRQ
void EXTI15_10_IRQHandler(void)
{
	//PC13
	if(EXTI->PR ==0x2000)
	{
		//Blinking LED twice
		GPIOA->BSRR = 0x20; 					// LED ON
		SysTickDelayMs(300);
		GPIOA->BSRR = 0x00200000;          // LED OFF
		SysTickDelayMs(300);
		GPIOA->BSRR = 0x20; 					// LED ON
		SysTickDelayMs(300);
		GPIOA->BSRR = 0x00200000;          // LED OFF
		SysTickDelayMs(300);
	
	EXTI->PR = 0x2000;						// Clearing interrupt flag
	pc13_counter++;
	}
	
	else if(EXTI->PR == 0x0400)
	{
		pb10_counter++;
		//Blinking LED once
		GPIOA->BSRR = 0x20; 					// LED ON
		SysTickDelayMs(300);
		GPIOA->BSRR = 0x00200000;          // LED OFF
		SysTickDelayMs(300);
			
		EXTI->PR = 0x0400;						// Clearing interrupt flag
	}
	
}
*/
////////////////////////////////////////// End of Program_2 ////////////////////////////////////////////
//SystemTick Delay Function

////////////////////////////////////////// Start of Program_3 ////////////////////////////////////////////
int main (void)
{
	__disable_irq();							// Always disable interrupts before doing anything
	RCC->AHB1ENR |= 1;						// Enable Port A clock (LED) -- PA5
	
	GPIOA->MODER |= 0x400;				// LED configured as output
	UART2_Init();
	USART2->CR1 |= 0x0020;        // Enable RX interrupt
	NVIC_EnableIRQ(USART2_IRQn);
	__enable_irq();								// Enable interrupt 
	
	while(1){}
}


void UART2_Init(void)
{
	RCC->AHB1ENR |= 1;
	RCC-> APB1ENR |=0x200000;			// Enable UART2 clock
	GPIOA->AFR[0] |= 0x7000;			// Alt7 for UART2
	GPIOA->MODER |= 0x0080;
	
	USART2->BRR = 0x008B;					// 115200 baud rate at 16 Mhz
	USART2->CR1 = 0x0004; 				// Enable receiver
	USART2->CR1 |= 0x2000;				// Enable USART2
}

void LED_play(int value)
{
	value %= 16;
	for(;value>0;value--)
	{
		GPIOA->BSRR = 0x20;					// LED ON
		SysTickDelayMs(100);
		GPIOA->BSRR = 0x00200000;   // LED OFF
		SysTickDelayMs(100);
	}
	SysTickDelayMs(400);
}

void USART2_IRQHandler(void)
{
	char c;
	if(USART2->SR & 0x0020)
	{
		c = USART2->DR;
		LED_play(c);
	}
}

////////////////////////////////////////// End of Program_3 ////////////////////////////////////////////

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



