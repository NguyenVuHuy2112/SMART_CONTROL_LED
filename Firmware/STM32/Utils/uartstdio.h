#ifndef __UARTSTDIO_H__
#define __UARTSTDIO_H__

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>
#include "stm32f1xx_hal.h"

/*********************
 *   GLOBAL FUNCTION
 *********************/

void UARTConfig(UART_HandleTypeDef * uart_p, uint32_t rx_buffer_size);
void UARTWrite(char *  data, uint32_t bytes);
uint32_t UARTRead(char * data);

#endif // __UARTSTDIO_H__
