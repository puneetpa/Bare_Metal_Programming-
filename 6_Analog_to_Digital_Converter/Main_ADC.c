/*
Program_1:	Configure ADC CH1
						- Read values of a potentiometer at CH1.
						
Program_2: Reading the CPU temperature with the ADC and displaying it using UART
*/

#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
int analogValue;
int data;
double voltage;
double celsius;

//Prototypes
void USART2_Init(void);
int USART2_write(int ch);

////////////////////////////////////////// Start of Program_1 //////////////////////////////////////////
/*
int main(void)
{
	RCC->AHB1ENR |= 1;										// ADC1 connected to PA1
	GPIOA->MODER |= 0xC;									// Set PA1 as analog pin
	RCC->APB2ENR |= 0x100;								// Enable ADC1 clock
	ADC1->CR2 = 0;												// Turn ADC OFF]
	ADC1->SQR3 = 1;												// Number of conversions
	ADC1->CR2 |=1;												// Enable ADC1
	
	
	while(1)
	{
		ADC1->CR2 |= 0x40000000;
		//Wait for conversion to be complete
		while(!(ADC1->SR & 2))
		{
			analogValue = ADC1->DR;
		}
	}
	
}
*/
////////////////////////////////////////// End of Program_1 //////////////////////////////////////////

////////////////////////////////////////// Start of Program_2 //////////////////////////////////////////

int main(void)
{
	RCC->AHB1ENR |=1;
	
	// Configure Timer 2 to 1 Hz
	RCC->APB1ENR |=1;
	TIM2->PSC = 1600 - 1;
	TIM2->ARR = 10000 - 1;
	TIM2->CNT = 0;
	TIM2->CCMR1 = 0x6800;
	TIM2->CCER = 0x0010;
	TIM2->CCR2 = 50-1;
	TIM2->CR1 = 1;
	
	// ADC setup
	RCC->APB2ENR |= 0x100;
	ADC->CCR |= 0x00800000;
	ADC->CCR &= ~0x00400000; 				//VBATE must be disabled for temp sensor
	
	ADC1->SMPR1 =0x04000000;				// Sampling time minimun 10 us
	ADC1->SQR3 = 18;
	ADC1->CR2 = 0x13000000;
	ADC1->CR2 |=1;
	
	USART2_Init();
	
	printf("STM32F4 Temperature\r\n");
	
	while(1)
	{
		while(!(ADC1->SR))
		{
		}
		data = ADC1->DR;
		voltage = (double)data/4095*3.3;
		celsius = (voltage-0.76)/0.0025+25;
		
		printf("%d, %.2f\370C\r\n",data, celsius);
	}
	
}

void USART2_Init(void)
{
	RCC->AHB1ENR 			|=1;															//Clock enable for GPIO Port A
	RCC->APB1ENR 			|=0x20000;												//Clock enable for APB1 bus
	GPIOA->AFR[0]			|=0x7700;													//Enabled PA3 for Rx (AF7) and PA2 for Tx (AF7)
	GPIOA->MODER      |=0x00A0;													//ENABLE alternate function for PA2 (Tx) and PA3 (Rx)
	
	USART2->BRR				 =0x0683;													//9600 baud rate @ 16MHz
	USART2->CR1				 =0x000C;													//ENABLE Tx, Rx
	USART2->CR1       |=0x2000;													//ENABLE USART
}

struct __FILE{int handle;};
FILE __stdout = {1};
int fputc(int c, FILE *f)
{
	return USART2_write(c);
}

int USART2_write(int ch)
{
	while(!(USART2->SR & 0x0080)){}											//Wait for Tx buffer empty
	USART2->DR = (ch & 0xFF);
	return ch;
}

////////////////////////////////////////// End of Program_2 //////////////////////////////////////////

