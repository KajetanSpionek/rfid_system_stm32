#include "util.h"

uint8_t MasterID[5] = {0x70, 0xD2, 0x82, 0xCB, 0xEB};
uint8_t MasterPassword[4] = {2,5,8,0};

extern uint8_t interrupt_flag;
extern uint32_t door_flag;

/**
   * @brief Handles whole input password process
   * @param password: pointer to inputted password
   * @retval state: returns 1 if password successfully inputted or 0 if failed
   */
uint8_t state_PasswordInput(uint8_t* password) {
	
		uint32_t x = 0;
		uint32_t y = 0;
		
		uint8_t button = 0xFF;
		uint8_t last_pressed = 0xFF;	
		uint8_t star_state = 0;
		
		lcd_ToggleKeyboard();
	
		while (star_state < 4) {
			lcd_TouchDetect(&x, &y);
			button = lcd_BoxDetect(x,y);
			
			if (button != last_pressed && button != 0xFF) {
				if (button != 0x0A && button != 0x0B && button != 0xEE) {
				
					password[star_state] = button;
					star_state+=1;
					lcd_DisplayPinState(star_state);
					last_pressed = button;
				} // if (button != 0x0A && button != 0x0B) 
				
				if (button == 0x0B && star_state != 0) {
					star_state-=1;
					password[star_state] = 0xFF;
					lcd_DisplayPinState(star_state);
					last_pressed = button;
				} // if (button == 0xB)
				
				if (button == 0xEE) {
					last_pressed = button;
				} // if (button == 0xEE)
				
				if (button == 0x0A) return 0x00;
				
				

			} // if (button != last_pressed && button != 0xFF)
		HAL_Delay(5);
		} // while(1)
		return 0x01;		
}

/**
   * @brief Handles whole input password process
   * @param password: pointer to scanned card ID
   * @retval None
   */
void state_CheckToken(uint8_t* CardID) {
	
		uint8_t password[4] = {0,0,0,0};
		uint8_t input_password[4] = {0,0,0,0};
		uint8_t enable_password = 1;
		uint8_t status = 0;
		
		// Check if masterCard
		status = checkMasterCard(CardID, password, &enable_password);
		if (status != 2) status = searchDb(CardID, password, &enable_password);
		
		if (status == 2) {
			
			state_PasswordInput(input_password);			
			if (passwordCheck(password, input_password) == 1) result(RESULT_MASTER_MODE, CardID);
			else result(RESULT_WRONG_PASS, CardID);
		}
		
		else if (status == 1 && enable_password == 1) {
			
			state_PasswordInput(input_password);
			if (passwordCheck(password, input_password) == 1) result(RESULT_OPEN_DOOR, CardID);
			else result(RESULT_WRONG_PASS, CardID);
		}

		else if (status == 1 && enable_password == 0) result(RESULT_OPEN_DOOR, CardID);
		
		else result(RESULT_WRONG_TOKEN, CardID);
}

/**
   * @brief Search EEPROM for scanned cardID
   * @param cardID: pointer to scanned card ID
   * @param password: pointer to protection password
   * @param enable_password: pointer to enable_password (stores if user wants to protect tag with password)
   * @retval state: returns 1 if card found in bd or 0 if not
   */
