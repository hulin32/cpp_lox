// Copyright 2020 <Copyright hulin>

#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include <string>
#include <memory>

using std::string;
using std::shared_ptr;

class LoxCallable;
class LoxInstance;
class LoxClass;

typedef enum {
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  TOKEN_EOF
} TokenType;

class Object {
 public:
    typedef enum {
      Object_str,
      Object_num,
      Object_bool,
      Object_nil,
      Object_fun,
      Object_instance,
      Object_class,
    } Object_type;
    string str;
    double num;
    bool boolean;
    int* nil;
    Object_type type;
    string toString();
    shared_ptr<LoxCallable> function;
    shared_ptr<LoxInstance> instance;
    shared_ptr<LoxClass> lox_class;
    static Object make_num_obj(double num);
    static Object make_str_obj(string str);
    static Object make_bool_obj(bool boolean);
    static Object make_nil_obj();
    static Object make_fun_obj(shared_ptr<LoxCallable> function_);
    static Object make_instance_obj(shared_ptr<LoxInstance> instance_);
    static Object make_class_obj(shared_ptr<LoxClass> lox_class_);
};

class Token {
 public:
    Token(TokenType type, string lexeme, Object literal, int line);
    string toString();
    TokenType type;
    string lexeme;
    Object literal;
    int line;
};

#endif  // TOKEN_HPP_
