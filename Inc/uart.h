/*!
* \file uart.h
* \author Kajetan Spionek
* Handles uart connection with PC via uart-usb converter
*/

#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart1;

void uartTx(uint8_t* str, uint8_t size);
void uartTxSameLine(uint8_t* str, uint8_t size);
void uartTxHexx(uint8_t* str);
void uartTxNewLine(void);
void uartTxTag(uint8_t* str);

