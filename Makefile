CXXFLAGS=-g -std=c++11 -Wall -pedantic
CXX = g++

all: main

lox.o:	lox.cpp lox.h
scanner.o:	scanner.cpp scanner.h
token.o:	token.cpp token.h

main: main.cpp token.o lox.o scanner.o

clean:
	rm -f *.o main