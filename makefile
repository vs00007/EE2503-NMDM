# This is ChatGPT code. Soham, feel free to change it.

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -lm

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TEST_DIR = test

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Test files
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_FILES))

# Executable names
EXEC = current_sim
TEST_EXEC = test_exec

# Default target to build the project
all: $(EXEC)

# Linking the final executable
$(EXEC): $(OBJ_FILES) $(BUILD_DIR)/main.o
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

# Unit testing target
test: $(TEST_OBJ_FILES) $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(TEST_EXEC) $(TEST_OBJ_FILES) $(OBJ_FILES)
	./$(TEST_EXEC)

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executables
clean:
	rm -rf $(BUILD_DIR) $(EXEC) $(TEST_EXEC)

# Phony targets (not actual files)
.PHONY: all clean test
