/*
 * Timers_Driver.h
 *
 *  Created on: Dec 3, 2024
 *      Author: ABAlm
 */

#ifndef INC_TIMERS_DRIVER_H_
#define INC_TIMERS_DRIVER_H_


#include "main.h"

// Function Prototypes
void Timer_Init(void);
void Timer_StartInterruptElapsedTime(void);
uint32_t Timer_GetElapsedSeconds(void);
void Timer_ResetElapsedTime(void);
void Timer_IncrementElapsedSeconds(void);
void TIM7_IRQHandler(void);

#endif /* INC_TIMERS_DRIVER_H_ */
