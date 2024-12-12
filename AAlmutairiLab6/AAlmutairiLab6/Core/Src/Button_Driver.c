/*
 * Button_Driver.c
 *
 *  Created on: Sep 30, 2024
 *      Author: ABAlm
 */

#include <Button_Driver.h>

void button_init()
{

	GPIO_InitTypeDef buttonConfig = {0};


	buttonConfig.Pin = USER_Button_pin;
	buttonConfig.Mode = GPIO_MODE_INPUT;
	buttonConfig.Speed = GPIO_SPEED_FREQ_MEDIUM;
	buttonConfig.Pull = GPIO_NOPULL;

	button_clk_enable();
	HAL_GPIO_Init(USER_Button_Port, &buttonConfig);

}

void button_clk_enable()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

}

bool is_button_pressed()
{
	uint8_t var = HAL_GPIO_ReadPin(USER_Button_Port, USER_Button_pin);

	if (var == USER_Button_Pressed)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}


void Interrupt_Button_Init()
{
	GPIO_InitTypeDef buttonConfig = {0};

		buttonConfig.Pin = USER_Button_pin;
		buttonConfig.Mode = GPIO_MODE_IT_RISING;
		buttonConfig.Speed = GPIO_SPEED_FREQ_MEDIUM;
		buttonConfig.Pull = GPIO_NOPULL;


		button_clk_enable();
		HAL_GPIO_Init(USER_Button_Port, &buttonConfig);
		HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
