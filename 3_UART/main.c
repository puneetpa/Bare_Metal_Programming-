// Program: USART driver -> bidirectional 

//////////////////          Additional Notes                ///////////////////////

// USART2 connected to APB1 bus
// USART2 connected to PA2
// Clock enable (bit 17): RCC_APB1ENR = 0x00020000
// Registers of interest: Baud rate and control
// Baud_rate:  USART_BRR
// Control Register 1 USART_CR1: Enable usart, TE and RE
// USART is a special purpose IO -> Thus enable alternate function
// AFRL (AF7 = USART1 and USART2) 


#include "stm32f4xx.h"                  // Device header
#include <stdio.h>



//Prototypes
void USART2_Init(void);
int USART2_write(int ch);
int USART2_read(void);
void delayMs(int delay);



int main(void)
{
	USART2_Init();
	int n;
	char str[100];
	printf("Hello from the other side\r\n");
	fprintf(stdout, "test for stdout\r\n");
	fprintf(stderr, "test for stderr\r\n");
	
	while(1)
	{
		printf("How old are you? ");
		scanf("%d", &n);
		printf("Your age is: %d\r\n",n);
		printf("Enter your first name :");
		gets(str);
		printf("I like your style: ");
		puts(str);
		printf("\r\n");
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

int USART2_write(int ch)
{
	while(!(USART2->SR & 0x0080)){}											//Wait for Tx buffer empty
	USART2->DR = (ch & 0xFF);
	return ch;
}

int USART2_read(void)
{
	while(!(USART2->SR & 0x0020)){}											//Wait for Rx buffer empty
		return USART2->DR;
}


//Interface to stdio library
struct __FILE{int handle;};
FILE __stdin = {0};
FILE __stdout = {1};
FILE __stderr = {2};

int fgetc(FILE *f)
{
	int c;
	c = USART2_read();
	if (c == '\r')
	{
		USART2_write(c);
		c = '\n';
	}
	USART2_write(c);
	
	return c;
}

int fputc(int c, FILE *f)
{
	return USART2_write(c);
}




void delayMs(int delay)
{
	int i;
	for(; delay > 0; delay--)
	{
		for(i = 0; i < 3195; i++);
	}
}




