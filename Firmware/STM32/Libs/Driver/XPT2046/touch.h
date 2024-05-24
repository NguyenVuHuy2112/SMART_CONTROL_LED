#ifndef DRIVER_XPT2046_TOUCH_H_
#define DRIVER_XPT2046_TOUCH_H_

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>
#include <stdbool.h>

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void TouchCalibrate(void);
bool TouchIsTouched(void);
bool TouchGetCalibratedPoint(int16_t* x, int16_t* y);

#endif /* DRIVER_XPT2046_TOUCH_H_ */
