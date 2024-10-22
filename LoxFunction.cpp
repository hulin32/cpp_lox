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
#include "./LoxInstance.hpp"

using std::shared_ptr;
using std::vector;
using std::string;

LoxFunction::LoxFunction(
    shared_ptr<Function> declaration_,
    shared_ptr<Environment> closure_,
    bool isInitializer_
):
    declaration(declaration_),
    closure(closure_),
    isInitializer(isInitializer_) {}

int LoxFunction::arity() {
    return declaration->params.size();
}

Object LoxFunction::call(
    shared_ptr<Interpreter> interpreter,
    vector<Object> arguments
) {
    shared_ptr<Environment> environment(new Environment(closure));

    for (int i = 0; i < declaration->params.size(); i++) {
    environment->define(
        declaration->params[i].lexeme,
        arguments[i]);
    }

    try {
        interpreter->executeBlock(declaration->body, environment);
    } catch (ReturnError returnValue) {
        if (isInitializer) {
            return closure->getAt(0, "this");
        }
        return returnValue.value;
    }
    if (isInitializer) {
        return closure->getAt(0, "this");
    }
    return Object::make_nil_obj();
}

string LoxFunction::toString() {
    return "<fn " + declaration->name.lexeme + ">";
}

shared_ptr<LoxFunction> LoxFunction::bind(shared_ptr<LoxInstance> instance) {
    shared_ptr<Environment> environment(new Environment(closure));
    environment->define("this", Object::make_instance_obj(instance));
    return shared_ptr<LoxFunction> (
        new LoxFunction(declaration, environment, isInitializer)
    );
}