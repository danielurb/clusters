#
# A simple makefile for compiling a c++ project
#
SRC=./
BUILD=./build
GCC = gcc
CFLAGS = -o $(BUILD)/main.out -Wpedantic -Wall -Wextra -pedantic -std=gnu99 -Werror
RM = rm -rf

all: clean default test

default: main.out

main.out:
	$(GCC) $(CFLAGS) main.c clusters.c -lm ## -lm link math library

test: main.out
	$(BUILD)/main.out img1.pbm

clean:
	$(RM) $(BUILD)/*.out

memcheck:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes  ./build/main.out img1.pbm