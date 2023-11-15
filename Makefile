# A Makefile to build my compiler (main.c)
# and run it to compile SRC/test.ent into
# BUILD/test.asm and then assemble it with NASM
# and compile it using GNU LD
CC = gcc
ASM = nasm
LINK = ld
ROOT = .
SRC_DIR = $(ROOT)/compiler
BUILD_DIR = $(ROOT)/build
# Name of file we compiling
COMPILE = testing/test

# Define ANSI escape codes for colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m # No Color

# List all source files in the compiler directory
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
# Generate corresponding object file names
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

.PHONY: all compiler clean

all: $(BUILD_DIR) compiler #$(ROOT)/build/$(COMPILE)

compiler: $(OBJ_FILES) $(ROOT)/ent

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(ROOT)/ent: $(OBJ_FILES)
	@echo -e "$(GREEN)Linking $@$(NC)"
	$(CC) -o $@ $^

# Pattern rule to compile .c files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo -e "$(GREEN)Compiling $@$(NC)"
	$(CC) -c -o $@ $<

$(BUILD_DIR)/$(COMPILE): $(ROOT)/$(COMPILE).ent
	@echo -e "$(YELLOW)Compiling...$(NC)"
	$(ROOT)/ent --compile-only $< $(BUILD_DIR)/$(COMPILE).asm
	@echo -e "$(YELLOW)Assembling...$(NC)"
	$(ASM) -f elf -o $(BUILD_DIR)/$(COMPILE).o $(BUILD_DIR)/$(COMPILE).asm
	@echo -e "$(YELLOW)Linking...$(NC)"
	$(CC) -m32 -o $@ $(BUILD_DIR)/$(COMPILE).o

clean:
	rm -rf $(ROOT)/ent $(ROOT)/ent.o $(BUILD_DIR)
	@clear
