# Makefile for SDL2 program with math library linked

# Compiler and flags
CC = gcc
CFLAGS = -Wall  -pedantic
LIBS = `sdl2-config --cflags --libs` -lm

# Target executable
TARGET = WPlayer

# Source files
SRCS = main2.c rendering.c raycasting.c render.h

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

# Compile source files into object files
%.o: %.c
	$(CC) -I. $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)