uint8_t searchDb(uint8_t* CardID, uint8_t* password, uint8_t* enable_password) {
	
	uint8_t temp_data;
	
	// Search db
	for (int search_radius = 1; search_radius <= 20; search_radius++) {
	
		// Search db for 7E value
		ReadEEPROM(&temp_data, 1, (search_radius*16)+1);
		if (temp_data == 0x7E) {
			ReadEEPROM(&temp_data, 1, (search_radius*16)+2);
			if (temp_data == CardID[0]) {
				ReadEEPROM(&temp_data, 1, (search_radius*16)+3);
				if (temp_data == CardID[1]) { 
					ReadEEPROM(&temp_data, 1, (search_radius*16)+4);
					if (temp_data == CardID[2]) { 
						ReadEEPROM(&temp_data, 1, (search_radius*16)+5);
						if (temp_data == CardID[3]) { 
							ReadEEPROM(&temp_data, 1, (search_radius*16)+6);
							if (temp_data == CardID[4]) {
								// Found CardID in db!
								// Read password status
								ReadEEPROM(&temp_data, 1, (search_radius*16)+7);
								*enable_password = temp_data;
								// Read password
								ReadEEPROM(&temp_data, 1, (search_radius*16)+8);
								password[0] = temp_data;
								ReadEEPROM(&temp_data, 1, (search_radius*16)+9);
								password[1] = temp_data;
								ReadEEPROM(&temp_data, 1, (search_radius*16)+10);
								password[2] = temp_data;
								ReadEEPROM(&temp_data, 1, (search_radius*16)+11);
								password[3] = temp_data;
								return 1;
							}	
						}		
					}
				}
			}
		}
	}
	return 0;
}

/**
   * @brief Check if MasterCard was scanned
   * @param cardID: pointer to scaned id card
   * @param password: pointer to protection password
   * @param enable_password: pointer to enable_password (stores if user wants to protect tag with password)
   * @retval state: returns 2 if master card was scanned or 0 if not
   */
uint8_t checkMasterCard(uint8_t* CardID, uint8_t* password, uint8_t* enable_password) {
	
	if (CardID[0] == MasterID[0] && 
			CardID[1] == MasterID[1] && 
			CardID[2] == MasterID[2] && 
			CardID[3] == MasterID[3] && 
			CardID[4] == MasterID[4]) {
			
			password[0] = MasterPassword[0];
			password[1] = MasterPassword[1];	
			password[2] = MasterPassword[2];	
			password[3] = MasterPassword[3];					
				
			*enable_password = 1;
				
			return 2; 
			}

	else return 0;
}

/**
   * @brief Check if correct password for card was inputted
   * @param password: pointer to protection password
   * @param input_password: pointer to inputted password
   * @retval state: returns 1 if password inputted successfully or 0 if not
   */
uint8_t passwordCheck(uint8_t* password, uint8_t* input_password) {
	
	if (password[0] == input_password[0] && 
			password[1] == input_password[1] && 
			password[2] == input_password[2] && 
			password[3] == input_password[3]) return 1;		
	else return 0;
}

/**
   * @brief Handles result logic - what to display and do, after scanning card and inputing password
   * @param mode: this variable contains next mode, 1 is open door, 2 wrong password, 3 wrong token scanned and 4 master mode
   * @param CardID: pointer scanned card id
   * @retval None
   */
void result(uint8_t mode, uint8_t* CardID) {
	
		if (mode == 4) {	
			state_MasterMode();
			sendUartPacketStatus(RESULT_MASTER_MODE, CardID);
		}
		else {
			
			lcd_ClearScreen();
			lcd_DisplayHeader();	
			
			if (mode == 1) { // Open door
				sendUartPacketStatus(RESULT_OPEN_DOOR, CardID);
				lcd_DisplayTokenDetectedValid(CardID);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
				door_flag = 0;
			}			
			
			if (mode == 2) { // Wrong password
				sendUartPacketStatus(RESULT_WRONG_PASS, CardID);
				lcd_DisplayInvalidPassword();
			}			
			
			if (mode == 3) { // Wrong token
				sendUartPacketStatus(RESULT_WRONG_TOKEN, CardID);
				lcd_DisplayTokenDetectedInvalid();
			}
		}
			
		interrupt_flag = 1;
}

/**
   * @brief Handles masterMode (menu)
   * @param None
   * @retval None
   */
void state_MasterMode(void) {
	
	HAL_Delay(20);
	lcd_DisplayMasterMode();
	sendUartDb();
	uint32_t x = 0;
	uint32_t y = 0;
	uint8_t end = 1;

	
	while (end) {
		lcd_TouchDetect(&x, &y);
	
		if (x < 250) {
			if (y > 24 && y < 95) {
				state_AddTokenDb();
				end = 0;
			}
			if (y > 120 && y < 191) {
				state_DeleteTokenDb();
				end = 0;
			} 
			if (y > 216 && y < 287) end = 0;
		}
	}
	
		lcd_ClearScreen();
		lcd_DisplayHeader();
		lcd_DisplayNoToken();
}

