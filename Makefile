CFLAGS ?= -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
					-Wformat-truncation -fno-common -Wconversion \
					-g3 -Os -ffunction-sections -fdata-sections -I. \
					-mcpu=cortex-m0plus -mthumb $(EXTRA_CFLAGS)
# LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -u_printf_float -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=build/$@.map
SOURCES ?= main.c  hal/hal.c mpu6050/mpu6050.c stm32c0_irq.c startup.s syscalls.c
INCLUDE ?= -Ihal -Impu6050

firmware.elf: $(SOURCES)
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(INCLUDE) $(LDFLAGS) -o build/$@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

clean:
	rm -rf firmware.*
