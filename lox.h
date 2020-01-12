// Copyright 2020 <Copyright hulin>

#ifndef LOX_H_
#define LOX_H_

#include <string>

using std::string;

class lox {
 private:
    static bool hadError;
    static bool hadRuntimeError;
    static void runFile(string path);
    static void runPrompt();
    static void run(string source);
    static void report(int line, string where, string message);
 public:
    static void error(int line, string message);
    static int runScript(int argc, char const *argv[]);
};

#endif  // LOX_H_
