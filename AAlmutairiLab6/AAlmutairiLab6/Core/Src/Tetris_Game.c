/*
 * Tetris_Game.c
 *
 *  Created on: Dec 2, 2024
 *      Author: ABAlm
 */

#include "Tetris_Game.h"




uint32_t Tetris_gamecurrentScreen[HEIGHT_SIZE][WIDTH_SIZE] = {0};
uint32_t Tetris_nextGrid[HEIGHT_SIZE][WIDTH_SIZE] = {0};
uint32_t Tetris_gameScreenclear[HEIGHT_SIZE][WIDTH_SIZE] = {0};
uint8_t Xpos = 6;
uint8_t Ypos = 0;
uint8_t Rotation_at_currentScreen = 0;
uint32_t Tetris_currentTetromino[TETROMINO_SIZE][TETROMINO_SIZE];
char timePlayedtoDisplay[10] = {'\0'};
static uint32_t timeFromStart = 0;
static uint32_t timeGameOver = 0;
static uint32_t Timecounted = 0;
uint32_t Tetris_clearTetromino[TETROMINO_SIZE][TETROMINO_SIZE] = {0};
uint8_t randomNum = 0;

 uint32_t tetrominoShapes[7][4][TETROMINO_SIZE][TETROMINO_SIZE] =
{
		{
		// Tetromino_I
		{
				{0,0,0,0},
				{1,1,1,1},
				{0,0,0,0},
				{0,0,0,0}
		},
		// Rotation 1
		{
				{0,0,1,0},
				{0,0,1,0},
				{0,0,1,0},
				{0,0,1,0}
		},
		// Rotation 2
		{
				{0,0,0,0},
				{0,0,0,0},
				{1,1,1,1},
				{0,0,0,0}
		},
		// Rotation 3
		{
				{0,1,0,0},
				{0,1,0,0},
				{0,1,0,0},
				{0,1,0,0}
		}
	},

	 // TETROMINO_O
	    {

	        {
	            {0, 0, 0, 0},
	            {0, 1, 1, 0},
	            {0, 1, 1, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 1
	        {
	            {0, 0, 0, 0},
	            {0, 1, 1, 0},
	            {0, 1, 1, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 2
	        {
	            {0, 0, 0, 0},
	            {0, 1, 1, 0},
	            {0, 1, 1, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 3
	        {
	            {0, 0, 0, 0},
	            {0, 1, 1, 0},
	            {0, 1, 1, 0},
	            {0, 0, 0, 0}
	        }
	    },
	    // TETROMINO_T
	    {
	        // Rotation 0
	        {
	            {0, 0, 0, 0},
	            {1, 1, 1, 0},
	            {0, 1, 0, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 1
	        {
	            {0, 1, 0, 0},
	            {1, 1, 0, 0},
	            {0, 1, 0, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 2
	        {
	            {0, 1, 0, 0},
	            {1, 1, 1, 0},
	            {0, 0, 0, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 3
	        {
	            {0, 1, 0, 0},
	            {0, 1, 1, 0},
	            {0, 1, 0, 0},
	            {0, 0, 0, 0}
	        }
	    },
	    // TETROMINO_S
	    {
	        // Rotation 0
	        {
	            {0, 0, 0, 0},
	            {0, 1, 1, 0},
	            {1, 1, 0, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 1
	        {
	            {1, 0, 0, 0},
	            {1, 1, 0, 0},
	            {0, 1, 0, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 2
	        {
	            {0, 0, 0, 0},
	            {0, 1, 1, 0},
	            {1, 1, 0, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 3
	        {
	            {1, 0, 0, 0},
	            {1, 1, 0, 0},
	            {0, 1, 0, 0},
	            {0, 0, 0, 0}
	        }
	    },
	    // TETROMINO_Z
	    {

	        {
	            {0, 0, 0, 0},
	            {1, 1, 0, 0},
	            {0, 1, 1, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 1
	        {
	            {0, 1, 0, 0},
	            {1, 1, 0, 0},
	            {1, 0, 0, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 2
	        {
	            {0, 0, 0, 0},
	            {1, 1, 0, 0},
	            {0, 1, 1, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 3
	        {
	            {0, 1, 0, 0},
	            {1, 1, 0, 0},
	            {1, 0, 0, 0},
	            {0, 0, 0, 0}
	        }
	    },
	    // TETROMINO_J
	    {
	        // Rotation 0
	        {
	            {0, 0, 0, 0},
	            {1, 1, 1, 0},
	            {0, 0, 1, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 1
	        {
	            {0, 1, 0, 0},
	            {0, 1, 0, 0},
	            {1, 1, 0, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 2
	        {
	            {1, 0, 0, 0},
	            {1, 1, 1, 0},
	            {0, 0, 0, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 3
	        {
	            {0, 1, 1, 0},
	            {0, 1, 0, 0},
	            {0, 1, 0, 0},
	            {0, 0, 0, 0}
	        }
	    },
	    // TETROMINO_L
	    {
	        // Rotation 0
	        {
	            {0, 0, 0, 0},
	            {1, 1, 1, 0},
	            {1, 0, 0, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 1
	        {
	            {1, 1, 0, 0},
	            {0, 1, 0, 0},
	            {0, 1, 0, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 2
	        {
	            {0, 0, 1, 0},
	            {1, 1, 1, 0},
	            {0, 0, 0, 0},
	            {0, 0, 0, 0}
	        },
	        // Rotation 3
	        {
	            {0, 1, 0, 0},
	            {0, 1, 0, 0},
	            {0, 1, 1, 0},
	            {0, 0, 0, 0}
	        }
	    }
};

 void Tetris_GameLoop()
 {
 	 while (1) {
 	    	Tetris_Draw_Grid(Tetris_nextGrid);
 	        if (Tetris_CheckCollision())
 	        {

 	            Tetris_PlaceTetromino(Tetris_gamecurrentScreen);


 	            memcpy(Tetris_nextGrid, Tetris_gamecurrentScreen, sizeof(Tetris_gamecurrentScreen));



 	            Ypos = 0;

 	            Rotation_at_currentScreen = 0;
 	            Tetris_GenerateTetromino();


 	            if (Tetris_CheckCollision()) {

 	                Tetris_PlaceTetromino(Tetris_gamecurrentScreen);
 	                LCD_TetrisDrawGrid();
 	                break;

 	            }
 	        } else {

 	            memcpy(Tetris_nextGrid, Tetris_gamecurrentScreen, sizeof(Tetris_gamecurrentScreen));
 	            Tetris_MoveBlockDown();
 	        }


 	        HAL_Delay(1000);
 	    }
 }

 void TimePlaying(uint32_t timeForGameOver, uint32_t time_Start) {
     if (timeForGameOver < time_Start) {
         Timecounted = 40000000 - time_Start + timeForGameOver;
     } else {
         Timecounted = timeForGameOver - time_Start;
     }

     sprintf(timePlayedtoDisplay, "%lu", Timecounted / 1000);
 }



void Tetris_Init(void) {
    Tetris_GenerateTetromino();
    HAL_Delay(750);

}


void Tetris_GenerateTetromino(void) {
    randomNum = RNG_GetRAND_Num();
    HAL_Delay(20);
    memcpy(Tetris_currentTetromino, tetrominoShapes[randomNum][Rotation_at_currentScreen], sizeof(tetrominoShapes[randomNum][Rotation_at_currentScreen]));

}





void Tetris_PlaceTetromino(uint32_t grid[HEIGHT_SIZE][WIDTH_SIZE]) {
    for (uint8_t y = 0; y < TETROMINO_SIZE; y++) {
        for (uint8_t x = 0; x < TETROMINO_SIZE; x++) {
            if (Tetris_currentTetromino[y][x] == 1) {
                uint8_t gridX = Xpos + x;
                uint8_t gridY = Ypos + y ;
                if (gridX < WIDTH_SIZE && gridY < HEIGHT_SIZE) {
                    grid[gridY][gridX] = 1;
                }
            }
        }
    }
}

bool Tetris_CheckCollision(void) {
    for (uint8_t y = 0; y < TETROMINO_SIZE; y++) {
        for (uint8_t x = 0; x < TETROMINO_SIZE; x++) {
            if (Tetris_currentTetromino[y][x] == 1) {
                uint8_t gridX = Xpos + x;
                uint8_t gridY = Ypos + y;

                if (gridX >= WIDTH_SIZE || gridY >= HEIGHT_SIZE || Tetris_gamecurrentScreen[gridY][gridX] != 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Tetris_RotateBlock(){

	uint8_t Tetris_previousRotation = Rotation_at_currentScreen;
	Rotation_at_currentScreen +=1;
	if (Rotation_at_currentScreen >= 4) {
	        Rotation_at_currentScreen = 0;
	}
    memcpy(Tetris_currentTetromino, Tetris_clearTetromino, sizeof(tetrominoShapes[randomNum][Rotation_at_currentScreen]));
    memcpy(Tetris_currentTetromino, tetrominoShapes[randomNum][Rotation_at_currentScreen], sizeof(tetrominoShapes[randomNum][Rotation_at_currentScreen]));
	bool collision = Tetris_CheckCollision();
	if (!collision){
		memcpy(Tetris_nextGrid, Tetris_gamecurrentScreen, sizeof(Tetris_gamecurrentScreen));
		Tetris_PlaceTetromino(Tetris_nextGrid);
		Tetris_Draw_Grid(Tetris_gamecurrentScreen);
		Tetris_Draw_Grid(Tetris_nextGrid);
	} else {
		Rotation_at_currentScreen = Tetris_previousRotation;
	    memcpy(Tetris_currentTetromino, Tetris_clearTetromino, sizeof(tetrominoShapes[randomNum][Rotation_at_currentScreen]));
        memcpy(Tetris_currentTetromino, tetrominoShapes[randomNum][Rotation_at_currentScreen], sizeof(tetrominoShapes[randomNum][Rotation_at_currentScreen]));

	}
}


void Tetris_MoveBlockDown(void) {
	uint8_t prevYpos = Ypos;
		Ypos +=1;

		if (!Tetris_CheckCollision()){
			Tetris_PlaceTetromino(Tetris_nextGrid);
		} else {
			Ypos = prevYpos;
			Tetris_PlaceTetromino(Tetris_gamecurrentScreen);
		}
}




void Tetris_PlayGame(void) {

	Tetris_Init();

	timeFromStart = HAL_GetTick();
    Tetris_PlaceTetromino(Tetris_nextGrid);

    Tetris_GameLoop();

    timeGameOver = HAL_GetTick();

    TimePlaying( timeGameOver, timeFromStart);

    GameOverDemo(timePlayedtoDisplay);
}


