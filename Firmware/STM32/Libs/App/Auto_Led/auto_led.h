#ifndef APP_AUTO_LED_AUTO_LED_H_
#define APP_AUTO_LED_AUTO_LED_H_

/*********************
 *      INCLUDES
 *********************/

#include "stm32f1xx_hal.h"

/*********************
 *      DEFINES
 *********************/

#define turnOnLight() 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET)
#define turnOffLight() 	    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET)

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

uint8_t autocontrol_mode();

#endif /* APP_AUTO_LED_AUTO_LED_H_ */
