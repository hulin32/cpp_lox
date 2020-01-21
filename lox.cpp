// Copyright 2020 <Copyright hulin>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "./lox.hpp"
#include "./scanner.hpp"


using std::string;
using std::stringstream;
using std::ifstream;
using std::cin;
using std::cout;

bool lox::hadError = false;
bool lox::hadRuntimeError = false;

int lox::runScript(int argc, const char *argv[]) {
    if (argc > 2) {
        printf("Usage: jlox [script] \n");
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }
    return 0;
}

void lox::runFile(string path) {
    std::ifstream input(path);
    std::stringstream sstr;
    while (input >> sstr.rdbuf()) {}
    run(sstr.str());

    if (hadError) exit(65);
    if (hadRuntimeError) exit(70);
}

void lox::runPrompt() {
    string input;
    while (1) {
        cout << "> ";
        getline(cin, input);
        run(input);
        hadError = false;
    }
}

void lox::error(int line, string message) {
    report(line, "", message);
}

void lox::report(int line, string where, string message) {
    printf("[line %d] Error %s : %s \n", line, where.c_str(), message.c_str());
    hadError = true;
}

void lox::run(string source) {
    scanner* scannerObj = new scanner(source);
    list<Token*> tokens = scannerObj->scanTokens();

    // For now, just print the tokens.
    for (Token* token : tokens) {
        printf("%s \n", (token->toString()).c_str());
    }
    delete scannerObj;
}
