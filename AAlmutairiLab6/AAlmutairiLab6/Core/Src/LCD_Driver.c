/*
 * LCD_Driver.c
 *
 *  Created on: Sep 28, 2023
 *      Author: Xavion
 */

#include "LCD_Driver.h"

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */

static LTDC_HandleTypeDef hltdc;
static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
static FONT_t *LCD_Currentfonts;
static uint16_t CurrentTextColor   = 0xFFFF;


/*
 * fb[y*W+x] OR fb[y][x]
 * Alternatively, we can modify the linker script to have an end address of 20013DFB instead of 2002FFFF, so it does not place variables in the same region as the frame buffer. In this case it is safe to just specify the raw address as frame buffer.
 */
//uint32_t frameBuffer[(LCD_PIXEL_WIDTH*LCD_PIXEL_WIDTH)/2] = {0};		//16bpp pixel format. We can size to uint32. this ensures 32 bit alignment


//Someone from STM said it was "often accessed" a 1-dim array, and not a 2d array. However you still access it like a 2dim array,  using fb[y*W+x] instead of fb[y][x].
uint16_t frameBuffer[LCD_PIXEL_WIDTH*LCD_PIXEL_HEIGHT] = {0};			//16bpp pixel format.


void LCD_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the LTDC clock */
  __HAL_RCC_LTDC_CLK_ENABLE();

  /* Enable GPIO clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* GPIO Config
   *
    LCD pins
   LCD_TFT R2 <-> PC.10
   LCD_TFT G2 <-> PA.06
   LCD_TFT B2 <-> PD.06
   LCD_TFT R3 <-> PB.00
   LCD_TFT G3 <-> PG.10
   LCD_TFT B3 <-> PG.11
   LCD_TFT R4 <-> PA.11
   LCD_TFT G4 <-> PB.10
   LCD_TFT B4 <-> PG.12
   LCD_TFT R5 <-> PA.12
   LCD_TFT G5 <-> PB.11
   LCD_TFT B5 <-> PA.03
   LCD_TFT R6 <-> PB.01
   LCD_TFT G6 <-> PC.07
   LCD_TFT B6 <-> PB.08
   LCD_TFT R7 <-> PG.06
   LCD_TFT G7 <-> PD.03
   LCD_TFT B7 <-> PB.09
   LCD_TFT HSYNC <-> PC.06
   LCDTFT VSYNC <->  PA.04
   LCD_TFT CLK   <-> PG.07
   LCD_TFT DE   <->  PF.10
  */

  /* GPIOA configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 |
                           GPIO_PIN_11 | GPIO_PIN_12;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
  GPIO_InitStructure.Alternate= GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

 /* GPIOB configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_8 | \
                           GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

 /* GPIOC configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

 /* GPIOD configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_6;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

 /* GPIOF configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_10;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);

 /* GPIOG configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | \
                           GPIO_PIN_11;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

  /* GPIOB configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStructure.Alternate= GPIO_AF9_LTDC;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* GPIOG configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_12;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void LTCD_Layer_Init(uint8_t LayerIndex)
{
	LTDC_LayerCfgTypeDef  pLayerCfg;

	pLayerCfg.WindowX0 = 0;	//Configures the Window HORZ START Position.
	pLayerCfg.WindowX1 = LCD_PIXEL_WIDTH;	//Configures the Window HORZ Stop Position.
	pLayerCfg.WindowY0 = 0;	//Configures the Window vertical START Position.
	pLayerCfg.WindowY1 = LCD_PIXEL_HEIGHT;	//Configures the Window vertical Stop Position.
	pLayerCfg.PixelFormat = LCD_PIXEL_FORMAT_1;  //INCORRECT PIXEL FORMAT WILL GIVE WEIRD RESULTS!! IT MAY STILL WORK FOR 1/2 THE DISPLAY!!! //This is our buffers pixel format. 2 bytes for each pixel
	pLayerCfg.Alpha = 255;
	pLayerCfg.Alpha0 = 0;
	pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
	pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
	if (LayerIndex == 0){
		pLayerCfg.FBStartAdress = (uintptr_t)frameBuffer;
	}
	pLayerCfg.ImageWidth = LCD_PIXEL_WIDTH;
	pLayerCfg.ImageHeight = LCD_PIXEL_HEIGHT;
	pLayerCfg.Backcolor.Blue = 0;
	pLayerCfg.Backcolor.Green = 0;
	pLayerCfg.Backcolor.Red = 0;
	if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, LayerIndex) != HAL_OK)
	{
		LCD_Error_Handler();
	}
}

void clearScreen(void)
{
  LCD_Clear(0,LCD_COLOR_WHITE);
}

void LTCD__Init(void)
{
	hltdc.Instance = LTDC;
	/* Configure horizontal synchronization width */
	hltdc.Init.HorizontalSync = ILI9341_HSYNC;
	/* Configure vertical synchronization height */
	hltdc.Init.VerticalSync = ILI9341_VSYNC;
	/* Configure accumulated horizontal back porch */
	hltdc.Init.AccumulatedHBP = ILI9341_HBP;
	/* Configure accumulated vertical back porch */
	hltdc.Init.AccumulatedVBP = ILI9341_VBP;
	/* Configure accumulated active width */
	hltdc.Init.AccumulatedActiveW = 269;
	/* Configure accumulated active height */
	hltdc.Init.AccumulatedActiveH = 323;
	/* Configure total width */
	hltdc.Init.TotalWidth = 279;
	/* Configure total height */
	hltdc.Init.TotalHeigh = 327;
	/* Configure R,G,B component values for LCD background color */
	hltdc.Init.Backcolor.Red = 0;
	hltdc.Init.Backcolor.Blue = 0;
	hltdc.Init.Backcolor.Green = 0;

	/* LCD clock configuration */
	/* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
	/* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
	/* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/4 = 48 Mhz */
	/* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_8 = 48/4 = 6Mhz */

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
	PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
	PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
	/* Polarity */
	hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
	hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
	hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
	hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

	LCD_GPIO_Init();

	if (HAL_LTDC_Init(&hltdc) != HAL_OK)
	 {
	   LCD_Error_Handler();
	 }

	ili9341_Init();
}

