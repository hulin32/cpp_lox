// Copyright 2020.2.23 <Copyright hulin>

#ifndef LOXINSTANCE_HPP_
#define LOXINSTANCE_HPP_

#include <string>
#include "./LoxClass.hpp"

using std::string;

class LoxInstance {
 public:
    LoxClass klass;
    explicit LoxInstance(LoxClass klass_);
    string toString();
};

#endif  // LOXINSTANCE_HPP_
