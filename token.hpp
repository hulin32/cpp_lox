// Copyright 2020 <Copyright hulin>

#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include <string>

using std::string;
using std::to_string;

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

struct Object {
    typedef enum { Object_str, Object_num, Object_bool, Object_nil } Object_type;
    string str;
    double num;
    bool boolean;
    int* nil;
    Object_type type;
    string toString() {
      switch (type) {
        case Object_nil:
          return "nil";
        case Object_bool:
          return boolean ? "1" : "0";
        case Object_str:
          return str;
        default:
          return to_string(num);
      }
    }
    static Object make_num_obj(double num) {
      Object num_obj;
      num_obj.type = Object::Object_num;
      num_obj.num = num;
      return num_obj;
    }

    static Object make_str_obj(string str) {
      Object str_obj;
      str_obj.type = Object::Object_str;
      str_obj.str = str;
      return str_obj;
    }

    static Object make_bool_obj(bool boolean) {
      Object bool_obj;
      bool_obj.type = Object::Object_num;
      bool_obj.boolean = boolean;
      return bool_obj;
    }

    static Object make_nil_obj() {
      Object nil_obj;
      nil_obj.type = Object::Object_nil;
      nil_obj.nil = NULL;
      return nil_obj;
    }
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
