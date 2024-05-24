#include "illuminance.h"
#include "calibrate_adc.h"
#include "read_adc.h"
#include "math.h"

/*********************
 *      DEFINES
 *********************/

#define LOW_THRESHOLD 10
#define HIGH_THRESHOLD 875

extern ADC_HandleTypeDef hadc2;

/******************************
 *  STATIC PROTOTYPE FUNCTION
 ******************************/
 int16_t myRound(double x);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

uint8_t illuminance_signal(int16_t Ev)
{
	static int previous_State = 0;

	if (Ev < 250)
	{
		previous_State = 1; // Turn the light On
		return 1;
	}
	else if (Ev > 400)
	{
		previous_State = 0; // Turn the light Off
		return 0;
	}
	else
	{
		// if the illumination is from 250 - 400, keep the previous state
		return previous_State;
	}
}

int16_t adjust_Ev(float Ev_before)
{
    int16_t Ev = round((double) Ev_before / 10) * 10;
    if (Ev < LOW_THRESHOLD) {
        Ev = LOW_THRESHOLD;
    } else if (Ev > HIGH_THRESHOLD) {
        Ev = 1000;
    }
    return Ev;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * The function `voltage_adc` reads an ADC value, calibrates it, and calculates the corresponding
 * voltage.
 * 
 * @return The function `voltage_adc()` is returning a floating-point value representing the voltage
 * read from an ADC (Analog-to-Digital Converter) after calibration and conversion.
 */
float voltage_adc()
{
	float volt;
	int16_t var;
	int16_t var_after;
	var = read_adc(&hadc2);
	var_after = calibrate_adc(var);
	volt = (((float)var_after * 3.3) / 4096);
	volt = volt / 6;
	return volt;
}

/**
 * The function `illuminance_adc` calculates illuminance based on the voltage read from an ADC.
 * 
 * @return The function `illuminance_adc()` is returning the calculated illuminance value `Ev_before`.
 */
float illuminance_adc(float volt)
{
	float Ev_before;
	float R = volt * 10; // I=100uA, (The unit of R is KOhm)
	Ev_before = R - 4.6974;
	Ev_before = Ev_before / (-1.02 * 10e-4);
	Ev_before = Ev_before / 5;
	if (Ev_before < 800)	Ev_before = Ev_before - 500;
	return Ev_before;
}

/**
 * The function `myRound` rounds a double value to the nearest integer using a simple method.
 * 
 * @param x The function `myRound` takes a `double` value `x` as input and rounds it to the nearest
 * integer using the conventional rounding method (rounding halves away from zero). The result is then
 * cast to an `int16_t` type before being returned.
 * 
 * @return The function `myRound` takes a `double` input `x`, rounds it to the nearest integer, and
 * returns the result as a 16-bit signed integer (`int16_t`).
 */
int16_t myRound(double x)
{
    if (x >= 0) {
        return (int16_t)(x + 0.5);
    } else {
        return (int16_t)(x - 0.5);
    }
}
