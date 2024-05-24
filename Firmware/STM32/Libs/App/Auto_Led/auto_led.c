/*********************
 *      INCLUDES
 *********************/

#include "auto_led.h"
#include "illuminance.h"

extern int16_t lux;

/**
 * The function `autocontrol_mode` reads voltage and illuminance values, adjusts illuminance, and
 * controls a light based on the illuminance signal.
 * 
 * @return The `autocontrol_mode()` function returns an unsigned 8-bit integer (`uint8_t`). It returns
 * `1` if the illuminance signal is equal to 1, indicating that the light should be turned on.
 * Otherwise, it returns `0`, indicating that the light should be turned off.
 */
uint8_t autocontrol_mode()
{
	float volt = voltage_adc();
	float ev_before = illuminance_adc(volt);
  	lux = adjust_Ev(ev_before);
	if (illuminance_signal(lux) == 1)
	{
		turnOnLight();
		return 1;
	}

	else
	{
		turnOffLight();
		return 0;
	}
}
