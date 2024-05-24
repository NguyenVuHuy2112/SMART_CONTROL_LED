#ifndef DELAY_TIMER_H_
#define DELAY_TIMER_H_

#include "main.h"

void DELAY_Tim_Init(TIM_HandleTypeDef *htim);

void DELAY_Tim_Us(TIM_HandleTypeDef *htim, uint16_t time_us);

void DELAY_Tim_Ms(TIM_HandleTypeDef *htim, uint16_t time_ms);

#endif /* DELAY_TIMER_H_ */