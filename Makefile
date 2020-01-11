CXX = g++
CC = g++

all: main

lox.o:	lox.cpp lox.h

main: main.cpp lox.o

clean:
	rm -f *.o main