/* START Draw functions */


/*
 * This is really the only function needed.
 * All drawing consists of is manipulating the array.
 * Adding input sanitation should probably be done.
 */
void LCD_Draw_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
	if (x < 0 || x > 239 || y < 0 || y > 319)
	{
		return;
	}
	frameBuffer[y*LCD_PIXEL_WIDTH+x] = color;  //You cannot do x*y to set the pixel.
}

/*
 * These functions are simple examples. Most computer graphics like OpenGl and stm's graphics library use a state machine. Where you first call some function like SetColor(color), SetPosition(x,y), then DrawSqure(size)
 * Instead all of these are explicit where color, size, and position are passed in.
 * There is tons of ways to handle drawing. I dont think it matters too much.
 */
void LCD_Draw_Circle_Fill(uint16_t Xpos, uint16_t Ypos, uint16_t radius, uint16_t color)
{
    for(int16_t y=-radius; y<=radius; y++)
    {
        for(int16_t x=-radius; x<=radius; x++)
        {
            if(x*x+y*y <= radius*radius)
            {
            	LCD_Draw_Pixel(x+Xpos, y+Ypos, color);
            }
        }
    }
}

void LCD_Draw_Vertical_Line(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
  for (uint16_t i = 0; i < len; i++)
  {
	  LCD_Draw_Pixel(x, i+y, color);
  }
}



void LCD_Clear(uint8_t LayerIndex, uint16_t Color)
{
	if (LayerIndex == 0){
		for (uint32_t i = 0; i < LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT; i++){
			frameBuffer[i] = Color;
		}
	}
  // TODO: Add more Layers if needed
}



//This was taken and adapted from stm32's mcu code
void LCD_SetTextColor(uint16_t Color)
{
  CurrentTextColor = Color;
}

//This was taken and adapted from stm32's mcu code
void LCD_SetFont(FONT_t *fonts)
{
  LCD_Currentfonts = fonts;
}

