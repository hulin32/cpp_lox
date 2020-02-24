// Copyright 2020.2.23 <Copyright hulin>

#ifndef LOXINSTANCE_HPP_
#define LOXINSTANCE_HPP_

#include <string>
#include <memory>
#include <map>
#include "./LoxClass.hpp"
#include "./Token.hpp"

using std::string;
using std::map;

class LoxInstance: public std::enable_shared_from_this<LoxInstance> {
 public:
    LoxClass klass;
    map<string, Object> fields;
    Object get(Token name);
    void set(Token name, Object value);
    explicit LoxInstance(LoxClass klass_);
    string toString();
};

#endif  // LOXINSTANCE_HPP_
