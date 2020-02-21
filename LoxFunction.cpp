// Copyright 2020 <Copyright hulin>

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include "./Environment.hpp"
#include "./Interpreter.hpp"
#include "./Stmt.hpp"
#include "./Token.hpp"
#include "./ReturnError.hpp"
#include "./LoxFunction.hpp"

using std::shared_ptr;
using std::vector;
using std::string;

LoxFunction::LoxFunction(
    Function declaration_,
    shared_ptr<Environment> closure_
): declaration(declaration_), closure(closure_) {}

int LoxFunction::arity() {
    return declaration.params.size();
}

Object LoxFunction::call(
    shared_ptr<Interpreter> interpreter,
    vector<Object> arguments
) {
    shared_ptr<Environment> environment(new Environment(closure));

    for (int i = 0; i < declaration.params.size(); i++) {
    environment->define(
        declaration.params[i].lexeme,
        arguments[i]);
    }

    try {
        interpreter->executeBlock(declaration.body, environment);
    } catch (ReturnError returnValue) {
        return returnValue.value;
    }
    return Object::make_nil_obj();
}

string LoxFunction::toString() {
    return "<fn " + declaration.name.lexeme + ">";
}
