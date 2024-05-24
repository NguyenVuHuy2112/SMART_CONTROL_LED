#ifndef APP_ILLUMINANCE_ILLUMINANCE_H_
#define APP_ILLUMINANCE_ILLUMINANCE_H_
/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

uint8_t illuminance_signal(int16_t Ev);
int16_t adjust_Ev(float Ev_before);
float illuminance_adc(float volt);
float voltage_adc();


#endif /* APP_ILLUMINANCE_ILLUMINANCE_H_ */
