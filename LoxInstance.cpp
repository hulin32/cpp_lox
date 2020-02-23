// Copyright 2020.2.23 <Copyright hulin>

#include <string>

#include "./LoxInstance.hpp"
#include "./LoxClass.hpp"

using std::string;

LoxInstance::LoxInstance(LoxClass klass_): klass(klass_) {}

string LoxInstance::toString() {
    return klass.name + " instance";
}
