
#include <stdexcept>
#include <map>
#include <string>

#include "./Environment.hpp"
#include "./RuntimeError.hpp"

using std::map;
using std::string;

Environment::Environment(shared_ptr<Environment> enclosing_):
enclosing(enclosing_) {}

void Environment::define(string name, Object value) {
    values[name] = value;
}

Object Environment::get(Token name) {
    auto search = values.find(name.lexeme);
    if (search != values.end()) {
      return search->second;
    }
    if (enclosing != nullptr) {
      return enclosing->get(name);
    }
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

Object Environment::getAt(int distance, string name) {
  return ancestor(distance)->values[name];
}

void Environment::assign(Token name, Object value) {
    auto search = values.find(name.lexeme);
    if (search != values.end()) {
      search->second = value;
      return;
    }
    if (enclosing != nullptr) {
      enclosing->assign(name, value);
      return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::assignAt(int distance, Token name, Object value) {
    ancestor(distance)->values[name.lexeme] = value;
}

shared_ptr<Environment> Environment::ancestor(int distance) {
    shared_ptr<Environment> environment = shared_from_this();
    for (int i = 0; i < distance; i++) {
        environment = environment->enclosing;
    }
    return environment;
}