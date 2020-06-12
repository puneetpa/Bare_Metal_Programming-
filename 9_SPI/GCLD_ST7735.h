#ifndef __GCLD_ST7735
#define __GCLD_ST7735

#include <stdint.h>
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common


#define LCD_RST1 HAL_GPIO_WritePin(GPIOB_PIN_5,GPIO_PIN_SET)
#define LCD_RST0 HAL_GPIO_WritePin(GPIOB_PIN_5,GPIO_PIN_RESET)

#define LCD_DC1 HAL_GPIO_WritePin(GPIOB_PIN_4,GPIO_PIN_SET)
#define LCD_DC0 HAL_GPIO_WritePin(GPIOB_PIN_4,GPIO_PIN_RESET)

#define LCD_CS1 HAL_GPIO_WritePin(GPIOB_PIN_3,GPIO_PIN_SET)
#define LCD_CS0 HAL_GPIO_WritePin(GPIOB_PIN_3,GPIO_PIN_RESET)

#endif