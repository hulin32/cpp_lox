
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include <exception>
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
    Token token(MINUS, "-", "", 1);
    shared_ptr<Literal> left(new Literal("5"));
    shared_ptr<Literal> right(new Literal("6"));
    shared_ptr<Binary> binary(new Binary(left, token, right));
    shared_ptr<AstPrinter> astPrinter(new AstPrinter);
    std::cout << astPrinter->print(binary) << std::endl;
    return 0;
}
