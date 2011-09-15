# This is a makefile created by Christian Bunk
# Last update 15.09.2011

# Define C Compiler
# -mpe=mpilog
CC = mpicc -mpe=mpilog

# -DZeitmessung wenn angeschaltet wird die Zeit mit MPI_Wtime() gemessen
CFLAGS = -Wall -Wextra -DZeitmessung

# Files to be compiled
OBJECTS = $(SRC)/Main.o \
$(SRC)/File_IO.o \
$(SRC)/Terminal_IO.o \
$(SRC)/Sort.o \
$(SRC)/Communication.o \
$(SRC)/Histogram.o \

# program name 
NAME = benchmark

# bin directory
BIN = ./

# source directory
SRC = ./src

# target (merge with bin and name)
TARGET = $(BIN)/$(NAME)

install: clean compile

compile: Main.o File_IO.o Terminal_IO.o Sort.o Communication.o Histogram.o
	mkdir -p $(BIN); $(CC) $(OBJECTS) -o $(TARGET) 

Main.o: $(SRC)/Main.c
	$(CC) -c $(CFLAGS) $(SRC)/Main.c -o $(SRC)/Main.o

File_IO.o: $(SRC)/File_IO.c
	$(CC) -c $(CFLAGS) $(SRC)/File_IO.c -o $(SRC)/File_IO.o

Terminal_IO.o: $(SRC)/Terminal_IO.c
	$(CC) -c $(CFLAGS) $(SRC)/Terminal_IO.c -o $(SRC)/Terminal_IO.o

Sort.o: $(SRC)/Sort.c
	$(CC) -c $(CFLAGS) $(SRC)/Sort.c -o $(SRC)/Sort.o

Communication.o: $(SRC)/Communication.c
	$(CC) -c $(CFLAGS) $(SRC)/Communication.c -o $(SRC)/Communication.o

Histogram.o: $(SRC)/Histogram.c
	$(CC) -c $(CFLAGS) $(SRC)/Histogram.c -o $(SRC)/Histogram.o

clean:	FORCE
	rm -f $(OBJECTS); clear

FORCE:
