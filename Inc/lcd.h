/*!
* \file lcd.h
* \author Kajetan Spionek
* Handles graphical interface displayed on LCD screen
*/

#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"
#include "uart.h"

void lcd_Init(void);
void lcd_DisplayHeader(void);
void lcd_DisplayNoToken(void);
void lcd_DisplayTokenDetectedValid(uint8_t* CardID);
void lcd_DisplayTokenDetectedInvalid(void);
void lcd_DisplayInvalidPassword(void);
void lcd_TouchDetect(uint32_t *x, uint32_t *y);
uint8_t lcd_BoxDetect(uint32_t x, uint32_t y);
void lcd_ToggleKeyboard(void);
void lcd_KeyboardRow1(void);
void lcd_KeyboardRow2(void);
void lcd_KeyboardRow3(void);
void lcd_KeyboardRow4(void);
void lcd_DisplayPinState(uint8_t no_digits);
void lcd_ClearScreen(void);
void lcd_DisplayMasterMode(void);
void lcd_AddNewToken(void);
void lcd_DeleteToken(void);
void lcd_tokenAlreadyInDb(void);




