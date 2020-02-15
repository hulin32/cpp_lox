
#include <stdexcept>
#include <map>
#include <string>

#include "./Environment.hpp"

using std::runtime_error;
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
    throw runtime_error(name.toString() +
        "Undefined variable '" + name.lexeme + "'.");
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

    throw runtime_error(name.toString() +
        "Undefined variable '" + name.lexeme + "'.");
}