// Copyright 2020-1-29 <Copyright hulin>

/*
expression     → equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → addition ( ( ">" | ">=" | "<" | "<=" ) addition )* ;
addition       → multiplication ( ( "-" | "+" ) multiplication )* ;
multiplication → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → NUMBER | STRING | "false" | "true" | "nil"
               | "(" expression ")" ;
*/

#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "./Parser.hpp"
#include "./Expr.hpp"
#include "./Token.hpp"

using std::initializer_list;
using std::shared_ptr;
using std::runtime_error;
using std::to_string;
using std::vector;

shared_ptr<Expr<Object>> Parser::expression() {
    return equality();
}

shared_ptr<Expr<Object>> Parser::equality() {
    shared_ptr<Expr<Object>> expr = comparison();
    while (match({ BANG_EQUAL, EQUAL_EQUAL })) {
      Token operation = previous();
      shared_ptr<Expr<Object>> right = comparison();
      expr = shared_ptr<Expr<Object>>(new Binary<Object>(expr, operation, right));
    }
    return expr;
}

shared_ptr<Expr<Object>> Parser::comparison() {
    shared_ptr<Expr<Object>> expr = addition();

    while (match({ GREATER, GREATER_EQUAL, LESS, LESS_EQUAL })) {
      Token operation = previous();
      shared_ptr<Expr<Object>> right = addition();
      expr = shared_ptr<Expr<Object>>(new Binary<Object>(expr, operation, right));
    }
    return expr;
}

shared_ptr<Expr<Object>> Parser::addition() {
    shared_ptr<Expr<Object>> expr = multiplication();

    while (match({ MINUS, PLUS })) {
      Token operation = previous();
      shared_ptr<Expr<Object>> right = multiplication();
      expr = shared_ptr<Expr<Object>>(new Binary<Object>(expr, operation, right));
    }
    return expr;
}
shared_ptr<Expr<Object>> Parser::multiplication() {
    shared_ptr<Expr<Object>> expr = unary();

    while (match({ SLASH, STAR })) {
      Token operation = previous();
      shared_ptr<Expr<Object>> right = unary();
      expr = shared_ptr<Expr<Object>>(new Binary<Object>(expr, operation, right));
    }

    return expr;
}

shared_ptr<Expr<Object>> Parser::unary() {
    if (match({ BANG, MINUS })) {
      Token operation = previous();
      shared_ptr<Expr<Object>> right = unary();
      return shared_ptr<Expr<Object>>(new Unary<Object>(operation, right));
    }
    return primary();
}

shared_ptr<Expr<Object>> Parser::primary() {
    if (match({ FALSE })) {
        return shared_ptr<Expr<Object>>(new Literal<Object>("false"));
    }
    if (match({ TRUE })) {
        return shared_ptr<Expr<Object>>(new Literal<Object>("true"));
    }
    if (match({ NIL })) {
        return shared_ptr<Expr<Object>>(new Literal<Object>(""));
    }

    if (match({ NUMBER, STRING })) {
      return shared_ptr<Expr<Object>>(new Literal<Object>(previous().literal.toString()));
    }

    if (match({ LEFT_PAREN })) {
      shared_ptr<Expr<Object>> expr = expression();
      consume(RIGHT_PAREN, "Expect ')' after expression.");
      return shared_ptr<Expr<Object>>(new Grouping<Object>(expr));
    }
    throw error(peek(), "Expect expression.");
}

bool Parser::match(const initializer_list<TokenType> &types) {
    for (auto type : types) {
      if (check(type)) {
        advance();
        return true;
      }
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) {
        return false;
    }
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) {
        current++;
    }
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == TOKEN_EOF;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

Token Parser::consume(TokenType type, string message) {
  if (check(type)) return advance();
  throw error(peek(), message);
}

runtime_error Parser::error(Token token, string message) {
  if (token.type == TOKEN_EOF) {
    return runtime_error(to_string(token.line) + " at end" + message);
  } else {
    return runtime_error(to_string(token.line) + " at '" + token.lexeme + "'" + message);
  }
}

void Parser::synchronize() {
  advance();
    while (!isAtEnd()) {
      if (previous().type == SEMICOLON) return;
      switch (peek().type) {
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
          return;
      }
      advance();
    }
}

shared_ptr<Expr<Object>> Parser::parse() {
  try {
    return expression();
  } catch (runtime_error error) {
    return nullptr;
  }
}
