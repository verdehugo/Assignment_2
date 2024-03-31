# Paulo Pedreiras, Mar/2022
# Adapted from the ThrowTheSwitch.org   
# 
# This makefile assumes that test source and test files are in folders 
#   "src" and "test", respectively
# Moreover, this makefile, "src" and "test" are within the root Unity 
#   folder.
# Finaly, the SW module name has a MODULE_NAME.c file and MODULE_NAME.h
#   file, and the test file is testMODULENAME.c
# If the rules above are obeyed only MODULE_NAME needs to be adjusted

MODULE_NAME = cmdproc

# Paths
UNITY_ROOT = ..
SRC_FOLDER = src
TEST_FOLDER = test

# Commands
CLEANUP = rm -f
MKDIR = mkdir -p

# Compiler
C_COMPILER = gcc
CFLAGS = -std=c89 -Wall -Wextra -Wpointer-arith -Wcast-align -Wwrite-strings \
          -Wswitch-default -Wunreachable-code -Winit-self -Wmissing-field-initializers \
          -Wno-unknown-pragmas -Wstrict-prototypes -Wundef -Wold-style-definition

# Include directories
INC_DIRS = -I$(SRC_FOLDER) -I$(UNITY_ROOT)/src

# Target executable
TARGET = test$(MODULE_NAME)

# Source files
SRC_FILES = $(UNITY_ROOT)/src/unity.c $(SRC_FOLDER)/$(MODULE_NAME).c $(TEST_FOLDER)/test$(MODULE_NAME).c

# Object files
OBJ_FILES = $(SRC_FOLDER)/$(MODULE_NAME).o $(UNITY_ROOT)/src/unity.o

# Default rule
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJ_FILES)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $^ -o $@

# Rule to compile source files into object files
$(OBJ_FILES): $(SRC_FILES)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) -c $< -o $@

# Rule to run the tests
test: $(TARGET)
	./$(TARGET)

# Clean rule
clean:
	$(CLEANUP) $(TARGET) $(OBJ_FILES)