//This was taken and adapted from stm32's mcu code
void LCD_Draw_Char(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  uint32_t index = 0, counter = 0;
  for(index = 0; index < LCD_Currentfonts->Height; index++)
  {
    for(counter = 0; counter < LCD_Currentfonts->Width; counter++)
    {
      if((((c[index] & ((0x80 << ((LCD_Currentfonts->Width / 12 ) * 8 ) ) >> counter)) == 0x00) && (LCD_Currentfonts->Width <= 12)) || (((c[index] & (0x1 << counter)) == 0x00)&&(LCD_Currentfonts->Width > 12 )))
      {
         //Background If want to overrite text under then add a set color here
      }
      else
      {
    	  LCD_Draw_Pixel(counter + Xpos,index + Ypos,CurrentTextColor);
      }
    }
  }
}

//This was taken and adapted from stm32's mcu code
void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
  Ascii -= 32;
  LCD_Draw_Char(Xpos, Ypos, &LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height]);
}


void LCD_Draw_Row_Lines(uint16_t x, uint16_t y, uint16_t size, uint16_t color)
{
  for (uint16_t row = 0; row < size; row++)
  {
	  LCD_Draw_Pixel(row + x, y, color);
  }
}


void LCD_Draw_All_Shapes(uint16_t Xpos, uint16_t Ypos, uint16_t color, uint16_t rotation, TetrominoType type)
{
	switch (type)
	{
	case TETROMINO_I:
		if (rotation == 0)
		{
			LCD_DrawTetromino(Xpos, Ypos, color);
			LCD_DrawTetromino(Xpos + 24, Ypos, color);
			LCD_DrawTetromino(Xpos + 48, Ypos, color);
			LCD_DrawTetromino(Xpos + 72, Ypos, color);
		}
		else if (rotation == 1)
		{
			LCD_DrawTetromino(Xpos, Ypos, color);
			LCD_DrawTetromino(Xpos, Ypos + 32, color);
			LCD_DrawTetromino(Xpos, Ypos + 64, color);
			LCD_DrawTetromino(Xpos, Ypos + 96, color);
		}
		else if (rotation == 2)
		{
			LCD_DrawTetromino(Xpos, Ypos, color);
			LCD_DrawTetromino(Xpos + 24, Ypos, color);
			LCD_DrawTetromino(Xpos + 48, Ypos, color);
			LCD_DrawTetromino(Xpos + 72, Ypos, color);
		}
		else if ( rotation == 3)
		{
			LCD_DrawTetromino(Xpos, Ypos, color);
			LCD_DrawTetromino(Xpos, Ypos + 32, color);
			LCD_DrawTetromino(Xpos, Ypos + 64, color);
			LCD_DrawTetromino(Xpos, Ypos + 96, color);
		}
		break;
	case  TETROMINO_O:
				if (rotation == 0)
				{
					LCD_DrawTetromino(Xpos, Ypos, color);
					LCD_DrawTetromino(Xpos , Ypos + 32, color);
					LCD_DrawTetromino(Xpos + 24, Ypos, color);
					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
				}
				else if (rotation == 1)
				{
					LCD_DrawTetromino(Xpos, Ypos, color);
					LCD_DrawTetromino(Xpos, Ypos + 32, color);
					LCD_DrawTetromino(Xpos + 24, Ypos, color);
					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
				}
				else if (rotation == 2)
				{
					LCD_DrawTetromino(Xpos, Ypos, color);
					LCD_DrawTetromino(Xpos, Ypos + 32, color);
					LCD_DrawTetromino(Xpos + 24, Ypos, color);
					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
				}
				else if ( rotation == 3)
				{
					LCD_DrawTetromino(Xpos, Ypos, color);
					LCD_DrawTetromino(Xpos, Ypos + 32, color);
					LCD_DrawTetromino(Xpos + 24, Ypos, color);
					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
				}
				break;
	 case TETROMINO_T:
		 	 	 	 	 	if (rotation == 0)
		 				{
		 					LCD_DrawTetromino(Xpos, Ypos, color);
		 					LCD_DrawTetromino(Xpos , Ypos + 32, color);
		 					LCD_DrawTetromino(Xpos - 24, Ypos + 32, color);
		 					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
		 				}
		 				else if (rotation == 1)
		 				{
		 					LCD_DrawTetromino(Xpos, Ypos, color);
		 					LCD_DrawTetromino(Xpos, Ypos + 32, color);
		 					LCD_DrawTetromino(Xpos, Ypos + 64, color);
		 					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
		 				}
		 				else if (rotation == 2)
		 				{
		 					LCD_DrawTetromino(Xpos, Ypos, color);
		 					LCD_DrawTetromino(Xpos + 24, Ypos, color);
		 					LCD_DrawTetromino(Xpos + 48, Ypos, color);
		 					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
		 				}
		 				else if ( rotation == 3)
		 				{
		 					LCD_DrawTetromino(Xpos + 24, Ypos, color);
		 					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
		 					LCD_DrawTetromino(Xpos + 24, Ypos + 64, color);
		 					LCD_DrawTetromino(Xpos, Ypos + 32, color);
		 				}
		 				break;
	 case TETROMINO_S:
		 						if (rotation == 0)
				 				{
				 					LCD_DrawTetromino(Xpos, Ypos, color);
				 					LCD_DrawTetromino(Xpos + 24, Ypos, color);
				 					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
				 					LCD_DrawTetromino(Xpos + 48, Ypos + 32, color);
				 				}
				 				else if (rotation == 1)
				 				{
				 					LCD_DrawTetromino(Xpos + 24, Ypos, color);
				 					LCD_DrawTetromino(Xpos + 24 , Ypos + 32, color);
				 					LCD_DrawTetromino(Xpos, Ypos + 32, color);
				 					LCD_DrawTetromino(Xpos, Ypos + 64, color);
				 				}
				 				else if (rotation == 2)
				 				{
				 					LCD_DrawTetromino(Xpos, Ypos, color);
				 					LCD_DrawTetromino(Xpos + 24, Ypos, color);
				 					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
				 					LCD_DrawTetromino(Xpos + 48, Ypos + 32, color);
				 				}
				 				else if ( rotation == 3)
				 				{
				 					LCD_DrawTetromino(Xpos + 24, Ypos, color);
				 					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
				 					LCD_DrawTetromino(Xpos, Ypos + 32, color);
				 					LCD_DrawTetromino(Xpos, Ypos + 64, color);
				 				}
				 				break;
	 case TETROMINO_Z:
		 	 	 	 	 	 	if (rotation == 0)
		 				 		{
		 				 					LCD_DrawTetromino(Xpos + 24, Ypos, color);
		 				 					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
		 				 					LCD_DrawTetromino(Xpos + 48, Ypos, color);
		 				 					LCD_DrawTetromino(Xpos, Ypos + 32, color);
		 				 				}
		 				 				else if (rotation == 1)
		 				 				{
		 				 					LCD_DrawTetromino(Xpos, Ypos, color);
		 				 					LCD_DrawTetromino(Xpos, Ypos + 32, color);
		 				 					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
		 				 					LCD_DrawTetromino(Xpos + 24, Ypos + 64, color);
		 				 				}
		 				 				else if (rotation == 2)
		 				 				{
		 				 					LCD_DrawTetromino(Xpos + 24, Ypos, color);
		 				 					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
		 				 					LCD_DrawTetromino(Xpos + 48, Ypos, color);
		 				 					LCD_DrawTetromino(Xpos, Ypos + 32, color);
		 				 				}
		 				 				else if ( rotation == 3)
		 				 				{
		 				 					LCD_DrawTetromino(Xpos, Ypos, color);
		 				 					LCD_DrawTetromino(Xpos, Ypos + 32, color);
		 				 					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
		 				 					LCD_DrawTetromino(Xpos + 24, Ypos + 64, color);
		 				 				}
		 				 				break;
	 case TETROMINO_J:
		 	 	 	 	 	 	 	 if (rotation == 0)
		 		 				 		{
		 		 				 			LCD_DrawTetromino(Xpos, Ypos, color);
		 		 				 			LCD_DrawTetromino(Xpos, Ypos + 32, color);
		 		 				 			LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
		 		 				 			LCD_DrawTetromino(Xpos + 48, Ypos + 32, color);
		 		 				 				}
		 		 				 				else if (rotation == 1)
		 		 				 				{
		 		 				 					LCD_DrawTetromino(Xpos, Ypos, color);
		 		 				 					LCD_DrawTetromino(Xpos, Ypos + 32, color);
		 		 				 					LCD_DrawTetromino(Xpos, Ypos + 64, color);
		 		 				 					LCD_DrawTetromino(Xpos + 24, Ypos, color);
		 		 				 				}
		 		 				 				else if (rotation == 2)
		 		 				 				{
		 		 				 					LCD_DrawTetromino(Xpos, Ypos, color);
		 		 				 					LCD_DrawTetromino(Xpos + 24, Ypos, color);
		 		 				 					LCD_DrawTetromino(Xpos + 48, Ypos, color);
		 		 				 					LCD_DrawTetromino(Xpos + 48, Ypos + 32, color);
		 		 				 				}
		 		 				 				else if ( rotation == 3)
		 		 				 				{
		 		 				 					LCD_DrawTetromino(Xpos + 24, Ypos, color);
		 		 				 					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
		 		 				 					LCD_DrawTetromino(Xpos + 24, Ypos + 64, color);
		 		 				 					LCD_DrawTetromino(Xpos, Ypos + 64, color);
		 		 				 				}
		 		 				 				break;
	 case TETROMINO_L:
		 	 	 	 	 	 	 	 	 if (rotation == 0)
				 		 				 		{
				 		 				 			LCD_DrawTetromino(Xpos, Ypos + 32, color);
				 		 				 			LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
				 		 				 			LCD_DrawTetromino(Xpos + 48, Ypos + 32, color);
				 		 				 			LCD_DrawTetromino(Xpos + 48, Ypos, color);
				 		 				 				}
				 		 				 				else if (rotation == 1)
				 		 				 				{
				 		 				 					LCD_DrawTetromino(Xpos, Ypos, color);
				 		 				 					LCD_DrawTetromino(Xpos, Ypos + 32, color);
				 		 				 					LCD_DrawTetromino(Xpos, Ypos + 64, color);
				 		 				 					LCD_DrawTetromino(Xpos + 24, Ypos + 64, color);
				 		 				 				}
				 		 				 				else if (rotation == 2)
				 		 				 				{
				 		 				 					LCD_DrawTetromino(Xpos, Ypos, color);
				 		 				 					LCD_DrawTetromino(Xpos + 24, Ypos, color);
				 		 				 					LCD_DrawTetromino(Xpos + 48, Ypos, color);
				 		 				 					LCD_DrawTetromino(Xpos, Ypos + 32, color);
				 		 				 				}
				 		 				 				else if ( rotation == 3)
				 		 				 				{
				 		 				 					LCD_DrawTetromino(Xpos, Ypos, color);
				 		 				 					LCD_DrawTetromino(Xpos + 24, Ypos, color);
				 		 				 					LCD_DrawTetromino(Xpos + 24, Ypos + 32, color);
				 		 				 					LCD_DrawTetromino(Xpos + 24, Ypos + 64, color);
				 		 				 				}
				 		 				 				break;
	 case TETROMINO_NONE:
		 	 	 	 	 	 	 	 LCD_Clear(0,LCD_COLOR_BLACK);

		 	 	 	 	 	 	 	 break;

	}
}



