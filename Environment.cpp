
#include <stdexcept>
#include <map>
#include <string>

#include "./Environment.hpp"

using std::runtime_error;
using std::map;
using std::string;

void Environment::define(string name, Object value) {
    values[name] = value;
}

Object Environment::get(Token name) {
    auto search = values.find(name.lexeme);
    if (search != values.end()) {
      return search->second;
    }
    throw runtime_error(name.toString() +
        "Undefined variable '" + name.lexeme + "'.");
}