#include "lcd.h"

extern UART_HandleTypeDef huart1;

/**
   * @brief Screen initialization
   * @param None
   * @retval None
   */
void lcd_Init(void) {
	
	BSP_LCD_Init();
	BSP_TS_Init(240, 360);
	
	BSP_LCD_LayerDefaultInit(LCD_BACKGROUND_LAYER, LCD_FRAME_BUFFER);
	BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER, LCD_FRAME_BUFFER);
	BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
	BSP_LCD_DisplayOn();
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	
	
}

/**
   * @brief Displaying header on lcd "RDIF SYSTEM"
   * @param None
   * @retval None
   */
void lcd_DisplayHeader(void) {
	
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_DisplayStringAtLine(2,(uint8_t*)" RFID SYSTEM");
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

/**
   * @brief Displaying "scan your token" on screen
   * @param None
   * @retval None
   */
void lcd_DisplayNoToken(void) {
	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAtLine(7,(uint8_t*)"  SCAN YOUR   ");
	BSP_LCD_DisplayStringAtLine(8,(uint8_t*)"    TOKEN     ");
	
}

/**
   * @brief Displaying "Token valid" and it's ID
   * @param CardID: pointer to scanned card id
   * @retval None
   */
void lcd_DisplayTokenDetectedValid(uint8_t* CardID) {
	
	char buffer[50];
	sprintf(buffer, "%x %x %x %x %x", CardID[0],CardID[1],CardID[2],CardID[3],CardID[4]);
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
	BSP_LCD_DisplayStringAtLine(7,(uint8_t*)" TOKEN VALID   ");
	BSP_LCD_DisplayStringAtLine(8,(uint8_t*)buffer);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	
}

/**
   * @brief Displaying "Invalid token" on screen 
   * @param None
   * @retval None
   */
void lcd_DisplayTokenDetectedInvalid(void) {
	
	BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
	BSP_LCD_ClearStringLine(7);
	BSP_LCD_ClearStringLine(8);
	BSP_LCD_DisplayStringAtLine(7,(uint8_t*)"   UNKNOWN");
	BSP_LCD_DisplayStringAtLine(8,(uint8_t*)"    TOKEN   ");
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	
}

/**
   * @brief Displaying "Invalid password" on screen
   * @param None
   * @retval None
   */
void lcd_DisplayInvalidPassword(void) {
	
	BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
	BSP_LCD_ClearStringLine(7);
	BSP_LCD_ClearStringLine(8);
	BSP_LCD_DisplayStringAtLine(7,(uint8_t*)"  INCORRECT");
	BSP_LCD_DisplayStringAtLine(8,(uint8_t*)"   PASSWORD");
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	
}

/**
   * @brief Detecting touch position (x,y) 
   * @param x: pointer to horizonal touched (on screen) position
   * @param y: pointer to vertical touched (on screen) position
   * @retval None
   */
void lcd_TouchDetect(uint32_t *x, uint32_t *y) {
	
	static TS_StateTypeDef  TS_State;
	static uint32_t xx = 0, yy = 0;
	/* Get touch screen position */
	BSP_TS_GetState(&TS_State);
	
	xx = TS_State.X;
	yy = TS_State.Y;
		
	/* Checks if touch is detected */
	
	
	
	if(TS_State.TouchDetected){

	*x = xx;
	*y = yy;
	}	
	else {	
		*x = 9999;
		*y = 9999;
	}		
}
	
	
/**
   * @brief Determines key which was pressed (on pin keyboard)
   * @param x: horizonal touched (on screen) position
   * @param y: vertical touched (on screen) position
   * @retval box: returns key key which was pressed
   */
uint8_t lcd_BoxDetect(uint32_t x, uint32_t y) {
			
		// Row 1

		if (y > 60 && y < 110) {		
				if (x > 25 && x < 75) return 0x1;
				if (x > 95 && x < 145) return 0x2;			
				if (x > 165 && x < 215) return 0x3;		
			}	

		// Row 2 

			if (y > 125 && y < 175) {		
				if (x > 25 && x < 75) return 0x4;
				if (x > 95 && x < 145) return 0x5;			
				if (x > 165 && x < 215)	return 0x6;		
			}	
			
		// Row 3
			
			if (y > 190 && y < 240) {		
				if (x > 25 && x < 75) return 0x7;
				if (x > 95 && x < 145) return 0x8;		
				if (x > 165 && x < 215)	return 0x9;		
			}	

		// Row 4	
			
			if (y > 255 && y < 305) {		
				if (x > 25 && x < 75) return 0x0A;
				if (x > 95 && x < 145) return 0x0;		
				if (x > 165 && x < 215)	return 0x0B;		
			}	
			
		// Not touched 
			
			if (x == 9999 && y == 9999) return 0xEE;
			
			return 0xFF;			
}

/**
   * @brief Displaying pin keyboard
   * @param None
   * @retval None
   */
void lcd_ToggleKeyboard(void) {
	
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	lcd_DisplayPinState(0);

	int start_x = 25;
	int start_y = 60;
	
	// Create borders
	for (int const_x = 0; const_x < 3; const_x++) {
		for (int const_y = 0; const_y < 4; const_y++) {
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHTCYAN);
			BSP_LCD_FillRect(start_x+70*const_x,start_y+65*const_y,49,49);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_DrawRect(start_x+70*const_x,start_y+65*const_y,49,49);
			
		}
	}	

	lcd_KeyboardRow1();
	lcd_KeyboardRow2();
	lcd_KeyboardRow3();
	lcd_KeyboardRow4();
	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

}

