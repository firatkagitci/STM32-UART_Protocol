# STM32F401RE UART Packet Protocol Demo

## Overview
This project implements a simple, framed packet protocol over UART on the STM32F401RE Nucleo board. Packets are verified with CRC-16 and handled via interrupt-driven HAL drivers. Incoming commands are echoed back with an acknowledgment.

## Features
- **Framed packet structure**: `0x55 0xAA | CMD | LEN | DATA… | CRC16`  
- **CRC-16-IBM** (polynomial 0x8005) for error checking  
- **Interrupt-driven UART2** (PA2=TX, PA3=RX) using STM32Cube HAL  
- Example command–response: board replies “HELLO” with ACK (0x10)

## Hardware
- STM32F401RE Nucleo  
- USB ST-LINK for power & virtual COM port  
- Connect UART pins:  
  - PA2 → TX  
  - PA3 → RX  

## Software Requirements
- **Toolchain**: `arm-none-eabi-gcc` (or STM32CubeIDE)  
- **HAL Drivers**: STM32CubeF4 HAL (place in `Drivers/STM32F4xx_HAL_Driver/`)  
- **OpenOCD** or **st-flash** for flashing  

## Building & Flashing
1. Clone the repository.  
2. Run:
   ```bash
   make
