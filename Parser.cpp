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

shared_ptr<Expr> Parser::expression() {
    return equality();
}

shared_ptr<Expr> Parser::equality() {
    shared_ptr<Expr> expr = comparison();
    while (match({ BANG_EQUAL, EQUAL_EQUAL })) {
      Token operation = previous();
      shared_ptr<Expr> right = comparison();
      expr = shared_ptr<Expr>(new Binary(expr, operation, right));
    }
    return expr;
}

shared_ptr<Expr> Parser::comparison() {
    shared_ptr<Expr> expr = addition();

    while (match({ GREATER, GREATER_EQUAL, LESS, LESS_EQUAL })) {
      Token operation = previous();
      shared_ptr<Expr> right = addition();
      expr = shared_ptr<Expr>(new Binary(expr, operation, right));
    }
    return expr;
}

shared_ptr<Expr> Parser::addition() {
    shared_ptr<Expr> expr = multiplication();

    while (match({ MINUS, PLUS })) {
      Token operation = previous();
      shared_ptr<Expr> right = multiplication();
      expr = shared_ptr<Expr>(new Binary(expr, operation, right));
    }
    return expr;
}
shared_ptr<Expr> Parser::multiplication() {
    shared_ptr<Expr> expr = unary();

    while (match({ SLASH, STAR })) {
      Token operation = previous();
      shared_ptr<Expr> right = unary();
      expr = shared_ptr<Expr>(new Binary(expr, operation, right));
    }

    return expr;
}

shared_ptr<Expr> Parser::unary() {
    if (match({ BANG, MINUS })) {
      Token operation = previous();
      shared_ptr<Expr> right = unary();
      return shared_ptr<Expr>(new Unary(operation, right));
    }
    return primary();
}

shared_ptr<Expr> Parser::primary() {
    if (match({ FALSE })) {
        return shared_ptr<Expr>(new Literal("false"));
    }
    if (match({ TRUE })) {
        return shared_ptr<Expr>(new Literal("true"));
    }
    if (match({ NIL })) {
        return shared_ptr<Expr>(new Literal(""));
    }

    if (match({ NUMBER, STRING })) {
      return shared_ptr<Expr>(new Literal(previous().literal));
    }

    if (match({ LEFT_PAREN })) {
      shared_ptr<Expr> expr = expression();
      consume(RIGHT_PAREN, "Expect ')' after expression.");
      return shared_ptr<Expr>(new Grouping(expr));
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

shared_ptr<Expr> Parser::parse() {
  try {
    return expression();
  } catch (runtime_error error) {
    return nullptr;
  }
}
