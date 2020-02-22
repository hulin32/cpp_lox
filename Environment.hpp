// Copyright 2020 <Copyright hulin>

#ifndef ENVIRONMENT_HPP_
#define ENVIRONMENT_HPP_

#include <map>
#include <string>
#include <memory>
#include "./Token.hpp"

using std::map;
using std::string;
using std::shared_ptr;

class Environment: public std::enable_shared_from_this<Environment> {
 public:
    Environment() = default;
    explicit Environment(shared_ptr<Environment> enclosing);
    void define(string name, Object value);
    void assign(Token name, Object value);
    Object get(Token name);
    Object getAt(int distance, string name);
    void assignAt(int distance, Token name, Object value);
    shared_ptr<Environment> ancestor(int distance);
 private:
    shared_ptr<Environment> enclosing;
    map<string, Object> values;
};

#endif  // ENVIRONMENT_HPP_
