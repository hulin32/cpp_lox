CXXFLAGS=-g -std=c++11 -Wall -pedantic
CXX = g++

all: main test demo

Environment.o: Environment.cpp Environment.hpp
Interpreter.o:	Interpreter.cpp Interpreter.hpp Environment.o
lox.o:	lox.cpp lox.hpp Interpreter.o
Parser.o:	Parser.cpp Parser.hpp
Scanner.o:	Scanner.cpp Scanner.hpp
Token.o:	Token.cpp Token.hpp
AstPrinter.o: AstPrinter.hpp AstPrinter.cpp

main: main.cpp lox.o Token.o Scanner.o Parser.o AstPrinter.o Interpreter.o Environment.o
test: test.cpp AstPrinter.o Token.o
demo: demo.cpp

clean:
	rm -f *.o main