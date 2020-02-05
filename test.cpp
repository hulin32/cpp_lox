
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include <exception>
#include "./Token.hpp"
#include "./Expr.hpp"
#include "./AstPrinter.hpp"

using std::shared_ptr;
using std::cout;
using std::endl;

int main(int argc, char const *argv[]) {
    Token token1(MINUS, "-", Object::make_str_obj(""), 1);
    Token token2(STAR, "*", Object::make_str_obj(""), 1);
    shared_ptr<Literal<string>> literal1(new Literal<string>("123"));
    shared_ptr<Unary<string>> unary(new Unary<string>(token1, literal1));
    shared_ptr<Literal<string>> literal2(new Literal<string>("45.67"));
    shared_ptr<Grouping<string>> grouping(new Grouping<string>(literal2));
    shared_ptr<Binary<string>> binary(new Binary<string>(unary, token2, grouping));
    shared_ptr<AstPrinter> astPrinter(new AstPrinter);
    std::cout << astPrinter->print(binary) << std::endl;
    return 0;
}
