

#ifndef DRIVER_ADC_READ_ADC_H_
#define DRIVER_ADC_READ_ADC_H_

/***********************************************************************/

#include "stm32f1xx_hal.h"
#include "stdint.h"

int16_t read_adc(ADC_HandleTypeDef* hadc);
void temperature_sensor_enable(int16_t value_adc , ADC_HandleTypeDef* hadc);
/***********************************************************************/

#endif /* DRIVER_ADC_READ_ADC_H_ */
