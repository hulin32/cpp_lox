// Copyright 2020.2.23 <Copyright hulin>

#ifndef LOXCLASS_HPP_
#define LOXCLASS_HPP_

#include <string>
#include <memory>
#include <vector>
#include "./LoxCallable.hpp"
#include "./Interpreter.hpp"
#include "./Token.hpp"

using std::string;
using std::shared_ptr;
using std::vector;

class LoxClass: public LoxCallable {
 public:
    string name;
    explicit LoxClass(string name_);
    Object call(
        shared_ptr<Interpreter> interpreter,
        vector<Object> arguments);
    int arity();
    string toString();
};

#endif  // LOXCLASS_HPP_
