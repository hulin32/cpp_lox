
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include "./token.hpp"
#include "Expr.hpp"
#include "./AstPrinter.hpp"

using std::shared_ptr;
using std::make_shared;

int main(int argc, char const *argv[]) {
    // Expr expression = (*new Binary(
    //     *(new Unary(
    //         *(new Token(MINUS, "-", "", 1)),
    //         *(new Literal("123")))),
    //    * (new Token(STAR, "*", "", 1)),
    //     *(new Grouping(
    //         *(new Literal("45.67"))))));
    shared_ptr<Literal> literal(new Literal("22.1"));
    AstPrinter astPrinter;
    std::cout << astPrinter.print(literal) << std::endl;
    return 0;
}
