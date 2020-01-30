// Copyright 2020 <Copyright hulin>

#include <iostream>
#include "./Token.hpp"
#include "./Expr.hpp"
#include "./AstPrinter.hpp"
#include "./lox.hpp"

int main(int argc, char const *argv[]) {
    lox::runScript(argc, argv);
    return 0;
}
