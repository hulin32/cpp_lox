
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include <exception>
#include "./Token.hpp"
#include "Expr.hpp"
#include "./AstPrinter.hpp"

using std::shared_ptr;
using std::make_shared;

int main(int argc, char const *argv[]) {
    Token token1(MINUS, "-", "", 1);
    Token token2(STAR, "*", "", 1);
    shared_ptr<Literal> literal1(new Literal("123"));
    shared_ptr<Unary> unary(new Unary(token1, literal1));
    shared_ptr<Literal> literal2(new Literal("45.67"));
    shared_ptr<Grouping> grouping(new Grouping(literal2));
    shared_ptr<Binary> binary(new Binary(unary, token2, grouping));
    shared_ptr<AstPrinter> astPrinter(new AstPrinter);
    std::cout << astPrinter->print(binary) << std::endl;
    return 0;
}
