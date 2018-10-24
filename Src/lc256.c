#include "lc256.h"

/**
   * @brief Writes data to EEPROM memory
   * @param txBuffer: pointer to buffer with data to send
	 * @param size: size of data
	 * @param addr: physical address in memmory
   * @retval None
   */
void WriteEEPROM(uint8_t *txBuffer, uint16_t size, uint16_t addr){

    uint8_t addrLow = addr & 0xFF;
    uint8_t addrHigh = (addr >> 8);

    uint8_t wrenInstruction = WREN; // Value : 0x06

    uint8_t buffer[32] = {WRITE, addrHigh, addrLow}; //Value : 0x02

    for(uint8_t i = 0 ; i < size ; i++){
        buffer[3+i] = txBuffer[i];
    }

    LC256_CS_LOW;
		HAL_Delay(1);
    HAL_SPI_Transmit(&hspi4, &wrenInstruction, 1, 1000);
		LC256_CS_HIGH;
		HAL_Delay(1);
		LC256_CS_LOW;
		HAL_Delay(1);
    HAL_SPI_Transmit(&hspi4, buffer, (size + 3), 1000);
		LC256_CS_HIGH;
		HAL_Delay(50);
}

/**
   * @brief Reads data from EEPROM memory
   * @param rxBuffer: pointer to buffer with recieved data
	 * @param size: size of data
	 * @param addr: physical address in memmory
   * @retval None
   */
void ReadEEPROM(uint8_t *rxBuffer, uint16_t size, uint16_t addr){

    uint8_t addrLow = addr & 0xFF;
    uint8_t addrHigh = (addr >> 8);
    uint8_t txBuffer[3] = {READ, addrHigh, addrLow};

    LC256_CS_LOW;
		HAL_Delay(1);
    HAL_SPI_Transmit(&hspi4, txBuffer, 3, 1000);
    HAL_SPI_Receive(&hspi4, rxBuffer, size, 1000);    
    LC256_CS_HIGH;
		HAL_Delay(1);
}


