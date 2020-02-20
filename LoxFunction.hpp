// Copyright 2020 <Copyright hulin>

#ifndef LOXFUNCTION_HPP_
#define LOXFUNCTION_HPP_

#include <memory>
#include <vector>
#include <string>
#include "./LoxCallable.hpp"
#include "./Interpreter.hpp"
#include "./Environment.hpp"
#include "./Stmt.hpp"
#include "./Token.hpp"

using std::shared_ptr;
using std::vector;
using std::string;

class LoxFunction: public LoxCallable {
 public:
    Function declaration;
    shared_ptr<Environment> closure;
    explicit LoxFunction(Function declaration_, shared_ptr<Environment> closure_);

    int arity();

    Object call(
      shared_ptr<Interpreter> interpreter,
      vector<Object> arguments
    );

    string toString();
};

#endif  // LOXFUNCTION_HPP_
