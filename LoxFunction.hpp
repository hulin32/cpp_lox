// Copyright 2020 <Copyright hulin>

#ifndef LOXFUNCTION_HPP_
#define LOXFUNCTION_HPP_

#include <memory>
#include <vector>
#include <string>
#include "./LoxCallable.hpp"
#include "./Interpreter.hpp"
#include "./Stmt.hpp"
#include "./Token.hpp"

using std::shared_ptr;
using std::vector;
using std::string;

class LoxFunction: public LoxCallable {
 public:
    Function declaration;

    explicit LoxFunction(Function declaration_);

    int arity();

    Object call(
      shared_ptr<Interpreter> interpreter,
      vector<Object> arguments
    );

    string toString();
};

#endif  // LOXFUNCTION_HPP_
