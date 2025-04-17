# See: https://makefiletutorial.com/

# Board selection: MotorBoard or SensorBoard
BOARD ?= MotorBoard

# Cross-compiler and flags
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CPU_FLAGS = \
    -mcpu=cortex-m4

FPU_FLAGS = \
    -mfpu=fpv4-sp-d16 \
    -mfloat-abi=hard

DEFINES = \
    -DPART_TM4C123GH6PM \
    -DTARGET_IS_BLIZZARD_RA1 \
    -Dgcc

CFLAGS = \
    $(FPU_FLAGS) \
    $(DEFINES) \
    -mthumb \
    $(CPU_FLAGS) \
    -ffunction-sections \
    -fdata-sections \
    -MD \
    -std=c99 \
    -Wall \
    -pedantic \
    -O0 \
    -g3 \
    -Wno-unused-but-set-variable

LDFLAGS = \
    $(FPU_FLAGS) \
    $(DEFINES) \
    -fno-builtin-printf \
    -mthumb \
    $(CPU_FLAGS) \
    -Wl,--gc-sections \
    -T gcc-files/tm4c123gh6pm.ld

# Source files
SRC_FILES = \
	gcc-files/startup_gcc.c \
	FreakyRTOS/OS.c \
	FreakyRTOS/osasm_gcc.s \
	FreakyRTOS/OS_Lists.c \
	FreakyRTOS/OS_LaunchPad.c \
	FreakyRTOS/OS_Fifo.c \
	FreakyRTOS/OS_MailBox.c \
	FreakyRTOS/OS_Sema4.c \
	FreakyRTOS/ADC.c \
	FreakyRTOS/ST7735.c \
	FreakyRTOS/UART0int.c \
	FreakyRTOS/Interpreter.c \
	FreakyRTOS/FIFOsimple.c \
	FreakyRTOS/eDisk.c \
	FreakyRTOS/can0.c \
	FreakyRTOS/OS_CLI_Commands/cat.c \
	FreakyRTOS/OS_CLI_Commands/CLI_Commands.c \
	FreakyRTOS/OS_CLI_Commands/write.c \
	FreakyRTOS/OS_CLI_Commands/ls.c \
	FreakyRTOS/OS_CLI_Commands/touch.c \
	FreakyRTOS/OS_CLI_Commands/rm.c \
	FreakyRTOS/OS_CLI_Commands/pwd.c \
	FreakyRTOS/OS_CLI_Commands/cd.c \
	FreakyRTOS/OS_CLI_Commands/mount.c \
	FreakyRTOS/OS_CLI_Commands/umount.c \
	FreakyRTOS/OS_CLI_Commands/mkfs.c \
	FreakyRTOS/OS_CLI_Commands/mkdir.c \
	driverlib/interrupt.c \
	inc/Timer0A.c \
	inc/Timer1A.c \
	inc/Timer2A.c \
	inc/Timer3A.c \
	inc/Timer4A.c \
	inc/Timer5A.c \
	inc/PLL.c \
	inc/ADCSWTrigger.c \
	inc/LPF.c \
	inc/IRDistance.c \
	inc/ADCT0ATrigger.c \
	inc/LaunchPad.c \
	inc/can.c \
	inc/PWMrobot.c \
	lib/std/string_lite/string_lite.c \
	lib/std/stdio_lite/stdio_lite.c \
	gcc-files/STM32F10x_DSP_Lib/src/PID_stm32.s \
	gcc-files/STM32F10x_DSP_Lib/src/cr4_fft_64_stm32.s

ifeq ($(BOARD),MotorBoard)
	SRC_FILES += MotorBoard/MotorTestmain.c
	SRC_FILES += MotorBoard/ChipComms_Motor.c
	SRC_FILES += MotorBoard/Motor.c
else ifeq ($(BOARD),SensorBoard)
	SRC_FILES += SensorBoard/SensorBoardTestMain.c
	SRC_FILES += SensorBoard/ChipComms_SensorBoard.c
	SRC_FILES += SensorBoard/Sensor.c
	SRC_FILES += SensorBoard/TFLuna.c
else
	$(error TARGET must be either MotorBoard or SensorBoard)
endif

# Include paths
INCLUDES = $(sort $(foreach src_f,$(SRC_FILES),-I$(patsubst %/,%,$(dir $(src_f)))))

# Object files
OBJ_FILES = $(patsubst %.c,build/%.o,$(filter %.c,$(SRC_FILES))) \
			$(patsubst %.s,build/%.o,$(filter %.s,$(SRC_FILES)))

# Target executable
TARGET = build/tm4c123gh6pm

# Default target
all: $(TARGET).elf $(TARGET).bin

# Build the target executable
$(TARGET).elf: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

# Compile C source files
build/%.o: %.c | build
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# Compile assembly source files
build/%.o: %.s | build
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# Convert ELF to BIN
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Clean up
.PHONY: clean
clean:
	rm -rf build/*

# Flash the binary
.PHONY: flash
flash: $(TARGET).bin
	lm4flash $(TARGET).bin

.PHONY: dump
dump: all
	@echo "Dumping object files (see build directory):"
	@for obj in $(OBJ_FILES); do \
		echo "  $$obj.dump"; \
		arm-none-eabi-objdump -d $$obj > $$obj.dump; \
	done
	@arm-none-eabi-objdump -d $(TARGET).elf > $(TARGET).elf.dump

# Include dependency files
-include $(OBJ_FILES:.o=.d)

# Create build directory
build:
	mkdir -p build

help:
	@echo "Makefile for TM4C123GH6PM project"
	@echo "Usage:"
	@echo "  make [BOARD=MotorBoard|SensorBoard]"
	@echo "  make clean"
	@echo "  make flash"
	@echo ""
	@echo "Useful flags:"
	@echo "  -B		Force build all files"
	@echo "  -j		Enable parallel build"
	@echo "Options:"
	@echo "  BOARD      Select the board (MotorBoard or SensorBoard)"
	@echo ""
	@echo "Targets:"
	@echo "  all        Build the project (default target)"
	@echo "  clean      Clean up build files"
	@echo "  flash      Flash the binary to the device"
	@echo ""
	@echo "Note:"
	@echo "  - The default board is MotorBoard if not specified."
	@echo "  - Make sure to have the required tools installed (arm-none-eabi-gcc, lm4flash)."
	@echo "  - The flash target depends on the project being built, so make flash will run make all first."



