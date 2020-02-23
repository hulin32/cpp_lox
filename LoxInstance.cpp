// Copyright 2020.2.23 <Copyright hulin>

#include <string>

#include "./LoxInstance.hpp"
#include "./LoxClass.hpp"
#include "./RuntimeError.hpp"
#include "./Token.hpp"

using std::string;

LoxInstance::LoxInstance(LoxClass klass_): klass(klass_) {}

string LoxInstance::toString() {
    return klass.name + " instance";
}

Object LoxInstance::get(Token name) {
    auto searched = fields.find(name.lexeme);
    if (searched != fields.end()) {
        return searched->second;
    }

    shared_ptr<LoxFunction> method = klass.findMethod(name.lexeme);
    if (method != nullptr) {
        return Object::make_fun_obj(method);
    }

    throw RuntimeError(name,
        "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(Token name, Object value) {
    fields[name.lexeme] = value;
}