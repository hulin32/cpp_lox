#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "lox.h"


using std::string;
using std::stringstream;
using std::ifstream;
using std::cin;

bool lox::hadError = false;
bool lox::hadRuntimeError = false;


lox::lox(/* args */)
{
}

lox::~lox()
{
}

int lox::runScript(int argc, const char *argv[])
{
    if (argc > 1) {
        printf("Usage: jlox [script] \n");
    } else if (argc == 1) {
        runFile(argv[0]);
    } else {
        runPrompt();
    } 
    return 0;
}

void lox::runFile(string path)
{
    std::ifstream input(path);
    std::stringstream sstr;
    while(input >> sstr.rdbuf());
    run(sstr.str());

    if (hadError) exit(65);
    if (hadRuntimeError) exit(70);
}

void lox::runPrompt()
{
    string input;
    while (1) {
        printf("> ");
        getline(cin, input);
        run(input);
        hadError = false;
    }  
}

void lox::error(int line, string message)
{
    report(line, "", message);
}

void lox::report(int line, string where, string message)
{
    printf("[line %d] Error %s : %s \n", line, where.c_str(), message.c_str());
    hadError = true;
}

void lox::run(string source)
{
    
}