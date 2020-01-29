CXXFLAGS=-g -std=c++11 -Wall -pedantic
CXX = g++

all: main test

lox.o:	lox.cpp lox.hpp
scanner.o:	scanner.cpp scanner.hpp
token.o:	token.cpp token.hpp
Expr.o: Expr.hpp Expr.cpp
AstPrinter.o: AstPrinter.hpp AstPrinter.cpp Expr.o

main: main.cpp token.o lox.o scanner.o
test: test.cpp Expr.o AstPrinter.o token.o

clean:
	rm -f *.o main