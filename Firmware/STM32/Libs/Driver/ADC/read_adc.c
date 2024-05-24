#include "read_adc.h"


int16_t read_adc (ADC_HandleTypeDef* hadc)
{
    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, 10);
    int16_t var = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);
    return var;
}

void temperature_sensor_enable(int16_t value_adc , ADC_HandleTypeDef* hadc)
{
    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
    value_adc = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);
}
