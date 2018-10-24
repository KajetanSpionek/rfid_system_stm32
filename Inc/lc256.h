/*!
* \file LCD256.h
* \author Kajetan Spionek
* Handles connection, reading and writing data on EEPROM memory
*/


#include "stm32f4xx_hal.h"
#include "uart.h"
#include "stm32f4xx_hal_gpio.h"

#define LC256_CS_LOW HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define LC256_CS_HIGH HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)
extern SPI_HandleTypeDef hspi4;


#define READ 0x03
#define WRITE 0x02
#define WRDI 0x04
#define WREN 0x06
#define RDSR 0x05
#define WRSR 0x01

void WriteEEPROM(uint8_t *txBuffer, uint16_t size, uint16_t addr);
void ReadEEPROM(uint8_t *rxBuffer, uint16_t size, uint16_t addr);


