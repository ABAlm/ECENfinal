/*
 * Button_Driver.h
 *
 *  Created on: Sep 30, 2024
 *      Author: ABAlm
 */

#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

#include  "stm32f4xx_hal.h"
#include <stdbool.h>
#define USER_Button_Port	GPIOA
#define USER_Button_pin		GPIO_PIN_0
#define USER_Button_Pressed 1
#define USER_Button_Not_pressed	0

 void button_init();
 void button_clk_enable();
 bool is_button_pressed();
 void Interrupt_Button_Init();


#endif /* BUTTON_DRIVER_H_ */
