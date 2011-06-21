# This is a makefile created by Christian Bunk
# Last update 02.05.2011

# Define C Compiler
CC = mpic++
# Define standard flags
CFLAGS = -Wall -Wextra -g -DLexic_Compare

# Files to be compiled
OBJECTS = $(SRC)/main.o \
$(SRC)/file_util.o \

# program name 
NAME = benchmark

# bin directory
BIN = ./

# source directory
SRC = ./src

# target (merge with bin and name)
TARGET = $(BIN)/$(NAME)

install: clean compile

compile: main.o fileutil.o
		mkdir -p $(BIN); $(CC) $(OBJECTS) -o $(TARGET) 

main.o: $(SRC)/main.cpp
		$(CC) -c $(SRC)/main.cpp -o $(SRC)/main.o

fileutil.o: $(SRC)/file_util.cpp
		$(CC) -c $(SRC)/file_util.cpp -o $(SRC)/file_util.o

clean:	FORCE
		rm -f $(OBJECTS)

FORCE:
