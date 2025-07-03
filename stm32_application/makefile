# Makefile

# Configuration
BUILD_DIR := ./Debug
BIN_FILE := $(BUILD_DIR)/stm32_application.bin
FLASH_ADDR := 0x08008000
SERIAL := /dev/ttyUSB0
BAUD := 115200

# Default target
all: build flash serial

# Compile the project
build:
	@echo "Building project..."
	@make -C $(BUILD_DIR) -j8 all

# Flash via DFU
flash: $(BIN_FILE)
	@echo "Flashing with dfu-util..."
	@sudo dfu-util -a 0 -s $(FLASH_ADDR):leave -D $(BIN_FILE)

# Open serial monitor
serial:
	@echo "Starting serial monitor on $(SERIAL) at $(BAUD) baud..."
	@python3 -m serial.tools.miniterm $(SERIAL) $(BAUD) --raw

# Clean build directory
clean:
	@echo "Cleaning build..."
	@make -C $(BUILD_DIR) clean

.PHONY: all build flash serial clean
