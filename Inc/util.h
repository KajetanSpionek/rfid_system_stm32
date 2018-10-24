/*!
* \file util.h
* \author Kajetan Spionek
* Handles interface and program logic
*/

#include "stm32f4xx_hal.h"
#include "lcd.h"
#include "uart.h"
#include "lc256.h"
#include "rc522.h"

#define RESULT_OPEN_DOOR 0x01
#define RESULT_WRONG_PASS 0x02
#define RESULT_WRONG_TOKEN 0x03
#define RESULT_MASTER_MODE 0x04

uint8_t state_PasswordInput(uint8_t* password);
void state_CheckToken(uint8_t* CardID);
uint8_t searchDb(uint8_t* CardID, uint8_t* password, uint8_t* enable_password);
uint8_t checkMasterCard(uint8_t* CardID, uint8_t* password, uint8_t* enable_password);
uint8_t passwordCheck(uint8_t* password, uint8_t* input_password);
void result(uint8_t mode, uint8_t* CardID);
void state_MasterMode(void);
void sendUartPacketStatus(uint8_t status, uint8_t* CardID);
void state_AddTokenDb(void);
void state_DeleteTokenDb(void);
void deleteDb(uint8_t* CardID);
uint8_t addDb(uint8_t* CardID, uint8_t* password, uint8_t password_status);
void sendUartDb(void);
uint8_t searchInDb(uint8_t* CardID);


