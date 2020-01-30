// Copyright 2020 <Copyright hulin>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>

#include "./lox.hpp"
#include "./Scanner.hpp"
#include "./Parser.hpp"
#include "./Expr.hpp"
#include "./AstPrinter.hpp"

using std::string;
using std::stringstream;
using std::ifstream;
using std::cin;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::vector;

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
    shared_ptr<Scanner> scannerObj(new Scanner(source));
    vector<Token> tokens = scannerObj->scanTokens();
    shared_ptr<Parser> parser(new Parser(tokens));
    shared_ptr<Expr> expression = parser->parse();
    // Stop if there was a syntax error.
    if (hadError) return;
    // // For now, just print the tokens.
    // for (Token* token : tokens) {
    //     printf("%s \n", (token->toString()).c_str());
    // }
    shared_ptr<AstPrinter> astPrinter(new AstPrinter);
    cout << astPrinter->print(expression) << endl;
}
