// Copyright 2020 <Copyright hulin>

#ifndef LOX_HPP_
#define LOX_HPP_

#include <string>
#include "./RuntimeError.hpp"
#include "./Interpreter.hpp"

using std::string;

class lox {
 public:
    static Interpreter interpreter;
    static void error(int line, string message);
    static int runScript(int argc, char const *argv[]);
    static void runtimeError(RuntimeError error);
 private:
    static bool hadError;
    static bool hadRuntimeError;
    static void runFile(string path);
    static void runPrompt();
    static void run(string source);
    static void report(int line, string where, string message);
};

#endif  // LOX_HPP_
