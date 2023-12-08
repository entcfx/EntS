CC = g++
ASM = nasm
ROOT = .
SRC_DIR = $(ROOT)/compiler
BUILD_DIR = $(ROOT)/build

# Define ANSI escape codes for colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m # No Color

# List all source files in the compiler directory
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
# Generate corresponding object file names
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

.PHONY: all compiler clean

all: $(BUILD_DIR) compiler

compiler: $(OBJ_FILES) $(ROOT)/ent


$(ROOT)/ent: $(OBJ_FILES)
	@echo -e "$(GREEN)Linking $@$(NC)"
	$(CC) -o $@ $^


# Pattern rule to compile .cpp files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo -e "$(GREEN)Compiling $@$(NC)"
	$(CC) -c -o $@ $<


clean:
	rm -rf $(ROOT)/ent $(BUILD_DIR)
	@clear

reset:
	$(MAKE) clean
	$(MAKE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
