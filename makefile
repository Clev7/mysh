# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Wuninitialized -g

# Target executable
TARGET = mysh

# Source files
SRCS = mysh.c vector_str.c
OBJS = $(SRCS:.c=.o)

# Default rule: build the program
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)