void LCD_DrawTetromino(uint16_t pixelX, uint16_t pixelY, uint16_t color) {

	for(int y = pixelY; y <= pixelY + 32; y++)
	{
		LCD_Draw_Row_Lines(pixelX, y, 24, color);
	}
}



void LCD_TetrisDrawGrid()
{
    for (int x = 0; x <= 240; x += 24) {
        LCD_Draw_Vertical_Line(x, 0, 320, LCD_COLOR_WHITE);
    }

    for (int y = 0; y <= 320; y += 32) {
        LCD_Draw_Row_Lines(0, y, 240, LCD_COLOR_WHITE);
    }
}

void Tetris_Draw_Grid(uint32_t grid[10][10])
{
	LCD_Clear(0,LCD_COLOR_GREY);
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			if (grid[y][x])
			{
				LCD_DrawTetromino(x*CELL_WIDTH, y * CELL_HEIGHT, LCD_COLOR_GREEN);


				}
			}
		}
	LCD_TetrisDrawGrid();
	}



void LCD_DrawMainMenu(void)
{
	LCD_Clear(0, LCD_COLOR_WHITE);
	LCD_Draw_All_Shapes(120, 0, LCD_COLOR_CYAN,0, TETROMINO_I);
	LCD_Draw_All_Shapes(72, 96, LCD_COLOR_BLUE,0, TETROMINO_O);
	LCD_Draw_All_Shapes(96, 192, LCD_COLOR_BLUE2, 0, TETROMINO_T);
	LCD_Draw_All_Shapes(24, 0, LCD_COLOR_GREEN,0, TETROMINO_S);
	LCD_Draw_All_Shapes(168, 96, LCD_COLOR_YELLOW, 0,TETROMINO_Z);
	LCD_Draw_All_Shapes(24, 256, LCD_COLOR_RED, 0,TETROMINO_J);
	LCD_Draw_All_Shapes(144, 225, LCD_COLOR_MAGENTA, 0,TETROMINO_L);

	LCD_DisplayChar(78, 140, 'T');
	LCD_DisplayChar(93, 140, 'e');
	LCD_DisplayChar(108, 140, 't');
	LCD_DisplayChar(123, 140, 'r');
	LCD_DisplayChar(138, 140, 'i');
	LCD_DisplayChar(153, 140, 's');


	LCD_DisplayChar(38, 160, 'T');
	LCD_DisplayChar(53, 160, 'o');
	LCD_DisplayChar(68, 160, 'u');
	LCD_DisplayChar(83, 160, 'c');
	LCD_DisplayChar(98, 160, 'h');

	LCD_DisplayChar(113, 160, 't');
	LCD_DisplayChar(128, 160, 'o');


	LCD_DisplayChar(153, 160, 'S');
	LCD_DisplayChar(168, 160, 'T');
	LCD_DisplayChar(183, 160, 'A');
	LCD_DisplayChar(198, 160, 'R');
	LCD_DisplayChar(213, 160, 'T');
}

