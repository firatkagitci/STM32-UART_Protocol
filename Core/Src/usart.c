#include "usart.h"
#include "protocol.h"

UART_HandleTypeDef huart2;

void UART_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {
        .Pin       = GPIO_PIN_2|GPIO_PIN_3,
        .Mode      = GPIO_MODE_AF_PP,
        .Pull      = GPIO_PULLUP,
        .Speed     = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF7_USART2
    };
    HAL_GPIO_Init(GPIOA, &gpio);

    huart2.Instance          = USART2;
    huart2.Init.BaudRate     = 115200;
    huart2.Init.WordLength   = UART_WORDLENGTH_8B;
    huart2.Init.StopBits     = UART_STOPBITS_1;
    huart2.Init.Parity       = UART_PARITY_NONE;
    huart2.Init.Mode         = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    HAL_UART_Init(&huart2);

    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void UART_SendByte(uint8_t b) {
    HAL_UART_Transmit(&huart2, &b, 1, 10);
}

void UART_SendBuffer(uint8_t *buf, uint16_t len) {
    HAL_UART_Transmit(&huart2, buf, len, 100);
}

// RX IRQ: feed incoming bytes into protocol parser
void USART2_IRQHandler(void) {
    uint8_t byte;
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE)) {
        HAL_UART_Receive(&huart2, &byte, 1, 0);
        Protocol_InputByte(byte);
    }
    HAL_UART_IRQHandler(&huart2);
}
