/*
 * RNG_Driver.h
 *
 *  Created on: Dec 2, 2024
 *      Author: ABAlm
 */

#ifndef INC_RNG_DRIVER_H_
#define INC_RNG_DRIVER_H_

#include "stm32f4xx_hal.h"
#include "stdint.h"


// initialize RNG
void RNG_init();

// Generate Random Number
uint32_t RNG_GetRAND_Num();

// Generate a Random number within a range
//uint32_t RNG_GetRAND_Range(uint32_t min, uint32_t max);

#endif /* INC_RNG_DRIVER_H_ */
