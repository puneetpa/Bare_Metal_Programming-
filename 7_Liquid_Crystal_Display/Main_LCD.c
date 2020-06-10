/*
Program_1: Developing the Hitachi HD44780 Liquid Crystal Display (LCD) Driver
Pin out: PC0 - PC 7 = D0 - D7
PB5 = RS
PB6 = R/W
PB7 = EN
*/

#include "stm32f4xx.h"                  // Device header
#include <stdio.h>

#define RS 0x20
#define RW 0x40
#define EN 0x80


//Prototypes
void delayMs(int delay);
void LCD_Init(void);
void GPIO_Init(void);
void LCD_command( unsigned char command);
void LCD_Data(char data);

int main(void)
{
	LCD_Init();
	
	while(1)
	{
		LCD_Data('H');
		LCD_Data('E');
		LCD_Data('L');
		LCD_Data('L');
		LCD_Data('O');
		LCD_command(1); 		// Clear command
		delayMs(500);
	}

}	

void LCD_Init(void)
{
	GPIO_Init();
	
	// Init Sequence
	delayMs(30);
	LCD_command(0x30);
	delayMs(10);
	LCD_command(0x30);
	delayMs(1);
	LCD_command(0x30);
	
	LCD_command(0x38); 				// Set 8-bit data mode, 2-line, 5x7 font
	LCD_command(0x06);				// Movie cursor right
	LCD_command(0x01);				// Clear screen, move cursor home
	LCD_command(0x0F);				// Turn display, blink cursor
}	


void GPIO_Init(void)
{
	RCC->AHB1ENR |= 0x06;									// Enable Port B and C clocks
	GPIOB->MODER |= 0x5400;								// Set as output
	GPIOB->BSRR = 0x00C;									// Set EN and RW low
	
	//Set PC0- PC7 as output
	GPIOC->MODER |= 0x00005555;
}

void LCD_command( unsigned char command)
{
	GPIOB->BSRR = (RS|RW) << 16;					// RS = 0 and R/W = 0
	GPIOC->ODR = command;
	GPIOB->BSRR = EN;
	delayMs(0);
	GPIOB->BSRR = EN <<16;								//  Clear EN
	
}


void LCD_Data(char data)
{
	GPIOB->BSRR = RS;											// RS = 1;
	GPIOA->BSRR = RW << 16;								// R/W = 0
	GPIOC->ODR = data;
	GPIOB->BSRR = EN;
	delayMs(0);
	GPIOB->BSRR = EN << 16;								// Clear EN
}

void delayMs(int delay)
{
	int i;
	for(;delay>0;delay--)
	{
		for(i=0; i<3195;i++)
		{
		}
	}
}


