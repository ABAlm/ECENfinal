#ifndef INC_TETRIS_GAME_H_
#define INC_TETRIS_GAME_H_

#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "RNG_Driver.h"
#include "LCD_Driver.h"
#include "stmpe811.h"
#include "ApplicationCode.h"

// Constants
#define WIDTH_SIZE 10
#define HEIGHT_SIZE 10
#define TETROMINO_SIZE 4
#define POINTS_PER_LINE 100



extern uint32_t tetrominoShapes[7][4][TETROMINO_SIZE][TETROMINO_SIZE];


// Enum for Tetromino types
typedef enum {
    TETROMINO_I,
    TETROMINO_O,
    TETROMINO_T,
    TETROMINO_S,
    TETROMINO_Z,
    TETROMINO_J,
    TETROMINO_L,
    TETROMINO_NONE
} TetrominoType;

extern uint32_t Tetris_currentGrid[HEIGHT_SIZE][WIDTH_SIZE];
extern uint32_t Tetris_nextGrid[HEIGHT_SIZE][WIDTH_SIZE];
extern uint32_t Tetris_clearGrid[HEIGHT_SIZE][WIDTH_SIZE];

extern uint8_t Xlocation;
extern uint8_t Ylocation;
extern uint8_t currRot;
extern uint32_t currentTetromino[TETROMINO_SIZE][TETROMINO_SIZE];
extern uint32_t clearTetromino[TETROMINO_SIZE][TETROMINO_SIZE];
extern uint8_t randomNum;



// Function declarations
void Tetris_Init(void);
void Tetris_PlayGame(void);
bool Tetris_CheckCollision(void);
void Tetris_GenerateTetromino(void);
void Tetris_MoveBlockDown(void);
void Tetris_PlaceTetromino(uint32_t grid[HEIGHT_SIZE][WIDTH_SIZE]);
void Tetris_RotateBlock(void);
void Tetris_GameLoop();





#endif /* INC_TETRIS_GAME_H_ */
