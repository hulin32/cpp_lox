// Copyright 2020.2.23 <Copyright hulin>

#ifndef LOXCLASS_HPP_
#define LOXCLASS_HPP_

#include <string>
#include <memory>
#include <vector>
#include <map>
#include "./LoxCallable.hpp"
#include "./Interpreter.hpp"
#include "./LoxFunction.hpp"
#include "./Token.hpp"

using std::string;
using std::shared_ptr;
using std::vector;
using std::map;

class LoxClass: public LoxCallable {
 public:
    string name;
    map<string, shared_ptr<LoxFunction>> methods;
    shared_ptr<LoxFunction> findMethod(string name);
    explicit LoxClass(
        string name_,
        map<string, shared_ptr<LoxFunction>> methods_);

    Object call(
        shared_ptr<Interpreter> interpreter,
        vector<Object> arguments);
    int arity();
    string toString();
};

#endif  // LOXCLASS_HPP_