/**
   * @brief Displaying row 1 of buttons on keyboard
   * @param None
   * @retval None
   */
void lcd_KeyboardRow1(void) {
	
		uint8_t bit_map_1[12][12] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
																{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
																{0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
																{0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
																{0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
																{0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
																{0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
															};		
	
	uint8_t bit_map_2[12][12] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
																{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
																{0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
															};
	
	uint8_t bit_map_3[12][12] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
															};																								
			// 1
		
		int base_x = 26;
		int base_y = 61;
		// 26,61
	
		for (int temp_y = 0; temp_y < 12; temp_y++) {
			for (int temp_x = 0; temp_x < 12; temp_x++) {
				if (bit_map_1[temp_y][temp_x] == 1)
					BSP_LCD_FillRect(base_x+temp_x*4,base_y+temp_y*4,4,4);
			}
		}
	
		// 2
	
		base_x = 96;
		base_y = 61;	
	
		for (int temp_y = 0; temp_y < 12; temp_y++) {
			for (int temp_x = 0; temp_x < 12; temp_x++) {
				if (bit_map_2[temp_y][temp_x] == 1)
					BSP_LCD_FillRect(base_x+temp_x*4,base_y+temp_y*4,4,4);
			}
		}
		
		// 3
	
		base_x = 166;
		base_y = 61;	
	
		for (int temp_y = 0; temp_y < 12; temp_y++) {
			for (int temp_x = 0; temp_x < 12; temp_x++) {
				if (bit_map_3[temp_y][temp_x] == 1)
					BSP_LCD_FillRect(base_x+temp_x*4,base_y+temp_y*4,4,4);
			}
		}
														
															
	
}



/**
   * @brief Displaying row 2 of buttons on keyboard
   * @param None
   * @retval None
   */
void lcd_KeyboardRow2(void) {
	
	uint8_t bit_map_4[12][12] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0},
																{0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
															};	

	uint8_t bit_map_5[12][12] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
															};														
													
													
	uint8_t bit_map_6[12][12] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
															};					
	
	
	
	// 4
		
		int base_x = 26;
		int base_y = 126;

										
		// 26,61
	
		for (int temp_y = 0; temp_y < 12; temp_y++) {
			for (int temp_x = 0; temp_x < 12; temp_x++) {
				if (bit_map_4[temp_y][temp_x] == 1)
					BSP_LCD_FillRect(base_x+temp_x*4,base_y+temp_y*4,4,4);
			}
		}
	
		// 5
	
		base_x = 96;
		base_y = 126;	
	
		for (int temp_y = 0; temp_y < 12; temp_y++) {
			for (int temp_x = 0; temp_x < 12; temp_x++) {
				if (bit_map_5[temp_y][temp_x] == 1)
					BSP_LCD_FillRect(base_x+temp_x*4,base_y+temp_y*4,4,4);
			}
		}
		
		// 6
	
		base_x = 166;
		base_y = 126;	
	
		for (int temp_y = 0; temp_y < 12; temp_y++) {
			for (int temp_x = 0; temp_x < 12; temp_x++) {
				if (bit_map_6[temp_y][temp_x] == 1)
					BSP_LCD_FillRect(base_x+temp_x*4,base_y+temp_y*4,4,4);
			}
		}

	
	
	
	
	
}



/**
   * @brief Displaying row 3 of buttons on keyboard
   * @param None
   * @retval None
   */
void lcd_KeyboardRow3(void) {
	
		uint8_t bit_map_7[12][12] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
																{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
																{0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
															};													
														
	uint8_t bit_map_8[12][12] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
															};													
												
	uint8_t bit_map_9[12][12] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
															};				
		
															
		// 7
															
		int base_x = 26;
		int base_y = 191;

		// 26,61
	
		for (int temp_y = 0; temp_y < 12; temp_y++) {
			for (int temp_x = 0; temp_x < 12; temp_x++) {
				if (bit_map_7[temp_y][temp_x] == 1)
					BSP_LCD_FillRect(base_x+temp_x*4,base_y+temp_y*4,4,4);
			}
		}
	
		// 8
	
		base_x = 96;
		base_y = 191;	
	
		for (int temp_y = 0; temp_y < 12; temp_y++) {
			for (int temp_x = 0; temp_x < 12; temp_x++) {
				if (bit_map_8[temp_y][temp_x] == 1)
					BSP_LCD_FillRect(base_x+temp_x*4,base_y+temp_y*4,4,4);
			}
		}
		
		// 9
	
		base_x = 166;
		base_y = 191;	
	
		for (int temp_y = 0; temp_y < 12; temp_y++) {
			for (int temp_x = 0; temp_x < 12; temp_x++) {
				if (bit_map_9[temp_y][temp_x] == 1)
					BSP_LCD_FillRect(base_x+temp_x*4,base_y+temp_y*4,4,4);
			}
		}
	
}



/**
   * @brief Displaying row 4 of buttons on keyboard
   * @param None
   * @retval None
   */
void lcd_KeyboardRow4(void) {

	uint8_t bit_map_0[12][12] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
																};		
		
	uint8_t bit_map_L[12][12] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0},
																{0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0},
																{0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
																{0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
																{0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0},
																{0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																};		
	
	uint8_t bit_map_R[12][12] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
																{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
																};		
	
	
	
	
	
	
	
	
	
	
	// L
		
		int base_x = 26;
		int base_y = 256;

		// 26,61
	
		for (int temp_y = 0; temp_y < 12; temp_y++) {
			for (int temp_x = 0; temp_x < 12; temp_x++) {
				if (bit_map_L[temp_y][temp_x] == 1)
					BSP_LCD_FillRect(base_x+temp_x*4,base_y+temp_y*4,4,4);
			}
		}
	
		// 0
	
		base_x = 96;
		base_y = 256;	
	
		for (int temp_y = 0; temp_y < 12; temp_y++) {
			for (int temp_x = 0; temp_x < 12; temp_x++) {
				if (bit_map_0[temp_y][temp_x] == 1)
					BSP_LCD_FillRect(base_x+temp_x*4,base_y+temp_y*4,4,4);
			}
		}
		
		// R
	
		base_x = 166;
		base_y = 256;	
	
		for (int temp_y = 0; temp_y < 12; temp_y++) {
			for (int temp_x = 0; temp_x < 12; temp_x++) {
				if (bit_map_R[temp_y][temp_x] == 1)
					BSP_LCD_FillRect(base_x+temp_x*4,base_y+temp_y*4,4,4);
			}
		}

}


/**
   * @brief Displaying pin progress
   * @param no_digits: numbers of digits already pressed while inputing password
   * @retval None
   */
void lcd_DisplayPinState(uint8_t no_digits) {
	
	BSP_LCD_ClearStringLine(1);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	
	switch(no_digits) {
		case 1: BSP_LCD_DisplayStringAtLine(1,(uint8_t*)" PIN: * _ _ _ "); break;
		case 2: BSP_LCD_DisplayStringAtLine(1,(uint8_t*)" PIN: * * _ _ "); break;
		case 3: BSP_LCD_DisplayStringAtLine(1,(uint8_t*)" PIN: * * * _ "); break;
		case 4: BSP_LCD_DisplayStringAtLine(1,(uint8_t*)" PIN: * * * * "); break;
		default: BSP_LCD_DisplayStringAtLine(1,(uint8_t*)" PIN: _ _ _ _ "); break;
	}
}

/**
   * @brief Clearing lcd screen
   * @param None
   * @retval None
   */
void lcd_ClearScreen(void) {
	
	BSP_LCD_Clear(LCD_COLOR_WHITE);	
}

/**
   * @brief Displaying master mode menu
   * @param None
   * @retval None
   */
void lcd_DisplayMasterMode(void){
	
	BSP_LCD_Clear(LCD_COLOR_LIGHTBLUE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

	BSP_LCD_DisplayStringAtLine(1,(uint8_t*)"               ");
	BSP_LCD_DisplayStringAtLine(2,(uint8_t*)"   ADD NEW      ");
	BSP_LCD_DisplayStringAtLine(3,(uint8_t*)"               ");
	
	BSP_LCD_DisplayStringAtLine(5,(uint8_t*)"               ");
	BSP_LCD_DisplayStringAtLine(6,(uint8_t*)"    DELETE       ");
	BSP_LCD_DisplayStringAtLine(7,(uint8_t*)"               ");
	
	BSP_LCD_DisplayStringAtLine(9,(uint8_t*)"               ");
	BSP_LCD_DisplayStringAtLine(10,(uint8_t*)"     EXIT        ");
	BSP_LCD_DisplayStringAtLine(11,(uint8_t*)"               ");
	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	
}

/**
   * @brief Displaying add new token menu
   * @param None
   * @retval None
   */
void lcd_AddNewToken(void){
		
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
	BSP_LCD_DisplayStringAtLine(2,(uint8_t*)" ADD NEW USER  ");
	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAtLine(6,(uint8_t*)"  SCAN TOKEN     ");
	
	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAtLine(10,(uint8_t*)"HOLD TO CANCEL");
}

/**
   * @brief Displaying delete token menu
   * @param None
   * @retval None
   */
void lcd_DeleteToken(void) {
	
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_DisplayStringAtLine(2,(uint8_t*)"  DELETE USER  ");
	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAtLine(6,(uint8_t*)"  SCAN TOKEN     ");
	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAtLine(10,(uint8_t*)"HOLD TO CANCEL");
	
}

/**
   * @brief Displaying "Token already in db"
   * @param None
   * @retval None
   */
void lcd_tokenAlreadyInDb(void) {
	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAtLine(7,(uint8_t*)" TOKEN ALREADY   ");
	BSP_LCD_DisplayStringAtLine(8,(uint8_t*)"    IN DB     ");
}







