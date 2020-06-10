/*
Program_1: Developing an I2C driver for the DS1307 realtime clock
SCL - PB8
SDA - PB9
*/

#include "stm32f4xx.h"                  // Device header
#define SLAVE_ADDR 0x68									// DS1307 address


void delayMs (int n);
int I2C_readbyte(char saddr, char maddr, char *data);
void I2C_Init(void);


int main (void)
{
	I2C_Init();
	RCC->AHB1ENR |= 1;
	GPIOA->MODER &= ~0x00000C00;
	GPIOA->MODER |= 0x00000400;
	char data;
	while(1)
	{
		I2C_readbyte(SLAVE_ADDR,0,&data);
		
		if(data &1)
		{
			GPIOA->ODR |= 0x00000020;
		}
		else
			GPIOA->ODR |= ~0x00000020;
	  
		delayMs(10);
	}
}	

void I2C_Init(void)
{
	RCC->AHB1ENR |= 2;
	RCC->APB1ENR |= 0X00200000;		// Enable clock to I2C
	
	GPIOB->MODER &= ~0X000F0000;	// PB8 and PB9 use alternate function
	GPIOB->MODER |= 0x000A0000;
	
	GPIOB->AFR[1] &= ~0x000000FF; // Deselecting
	GPIOB->AFR[1] |= 0x00000044;	// Selecting
	
	GPIOB->OTYPER |= 0x00000300;	// Set as open-drain
	
	GPIOB->PUPDR &= ~0x000F0000;	// Pull-ups
	GPIOB->PUPDR |= 0x00050000;   
	
	//I2C Config
	I2C1->CR1 = 0x8000;						// Software reset
	I2C1->CR1 &= ~0x8000;
	I2C1->CR2 = 0x0010;
	I2C1->CCR = 80; 							// standard mode, 100 kHz clock
	I2C1->TRISE = 17; 						// mx rise time
	I2C1->CR1 |= 0x0001;
}

int I2C_readbyte(char saddr, char maddr, char *data)
{
	volatile int tmp;
	
	//Make sure bus is not busy
	while(I2C1->SR2 & 2)
	{}
	
	I2C1->CR1|= 0x100;						// Start
	while(!(I2C1->SR1 & 1))
	{};
		
	I2C1->DR = saddr << 1;
		while(!(I2C1->SR1 & 2)) 
		{};
		tmp = I2C1->SR2;
			
	while(!(I2C1->SR1 &0x80))
	{};
		I2C1->DR = maddr;
		
 I2C1->CR1 |= 0x100;
		while(!(I2C1->SR1 &1))
		{};
		I2C1->DR = saddr << 1 | 1;
			
	while(!(I2C1->SR1 & 2))
		{};
			I2C1->CR1 &= ~0x400;
		tmp =  I2C1->SR2;
			
	I2C1->CR1 |= 0x200;
			
			while(!(I2C1->SR1 &0x40))
			{};
			*data++ = I2C1->DR;
				
	return 0;
}

void delayMs (int n)
{
	for(; n>0;n--)
	{
		for(int i = 0; i <3195; i++)
		{};
	}
}	

