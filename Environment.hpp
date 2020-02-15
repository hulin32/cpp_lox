// Copyright 2020 <Copyright hulin>

#ifndef ENVIRONMENT_HPP_
#define ENVIRONMENT_HPP_

#include <map>
#include <string>
#include "./Token.hpp"

using std::map;
using std::string;

class Environment {
 public:
    void define(string name, Object value);
    void assign(Token name, Object value);
    Object get(Token name);
 private:
    map<string, Object> values;
};

#endif  // ENVIRONMENT_HPP_
