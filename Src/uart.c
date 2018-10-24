#include "uart.h"

/**
   * @brief Send ASCII text and new line via uart
   * @param str: pointer to ASCII data to send (uint8_t values)
	 * @param size: size of data
   * @retval None
   */
void uartTx(uint8_t* str, uint8_t size) {
	
	uint8_t buf[] = "\r\n";
	size--;
	HAL_UART_Transmit_DMA(&huart1, str, size);
	HAL_Delay(1);
	HAL_UART_Transmit_DMA(&huart1,buf,sizeof(buf)-1);
	HAL_Delay(1);
}

/**
   * @brief Send ASCII text via uart
   * @param str: pointer to ASCII data to send (uint8_t values)
	 * @param size: size of data
   * @retval None
   */
void uartTxSameLine(uint8_t* str, uint8_t size) {
	
	size--;
	HAL_UART_Transmit_DMA(&huart1, str, size);
	HAL_Delay(1);
}


/**
   * @brief Send HEX value via uart
   * @param str: pointer to HEX data to send (uint8_t value)
   * @retval None
   */
void uartTxHexx(uint8_t* str) {
	
	HAL_UART_Transmit_DMA(&huart1, str, 1);
	HAL_Delay(1);
}
/**
   * @brief Send new line symbol in ASCII via uart
	 * @param None
   * @retval None
   */
void uartTxNewLine() {
	
	uint8_t buf[] = "\r\n";
	HAL_UART_Transmit_DMA(&huart1,buf,sizeof(buf)-1);
}

/**
   * @brief Send HEX token ID text via uart
	 * @param size: Pointer to token ID (uint8_t values)
   * @retval None
   */
void uartTxTag(uint8_t* str) {
	
	HAL_UART_Transmit_DMA(&huart1, str, 5);
	HAL_Delay(1);
}

