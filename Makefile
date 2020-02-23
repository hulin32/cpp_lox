CXXFLAGS=-g -std=c++11 -Wall -pedantic
CXX = g++

all: main

Token.o:	Token.cpp Token.hpp
LoxInstance.o:	LoxInstance.cpp LoxInstance.hpp
LoxClass.o:	LoxClass.cpp LoxClass.hpp
Resolver.o:	Resolver.cpp Resolver.hpp
LoxFunction.o: LoxFunction.cpp LoxFunction.hpp
Environment.o: Environment.cpp Environment.hpp
Interpreter.o:	Interpreter.cpp Interpreter.hpp
lox.o:	lox.cpp lox.hpp
Parser.o:	Parser.cpp Parser.hpp
Scanner.o:	Scanner.cpp Scanner.hpp

main: main.cpp lox.o LoxFunction.o Token.o Scanner.o Parser.o Interpreter.o Environment.o Resolver.o LoxClass.o LoxInstance.o

clean:
	rm -f *.o main