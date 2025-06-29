#include "main.h"

int main(void) {
    HAL_Init();
    SystemClock_Config();
    UART_Init();              // sets up USART2 @115200, IRQ enabled
    Protocol_Init();          // resets state, enables RX interrupt

    uint8_t rsp_payload[] = "HELLO";
    while (1) {
        if (Protocol_PacketReceived()) {
            Packet_t pkt;
            Protocol_GetPacket(&pkt);
            // echo back with ACK command (0x10)
            Protocol_SendPacket(0x10, rsp_payload, sizeof(rsp_payload)-1);
        }
        HAL_Delay(10);
    }
}

// configure HCLK=84MHz, PCLK1=42MHz for UART timing
void SystemClock_Config(void) {
    RCC_OscInitTypeDef    osc = {0};
    RCC_ClkInitTypeDef    clk = {0};
    osc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    osc.HSIState       = RCC_HSI_ON;
    osc.PLL.PLLState   = RCC_PLL_ON;
    osc.PLL.PLLSource  = RCC_PLLSOURCE_HSI;
    osc.PLL.PLLM       = 16;
    osc.PLL.PLLN       = 336;
    osc.PLL.PLLP       = RCC_PLLP_DIV4;
    HAL_RCC_OscConfig(&osc);
    clk.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                       | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clk.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    clk.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    clk.APB1CLKDivider = RCC_HCLK_DIV2;
    clk.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_2);
}
