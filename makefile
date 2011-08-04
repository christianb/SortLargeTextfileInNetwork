# This is a makefile created by Christian Bunk
# Last update 02.05.2011

# Define C Compiler
CC = mpicc

# Files to be compiled
OBJECTS = $(SRC)/fstream.o \
$(SRC)/File_IO.o \
$(SRC)/Terminal_IO.o \
$(SRC)/Sort.o \
$(SRC)/Communication.o \

# program name 
NAME = benchmark

# bin directory
BIN = ./

# source directory
SRC = ./src

# target (merge with bin and name)
TARGET = $(BIN)/$(NAME)

install: clean compile

compile: fstream.o File_IO.o Terminal_IO.o Sort.o Communication.o
	mkdir -p $(BIN); $(CC) $(OBJECTS) -o $(TARGET) 

fstream.o: $(SRC)/fstream.c
	$(CC) -c $(SRC)/fstream.c -o $(SRC)/fstream.o

File_IO.o: $(SRC)/File_IO.c
	$(CC) -c $(SRC)/File_IO.c -o $(SRC)/File_IO.o
		
Terminal_IO.o: $(SRC)/Terminal_IO.c
		$(CC) -c $(SRC)/Terminal_IO.c -o $(SRC)/Terminal_IO.o

Sort.o: $(SRC)/Sort.c
		$(CC) -c $(SRC)/Sort.c -o $(SRC)/Sort.o

Communication.o: $(SRC)/Communication.c
		$(CC) -c $(SRC)/Communication.c -o $(SRC)/Communication.o

clean:	FORCE
	rm -f $(OBJECTS); clear

FORCE:
