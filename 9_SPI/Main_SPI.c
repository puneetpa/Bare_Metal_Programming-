/*
Program_1: Developing a simple SPI driver
PA7 MOSI
PA5 SCK
PA4 SS

Program_2: Developing an SPI driver for the ST7735 graphics LCD
					 - with HAL library

*/

//#include "stm32f4xx.h"                  // Device header

//////////////////////////////// Start of Program_1 /////////////////////////////////////////////
/*
int main (void)
{
	
}

void SPI_Init(void)
{
	RCC->AHB1ENR |= 1;				// Enable clock to GPIO A
	RCC->APB2ENR |= 0x1000;		// Enable clock to SPI1
	
	GPIOA->MODER &= ~0x0000CC00;	// CLEAR PIN MODE
	GPIOA->MODER |= 0x00008800;
	GPIOA->AFR[0] &= ~0xF0F00000;
	GPIOA->AFR[0] |= 0x50500000;
	
	GPIOA->MODER &= ~0x00000300;
	GPIOA->MODER |= 0x00000100;
	
	// SPI CONFIG
	SPI1->CR1 = 0x31C;				// Set baudrate, t-bt data frame
	SPI1->CR2 = 0;
	SPI1->CR1 |= 0x40;
	
}

void SPI_write_data(unsigned char data)
{
	// Wait until transfer buffer is empty
	while(!(SPI1->SR & 2)){}
	GPIOA->BSRR = 0x00100000;		// Assert slave select
	SPI1->DR = data;						// Write data
	
	// WAIT FOR TRANSMISSION TO BE COMPLETE
	while(!(SPI1->SR & 0x80)){}	
		GPIOA->BSRR = 0x00000010;	// Deselect slave
}
*/
//////////////////////////////// End of Program_1 //////////////////////////////////////////////

//////////////////////////////// Start of Program_2 /////////////////////////////////////////////

#include "adc.h"
#include "GLCD_ST7735.h"

#define YMAX 5000
#define YMIN 0

volatile int sensorValue;

ADC_HandleTypeDef hadc1;
void drawaxes(void);
void drawInfoBar(void);
void plotData(void);

int  main(){

	HAL_Init();
	ADCI_Init();
	ST7735_Init();
	
	   drawaxes();
				 drawInfoBar();

	__enable_irq();
	while(1){
		
				 drawInfoBar();

		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,1);
		sensorValue =  HAL_ADC_GetValue(&hadc1);
		
				 plotData();

	
	}
	
}

void plotData(void)
{
	ST7735_PlotPoint(sensorValue,GREEN);
	ST7735_PlotIncrement();
	
	
}
void drawaxes(void){
    ST7735_Drawaxes(AXISCOLOR, BGCOLOR, "Time", "ADC", LIGHTCOLOR, "", 0, YMAX, YMIN);
}

void drawInfoBar(void)
{
    ST7735_DrawString(1, 0, "CPU =", GREEN);

	    ST7735_DrawString(7, 0, "75%", BLUE );
    ST7735_DrawString(11, 0, "Temp =", GREEN);
   	    ST7735_DrawString(18, 0, "30", BLUE );



}

void SysTick_Handler(void)
{
	HAL_IncTick();

}
//////////////////////////////// End of Program_2 //////////////////////////////////////////////