CROSS_COMPILE=arm-none-eabi-
#CROSS_COMPILE =/opt/OSELAS.Toolchain-2011.11.0/arm-cortexm3-eabi/gcc-4.6.2-newlib-1.19.0-binutils-2.21.1a/bin/arm-cortexm3-eabi-
#CROSS_COMPILE =/opt/arm-2011.03/bin/arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
AR=$(CROSS_COMPILE)ar
AS=$(CROSS_COMPILE)as
OC=$(CROSS_COMPILE)objcopy
OD=$(CROSS_COMPILE)objdump
SZ=$(CROSS_COMPILE)size

CFLAGS= -c -fno-common \
	-ffunction-sections \
	-fdata-sections \
	-Os \
	-mcpu=cortex-m3 -Wall \
	-mthumb

LDSCRIPT=ld/stm32_rom.ld
LDFLAGS	= --gc-sections,-T$(LDSCRIPT),-no-startup,-nostdlib,-lnosys
OCFLAGS	= -Obinary
ODFLAGS	= -S
OUTPUT_DIR = output
TARGET  = $(OUTPUT_DIR)/main

INCLUDE = -I./src/fw_lib/include \
	  -I./src/include \
	  -I./src/fw_lib/extra_added/ \
	  -I./src/freertos/include/

SRCS = 	./src/system_stm32f10x.c \
	./src/stm32f10x_it.c \
	./src/newlib_stubs.c \
	./src/fw_lib/core_cm3.c \
	./src/fw_lib/stm32f10x_rcc.c \
	./src/fw_lib/stm32f10x_gpio.c \
	./src/fw_lib/stm32f10x_usart.c \
	./src/fw_lib/extra_added/stm32f10x_systick.c \
	./src/fw_lib/misc.c \
	./src/freertos/croutine.c \
	./src/freertos/event_groups.c \
	./src/freertos/heap_1.c \
	./src/freertos/list.c \
	./src/freertos/port.c \
	./src/freertos/queue.c \
	./src/freertos/tasks.c \
	./src/freertos/timers.c \
	./src/main.c \
	./src/usart.c

OBJS=$(SRCS:.c=.o)

.PHONY : clean all

all: $(TARGET).bin  $(TARGET).list
	$(SZ) $(TARGET).elf

clean:
	-find . -name '*.o'   -exec rm {} \;
	-find . -name '*.elf' -exec rm {} \;
	-find . -name '*.lst' -exec rm {} \;
	-find . -name '*.out' -exec rm {} \;
	-find . -name '*.bin' -exec rm {} \;
	-find . -name '*.map' -exec rm {} \;

$(TARGET).list: $(TARGET).elf
	$(OD) $(ODFLAGS) $< > $(TARGET).lst

$(TARGET).bin: $(TARGET).elf
	$(OC) $(OCFLAGS) $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(OBJS) ./src/startup.o
	@$(CC) -mcpu=cortex-m3 -mthumb -Wl,$(LDFLAGS),-o$(TARGET).elf,-Map,$(TARGET).map ./src/startup.o $(OBJS)

%.o: %.c
	@echo "  CC $<"
	@$(CC) $(INCLUDE) $(CFLAGS)  $< -o $*.o

%.o: %.S
	@echo "  CC $<"
	@$(CC) $(INCLUDE) $(CFLAGS)  $< -o $*.o
