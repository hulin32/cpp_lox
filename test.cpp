
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
    Object obj = Object::make_bool_obj(true);
    cout << obj.type << endl;
    return 0;
}