void MainMenuDemo(void)
{
	LCD_TetrisDrawGrid();

	LCD_SetTextColor(LCD_COLOR_BLACK);

	LCD_SetFont(&Font16x24);


	LCD_DrawMainMenu();





	LCD_TetrisDrawGrid();
}



void GameplayDemo(void) {

    Tetris_PlayGame();
}






void GameOverDemo(char TimecountedPlaying[10])
{
	LCD_Clear(0, LCD_COLOR_CYAN);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font16x24);


	LCD_DisplayChar(100, 140, 'G');
	LCD_DisplayChar(117, 140, 'a');
	LCD_DisplayChar(132, 140, 'm');
	LCD_DisplayChar(146, 140, 'e');


	LCD_DisplayChar(100, 160, 'O');
	LCD_DisplayChar(115, 160, 'v');
	LCD_DisplayChar(126, 160, 'e');
	LCD_DisplayChar(137, 160, 'r');

	for (uint8_t t = 0; t < 10; t++)
	{
	        if (TimecountedPlaying[t] != '\0') {
	            LCD_DisplayChar(140 + (t * 10), 180, TimecountedPlaying[t]);
	        } else {
	            break;
	        }
	    }

	    LCD_DisplayChar(150 + (10 * strlen(TimecountedPlaying)), 180, 's');
	}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void LCD_Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

// Touch Functionality   //

#if COMPILE_TOUCH_FUNCTIONS == 1

void InitializeLCDTouch(void)
{
  if(STMPE811_Init() != STMPE811_State_Ok)
  {
	 for(;;); // Hang code due to error in initialzation
  }
}

STMPE811_State_t returnTouchStateAndLocation(STMPE811_TouchData * touchStruct)
{
	return STMPE811_ReadTouch(touchStruct);
}

void DetermineTouchPosition(STMPE811_TouchData * touchStruct)
{
	STMPE811_DetermineTouchPosition(touchStruct);
}

uint8_t ReadRegisterFromTouchModule(uint8_t RegToRead)
{
	return STMPE811_Read(RegToRead);
}

void WriteDataToTouchModule(uint8_t RegToWrite, uint8_t writeData)
{
	STMPE811_Write(RegToWrite, writeData);
}

#endif // COMPILE_TOUCH_FUNCTIONS
