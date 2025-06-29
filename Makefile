TARGET = uart_proto
CC     = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -O2 -DUSE_HAL_DRIVER -DSTM32F401xE
LDFLAGS= -TSTM32F401RETx_FLASH.ld -Wl,--gc-sections
INCDIR = -ICore/Inc -IDrivers/STM32F4xx_HAL_Driver/Inc
SRC    = Core/Src/main.c Core/Src/usart.c Core/Src/protocol.c \
         Drivers/STM32F4xx_HAL_Driver/Src/*.c

all: $(TARGET).hex

$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) $(INCDIR) $^ $(LDFLAGS) -o $@

%.hex: %.elf
	arm-none-eabi-objcopy -O ihex $< $@

clean:
	rm -rf build *.o *.elf *.hex
