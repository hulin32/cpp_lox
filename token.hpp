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

typedef string Object;

class Token {
 public:
    Token(TokenType type, string lexeme, Object literal, int line);
    string toString();
    TokenType type;
    string lexeme;
    string literal;
    int line;
};

#endif  // TOKEN_HPP_

