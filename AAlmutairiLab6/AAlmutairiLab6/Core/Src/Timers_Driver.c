/*
 * Timers_Driver.c
 *
 *  Created on: Dec 3, 2024
 *      Author: ABAlm
 */
#include "Timers_Driver.h"

static uint32_t elapsedSeconds = 0;
static TIM_HandleTypeDef htim7 = {0};

void Timer_Init(void)
{
	__HAL_RCC_TIM7_CLK_ENABLE();


	htim7.Instance = TIM7;
	htim7.Init.Prescaler = (SystemCoreClock /1000) - 1;
	htim7.Init.Period = 3000 -1;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
	{
	        Error_Handler();
	    }

	HAL_NVIC_SetPriority(TIM7_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);

	HAL_TIM_Base_Start_IT(&htim7);
}


uint32_t Timer_GetElapsedSeconds(void)
{
	return elapsedSeconds;
}
void Timer_StartInterruptElapsedTime(void)
{
	elapsedSeconds = 0;

	HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);

	HAL_TIM_Base_Start_IT(&htim7);
}

void Timer_ResetElapsedTime(void)
{
	 elapsedSeconds = 0;
}
void Timer_IncrementElapsedSeconds(void)
{
	elapsedSeconds++;
}

void TIM7_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim7);

   // if (currentScreenState == SCREEN_GAME) {
        static uint32_t tick = 0;
        tick++;
        if (tick >= 3000) {
            tick = 0;
            uint32_t elapsed_Time = Timer_GetElapsedSeconds();
            elapsed_Time++;
       // }
    }
}
