// Copyright 2020 <Copyright hulin>

#include <memory>
#include <string>

#include "./Token.hpp"
#include "./LoxCallable.hpp"

using std::to_string;
using std::string;
using std::shared_ptr;

string Object::toString() {
    switch (type) {
    case Object_nil:
        return "nil";
    case Object_bool:
        return boolean ? "1" : "0";
    case Object_str:
        return str;
    case Object_fun:
        return "func: ";
    default:
        return to_string(num);
    }
}

Object Object::make_num_obj(double num) {
    Object num_obj;
    num_obj.type = Object_num;
    num_obj.num = num;
    return num_obj;
}

Object Object::make_str_obj(string str) {
    Object str_obj;
    str_obj.type = Object_str;
    str_obj.str = str;
    return str_obj;
}

Object Object::make_bool_obj(bool boolean) {
    Object bool_obj;
    bool_obj.type = Object_bool;
    bool_obj.boolean = boolean;
    return bool_obj;
}

Object Object::make_nil_obj() {
    Object nil_obj;
    nil_obj.type = Object_nil;
    nil_obj.nil = nullptr;
    return nil_obj;
}

Object Object::make_fun_obj(shared_ptr<LoxCallable> function_) {
    Object nil_obj;
    nil_obj.type = Object_fun;
    nil_obj.function = function_;
    return nil_obj;
}

Token::Token(TokenType type, string lexeme, Object literal, int line)
: type(type), lexeme(lexeme), literal(literal), line(line) {}

string Token::toString() {
    return to_string(type) + " " + lexeme + " " + literal.toString();
}