/**
   * @brief Handles sending status bytes to PC
   * @param status: status of scanned token, 1 is open door, 2 wrong password, 3 wrong token scanned and 4 master mode
   * @param CardID: pointer scanned card id
   * @retval None
   */
void sendUartPacketStatus(uint8_t status, uint8_t* CardID) {

	uint8_t buf = 0x01;
	uartTxHexx(&buf);
	uartTxHexx(&status);
	for (int x = 0; x <= 4; x++) {
		buf = CardID[x];
		uartTxHexx(&buf);
	}
}

/**
   * @brief Handles adding new ID to EEPROM - logic
   * @param None
   * @retval None
   */
void state_AddTokenDb(void){
	
	HAL_Delay(20);
	lcd_AddNewToken();
	uint8_t end = 1;
	uint8_t NewCardID[5];
	uint32_t x = 0;
	uint32_t y = 0;
	uint8_t password[4] = {0,0,0,0};
	uint8_t password_status = 0x00;
	
	
	while(end) {
		if (MFRC522_Check(NewCardID) == MI_OK) {
				if (searchInDb(NewCardID) == 0) {
					password_status = state_PasswordInput(password);
					addDb(NewCardID, password, password_status);
				}
				else {
					lcd_ClearScreen();
					lcd_DisplayHeader();
					lcd_tokenAlreadyInDb();
					interrupt_flag = 1;
				}
			end = 0;
			}
		lcd_TouchDetect(&x, &y);
		if (x < 250 && x > 0 && y > 0 && y < 330) end = 0;		
	}
}

/**
   * @brief Handles deleting ID from EEPROM - logic
   * @param None
   * @retval None
   */
void state_DeleteTokenDb(void) {
	
	HAL_Delay(20);
	lcd_DeleteToken();
	uint8_t end = 1;
	uint8_t NewCardID[5];
	uint32_t x = 0;
	uint32_t y = 0;
	
	while(end) {
		if (MFRC522_Check(NewCardID) == MI_OK) {			
			deleteDb(NewCardID);
			end = 0;
		}	
		lcd_TouchDetect(&x, &y);
		if (x < 250 && x > 0 && y > 0 && y < 330) end = 0;		
	}
	
}

/**
   * @brief Handles deleting ID from EEPROM - searching and deleting
   * @param CardID: pointer scanned card id
   * @retval None
   */
void deleteDb(uint8_t* CardID) {
	
	uint8_t temp_data;
	
	// Search db
	
	for (int search_radius = 1; search_radius <= 20; search_radius++) {
	
		// Search db for 7E value
		ReadEEPROM(&temp_data, 1, (search_radius*16)+1);
		if (temp_data == 0x7E) {
			ReadEEPROM(&temp_data, 1, (search_radius*16)+2);
			if (temp_data == CardID[0]) {
				ReadEEPROM(&temp_data, 1, (search_radius*16)+3);
				if (temp_data == CardID[1]) { 
					ReadEEPROM(&temp_data, 1, (search_radius*16)+4);
					if (temp_data == CardID[2]) { 
						ReadEEPROM(&temp_data, 1, (search_radius*16)+5);
						if (temp_data == CardID[3]) { 
							ReadEEPROM(&temp_data, 1, (search_radius*16)+6);
							if (temp_data == CardID[4]) {
								// Found CardID in db!
								// Now, delete card 
								temp_data = 0xFF;
								WriteEEPROM(&temp_data, 1, ((search_radius)*16)+1);
								WriteEEPROM(&temp_data, 1, ((search_radius)*16)+2);
								WriteEEPROM(&temp_data, 1, ((search_radius)*16)+3);
								WriteEEPROM(&temp_data, 1, ((search_radius)*16)+4);
								WriteEEPROM(&temp_data, 1, ((search_radius)*16)+5);
								WriteEEPROM(&temp_data, 1, ((search_radius)*16)+6);
								WriteEEPROM(&temp_data, 1, ((search_radius)*16)+7);
								WriteEEPROM(&temp_data, 1, ((search_radius)*16)+8);
								WriteEEPROM(&temp_data, 1, ((search_radius)*16)+9);
								WriteEEPROM(&temp_data, 1, ((search_radius)*16)+10);
								WriteEEPROM(&temp_data, 1, ((search_radius)*16)+11);
							}	
						}		
					}
				}
			}
		}
	}
}

