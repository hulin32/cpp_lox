//// Copyright 2020 <Copyright hulin>
//
//#include "./lox.hpp"
//
//int main(int argc, const char *argv[]) {
//    lox::runScript(argc, argv);
//}


#include <iostream>
#include "./Token.hpp"
#include "./Expr.hpp"
#include "./AstPrinter.hpp"

int main(int argc, char const *argv[]) {
    // Expr expression = (*new Binary(
    //     *(new Unary(
    //         *(new Token(MINUS, "-", "", 1)),
    //         *(new Literal("123")))),
    //    * (new Token(STAR, "*", "", 1)),
    //     *(new Grouping(
    //         *(new Literal("45.67"))))));
    Literal literal("45.67");
    AstPrinter astPrinter;
    std::cout << astPrinter.print(&literal) << std::endl;
    return 0;
}
