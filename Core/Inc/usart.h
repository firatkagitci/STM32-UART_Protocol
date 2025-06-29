#ifndef USART_H
#define USART_H

#include "stm32f4xx_hal.h"

void UART_Init(void);
void UART_SendByte(uint8_t b);
void UART_SendBuffer(uint8_t *buf, uint16_t len);

#endif // USART_H
