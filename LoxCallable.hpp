// Copyright 2020 <Copyright hulin>
#ifndef LOXCALLABLE_HPP_
#define LOXCALLABLE_HPP_

#include <vector>
#include <memory>
#include "./Token.hpp"
#include "./Interpreter.hpp"

using std::vector;
using std::shared_ptr;

class LoxCallable {
 public:
    virtual ~LoxCallable() = default;  // for derived class
    virtual int arity() = 0;
    virtual Object call(
        shared_ptr<Interpreter> interpreter,
        vector<Object> arguments) = 0;
};

#endif  //  LOXCALLABLE_HPP_
