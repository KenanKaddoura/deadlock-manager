# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -pthread

# Target executable name
TARGET = deadlock_manager

# Directories
SRC_DIR = src

# Source files and Object files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/banker.c $(SRC_DIR)/simulator.c
OBJS = $(SRCS:.c=.o)

# Default rule to build the system
all: $(TARGET)

# Rule to link the object files and create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile C source files into object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up the compiled files
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

# Phony targets to prevent conflicts with files named 'all' or 'clean'
.PHONY: all clean