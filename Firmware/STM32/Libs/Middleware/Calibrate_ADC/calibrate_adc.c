/*********************
 *      INCLUDES
 *********************/

#include "calibrate_adc.h"

/**
 * The function calibrate_adc takes an input variable, applies a calibration formula, and returns the
 * calibrated value.
 * 
 * @param var The `var` parameter in the `calibrate_adc` function is an `int16_t` variable that
 * represents the input value to be calibrated.
 * 
 * @return The function `calibrate_adc` returns a variable of type `int16_t` which is the result of the
 * calculation `(float)var * 0.9769 + 157.13`.
 */
int16_t calibrate_adc(int16_t var)
{
	int16_t var_after = (float)var * 0.9769 + 157.13;
	return var_after;
}