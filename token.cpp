// Copyright 2020 <Copyright hulin>

#include "./Token.hpp"

Token::Token(TokenType type, string lexeme, Object literal, int line)
: type(type), lexeme(lexeme), literal(literal), line(line) {}

string Token::toString() {
    return to_string(type) + " " + lexeme + " " + literal;
}