/**
   * @brief Handles adding new ID to EEPROM - searching for free spot and adding new token
   * @param CardID: pointer scanned card id
   * @retval None
   */
uint8_t addDb(uint8_t* CardID, uint8_t* password, uint8_t password_status) {
	
	uint8_t temp_data;
	
	// Search db
	
	for (int search_radius = 1; search_radius <= 20; search_radius++) {
	
		// Search db for 7E value
		ReadEEPROM(&temp_data, 1, (search_radius*16)+1);
		if (temp_data != 0x7E) {
			// Free spot! Now, add new card
			temp_data = 0x7E;
			WriteEEPROM(&temp_data, 1, ((search_radius)*16)+1);	
			// CardID
			temp_data = CardID[0];
			WriteEEPROM(&temp_data, 1, ((search_radius)*16)+2);	
			temp_data = CardID[1];
			WriteEEPROM(&temp_data, 1, ((search_radius)*16)+3);	
			temp_data = CardID[2];
			WriteEEPROM(&temp_data, 1, ((search_radius)*16)+4);	
			temp_data = CardID[3];
			WriteEEPROM(&temp_data, 1, ((search_radius)*16)+5);	
			temp_data = CardID[4];
			WriteEEPROM(&temp_data, 1, ((search_radius)*16)+6);	
			// PE
			temp_data = password_status;
			WriteEEPROM(&temp_data, 1, ((search_radius)*16)+7);
			// Pin
			temp_data = password[0];
			WriteEEPROM(&temp_data, 1, ((search_radius)*16)+8);
			temp_data = password[1];
			WriteEEPROM(&temp_data, 1, ((search_radius)*16)+9);
			temp_data = password[2];
			WriteEEPROM(&temp_data, 1, ((search_radius)*16)+10);
			temp_data = password[3];
			WriteEEPROM(&temp_data, 1, ((search_radius)*16)+11);

			return 1;
		}
	}
	return 0;
}

/**
   * @brief Handles sending data from EEPROM to PC
   * @param None
   * @retval None
   */
void sendUartDb(void) {
	
	uint8_t data;
	uint8_t buf;
	
	for (int xyz = 1; xyz <= (16*10); xyz++) {
		ReadEEPROM(&buf, 1, xyz);
		data = buf;
		uartTxHexx(&data);
		HAL_Delay(2);
	}
}

/**
   * @brief Check existance of ID card in db
   * @param CardID: pointer scanned card id
   * @retval status: returns 1 if card found and 0 if not
   */
uint8_t searchInDb(uint8_t* CardID) {
	
	uint8_t temp_data;
	
	// Search db
	for (int search_radius = 1; search_radius <= 20; search_radius++) {
	
		// Search db for 7E value
		ReadEEPROM(&temp_data, 1, (search_radius*16)+1);
		if (temp_data == 0x7E) {
			ReadEEPROM(&temp_data, 1, (search_radius*16)+2);
			if (temp_data == CardID[0]) {
				ReadEEPROM(&temp_data, 1, (search_radius*16)+3);
				if (temp_data == CardID[1]) { 
					ReadEEPROM(&temp_data, 1, (search_radius*16)+4);
					if (temp_data == CardID[2]) { 
						ReadEEPROM(&temp_data, 1, (search_radius*16)+5);
						if (temp_data == CardID[3]) { 
							ReadEEPROM(&temp_data, 1, (search_radius*16)+6);
							if (temp_data == CardID[4]) {
								// Found CardID in db!
								return 1;
							}	
						}		
					}
				}
			}
		}
	}
	return 0;
}

