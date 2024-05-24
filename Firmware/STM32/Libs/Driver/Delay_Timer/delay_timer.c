#include "delay_timer.h"

/**
 * This function initializes the timer used for the delay function
 *
 * @param htim pointer to the timer handle
 */
void DELAY_Tim_Init(TIM_HandleTypeDef *htim)
{
    HAL_TIM_Base_Start(htim);
}

/**
 * The function takes a pointer to a timer handle and a time in microseconds as arguments. It starts
 * the timer, sets the counter to zero, and waits until the counter reaches the time in microseconds.
 * Then it stops the timer
 *
 * @param htim pointer to the timer handle
 * @param time_us the time in microseconds that you want to delay
 */
void DELAY_Tim_Us(TIM_HandleTypeDef *htim, uint16_t time_us)
{
    HAL_TIM_Base_Start(htim);
    htim->Instance->CNT = 0;
    while ((htim->Instance->CNT) < time_us)
        ;
    HAL_TIM_Base_Stop(htim);
}

/**
 * A function that delays the program for a certain amount of time.
 *
 * @param htim pointer to the timer handle
 * @param time_ms The time in milliseconds to delay.
 */
void DELAY_Tim_Ms(TIM_HandleTypeDef *htim, uint16_t time_ms)
{
    while (time_ms--)
    {
        DELAY_Tim_Us(htim, 1000);
    }
}