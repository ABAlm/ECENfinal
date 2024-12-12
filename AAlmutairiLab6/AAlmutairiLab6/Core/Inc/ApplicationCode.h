/*
 * ApplicationCode.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Xavion
 */

#include "LCD_Driver.h"
#include "stm32f4xx_hal.h"
#include "RNG_Driver.h"
#include "Tetris_Game.h"
#include "Timers_Driver.h"
#include "Button_Driver.h"
#include <stdio.h>


#ifndef INC_APPLICATIONCODE_H_
#define INC_APPLICATIONCODE_H_

void ApplicationInit(void);
void LCD_Visual_Demo(void);
void DrawMainMenuScreen(void);
void DrawGameScreen(void);
void DrawGameOverScreen(uint32_t elapsedTime);
typedef enum {
    SCREEN_MAIN_MENU,
    SCREEN_GAME,
    SCREEN_GAME_OVER
} ScreenState;





#if (COMPILE_TOUCH_FUNCTIONS == 1) && (COMPILE_TOUCH_INTERRUPT_SUPPORT == 0)
void LCD_Touch_Polling_Demo(void);
#endif // (COMPILE_TOUCH_FUNCTIONS == 1) && (COMPILE_TOUCH_INTERRUPT_SUPPORT == 0)

#endif /* INC_APPLICATIONCODE_H_ */
