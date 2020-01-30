CXXFLAGS=-g -std=c++11 -Wall -pedantic
CXX = g++

all: main test

lox.o:	lox.cpp lox.hpp
Parser.o:	Parser.cpp Parser.hpp
Scanner.o:	Scanner.cpp Scanner.hpp
Token.o:	Token.cpp Token.hpp
Expr.o: Expr.hpp Expr.cpp
AstPrinter.o: AstPrinter.hpp AstPrinter.cpp Expr.o

main: main.cpp lox.o Token.o Scanner.o Parser.o AstPrinter.o Expr.o
test: test.cpp Expr.o AstPrinter.o Token.o

clean:
	rm -f *.o main