#ifndef lox_h
#define lox_h

#include <string>

using std::string;

class lox
{
private:
    static bool hadError;
    static bool hadRuntimeError;
    static void runFile(string path);
    static void runPrompt();
    static void run(string source);
    static void error(int line, string message);
    static void report(int line, string where, string message);
public:
    static int runScript(int argc, char const *argv[]);
    lox(/* args */);
    ~lox();
};

#endif