/*
 * RNG_Driver.c
 *
 *  Created on: Dec 2, 2024
 *      Author: ABAlm
 */

#include "RNG_Driver.h"


static RNG_HandleTypeDef hrng;

// initialize RNG
void RNG_init()
{

	__HAL_RCC_RNG_CLK_ENABLE();

	hrng.Instance = RNG;

	HAL_RNG_Init(&hrng);
}

// Generate Random Number
uint32_t RNG_GetRAND_Num()
{
	uint32_t RANDNum = 0;
	HAL_RNG_GenerateRandomNumber(&hrng, &RANDNum);

	return RANDNum % 7;
}

// Generate a Random number within a range
//uint32_t RNG_GetRAND_Range(uint32_t min, uint32_t max)
//{
//	uint32_t RANDNum = RNG_GetRAND_Num();

	//uint32_t scaledRANDNum = (RANDNum % max - min +1) + min;

	//return scaledRANDNum;

//}

