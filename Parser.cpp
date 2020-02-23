// Copyright 2020-1-29 <Copyright hulin>

/*
expression     → equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → addition ( ( ">" | ">=" | "<" | "<=" ) addition )* ;
addition       → multiplication ( ( "-" | "+" ) multiplication )* ;
multiplication → unary ( ( "/" | "*" ) unary )* ;
unary → ( "!" | "-" ) unary | call ;
call → primary ( "(" arguments? ")" | "." IDENTIFIER )* ;
arguments → expression ( "," expression )* ;
primary        → "false" | "true" | "nil"
               | NUMBER | STRING
               | "(" expression ")"
               | IDENTIFIER ;
expression → assignment ;
assignment → ( call "." )? IDENTIFIER "=" assignment
           | logic_or;
logic_or   → logic_and ( "or" logic_and )* ;
logic_and  → equality ( "and" equality )* ;
*/

#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

#include "./Parser.hpp"
#include "./Expr.hpp"
#include "./Stmt.hpp"
#include "./Token.hpp"

using std::initializer_list;
using std::shared_ptr;
using std::runtime_error;
using std::to_string;
using std::vector;

shared_ptr<Expr<Object>> Parser::expression() {
    return assignment();
}

shared_ptr<Expr<Object>> Parser::assignment() {
  shared_ptr<Expr<Object>> expr = orSmt();
  if (match({ EQUAL })) {
    Token equals = previous();
    shared_ptr<Expr<Object>> value = assignment();

    auto variable = dynamic_cast<Variable<Object>*>(expr.get());
    auto get = dynamic_cast<Get<Object>*>(expr.get());
    if (variable != nullptr) {
      Token name = variable->name;
      return shared_ptr<Expr<Object>>(
        new Assign<Object>(name, value));
    }
    if (get != nullptr) {
      return shared_ptr<Expr<Object>>(
        new Set<Object>(get->object, get->name, value));
    }
    error(equals, "Invalid assignment target.");
  }
  return expr;
}

shared_ptr<Expr<Object>> Parser::orSmt() {
    shared_ptr<Expr<Object>> expr = andSmt();
    while (match({ OR })) {
      Token operation = previous();
      shared_ptr<Expr<Object>> right = andSmt();
      expr =  shared_ptr<Expr<Object>>(
        new Logical<Object>(expr, operation, right));
    }
    return expr;
}

shared_ptr<Expr<Object>> Parser::andSmt() {
    shared_ptr<Expr<Object>> expr = equality();
    while (match({ AND })) {
      Token operation = previous();
      shared_ptr<Expr<Object>> right = equality();
      expr = shared_ptr<Expr<Object>>(new Logical<Object>(expr, operation, right));
    }
    return expr;
}

shared_ptr<Stmt> Parser::statement() {
  if (match({ FOR })) {
    return forStatement();
  }
  if (match({ IF })) {
    return ifStatement();
  }
  if (match({ PRINT })) {
    return printStatement();
  }
  if (match({ RETURN })) {
    return returnStatement();
  }
  if (match({ WHILE })) {
    return whileStatement();
  }
  if (match({ LEFT_BRACE })) {
    return shared_ptr<Stmt>(new Block(block()));
  }
  return expressionStatement();
}

shared_ptr<Stmt> Parser::forStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'for'.");
    shared_ptr<Stmt> initializer;
    if (match({ SEMICOLON })) {
      initializer = nullptr;
    } else if (match({ VAR })) {
      initializer = varDeclaration();
    } else {
      initializer = expressionStatement();
    }

    shared_ptr<Expr<Object>> condition = nullptr;
    if (!check(SEMICOLON)) {
      condition = expression();
    }
    consume(SEMICOLON, "Expect ';' after loop condition.");

    shared_ptr<Expr<Object>> increment = nullptr;
    if (!check(RIGHT_PAREN)) {
      increment = expression();
    }
    consume(RIGHT_PAREN, "Expect ')' after for clauses.");

    shared_ptr<Stmt> body = statement();

    if (increment != nullptr) {
      vector<shared_ptr<Stmt>> stmts;
      stmts.push_back(body);
      stmts.push_back(shared_ptr<Stmt>(new Expression(increment)));
      body = shared_ptr<Stmt>(new Block(stmts));
    }

    if (condition == nullptr) {
      condition = shared_ptr<Expr<Object>>(
        new Literal<Object>(Object::make_bool_obj(true)));
    }
    body = shared_ptr<Stmt>(new While(condition, body));

    if (initializer != nullptr) {
      vector<shared_ptr<Stmt>> stmts2;
      stmts2.push_back(initializer);
      stmts2.push_back(body);
      body = shared_ptr<Stmt>(new Block(stmts2));
    }

    return body;
}

shared_ptr<Stmt> Parser::ifStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    shared_ptr<Expr<Object>> condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition.");
    shared_ptr<Stmt> thenBranch = statement();
    shared_ptr<Stmt> elseBranch = nullptr;
    if (match({ ELSE })) {
      elseBranch = statement();
    }
    return shared_ptr<Stmt>(new If(condition, thenBranch, elseBranch));
}

shared_ptr<Stmt> Parser::printStatement() {
  shared_ptr<Expr<Object>> value = expression();
  consume(SEMICOLON, "Expect ';' after value.");
  shared_ptr<Stmt> print(new Print(value));
  return print;
}

shared_ptr<Stmt> Parser::returnStatement() {
  Token keyword = previous();
  shared_ptr<Expr<Object>> value;
    if (!check(SEMICOLON)) {
      value = expression();
    }
    consume(SEMICOLON, "Expect ';' after return value.");
    return shared_ptr<Stmt>(new Return(keyword, value));
}

shared_ptr<Stmt> Parser::whileStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    shared_ptr<Expr<Object>> condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after condition.");
    shared_ptr<Stmt> body = statement();
    return shared_ptr<Stmt>(new While(condition, body));
}

shared_ptr<Stmt> Parser::expressionStatement() {
  shared_ptr<Expr<Object>> expr = expression();
  consume(SEMICOLON, "Expect ';' after expression.");
  shared_ptr<Stmt> expression(new Expression(expr));
  return expression;
}

shared_ptr<Function> Parser::function(string kind) {
    Token name = consume(IDENTIFIER, "Expect " + kind + " name.");
    consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
    vector<Token> parameters;
    if (!check(RIGHT_PAREN)) {
      do {
        if (parameters.size() >= 255) {
          error(peek(), "Cannot have more than 255 parameters.");
        }
        parameters.push_back(consume(IDENTIFIER, "Expect parameter name."));
      } while (match({ COMMA }));
    }
    consume(RIGHT_PAREN, "Expect ')' after parameters.");
    consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
    vector<shared_ptr<Stmt>> body = block();
    return shared_ptr<Function>(new Function(name, parameters, body));
  }

vector<shared_ptr<Stmt>> Parser::block() {
    vector<shared_ptr<Stmt>> statements;

    while (!check(RIGHT_BRACE) && !isAtEnd()) {
      statements.push_back(declaration());
    }
    consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
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
      expr = shared_ptr<Expr<Object>>(
        new Binary<Object>(expr, operation, right));
    }
    return expr;
}
shared_ptr<Expr<Object>> Parser::multiplication() {
    shared_ptr<Expr<Object>> expr = unary();

    while (match({ SLASH, STAR })) {
      Token operation = previous();
      shared_ptr<Expr<Object>> right = unary();
      expr = shared_ptr<Expr<Object>>(
        new Binary<Object>(expr, operation, right));
    }

    return expr;
}

shared_ptr<Expr<Object>> Parser::unary() {
    if (match({ BANG, MINUS })) {
      Token operation = previous();
      shared_ptr<Expr<Object>> right = unary();
      return shared_ptr<Expr<Object>>(new Unary<Object>(operation, right));
    }
    return call();
}

shared_ptr<Expr<Object>> Parser::finishCall(shared_ptr<Expr<Object>> callee) {
    vector<shared_ptr<Expr<Object>>> arguments;
    if (!check(RIGHT_PAREN)) {
      do {
        if (arguments.size() >= 255) {
          error(peek(), "Cannot have more than 255 arguments.");
        }
        arguments.push_back(expression());
      } while (match({ COMMA }));
    }

    Token paren = consume(RIGHT_PAREN, "Expect ')' after arguments.");

    return shared_ptr<Expr<Object>>(
      new Call<Object>(callee, paren, arguments));
}

shared_ptr<Expr<Object>> Parser::call() {
    shared_ptr<Expr<Object>> expr = primary();
    while (true) {
      if (match({ LEFT_PAREN })) {
        expr = finishCall(expr);
      } else if (match({ DOT })) {
        Token name = consume(IDENTIFIER, "Expect property name after '.'.");
        expr = shared_ptr<Expr<Object>>(new Get<Object>(expr, name));
      } else {
        break;
      }
    }
    return expr;
}

shared_ptr<Expr<Object>> Parser::primary() {
    if (match({ FALSE })) {
        return shared_ptr<Expr<Object>>(
          new Literal<Object>(Object::make_bool_obj(false)));
    }
    if (match({ TRUE })) {
        return shared_ptr<Expr<Object>>(
          new Literal<Object>(Object::make_bool_obj(true)));
    }
    if (match({ NIL })) {
        return shared_ptr<Expr<Object>>(
          new Literal<Object>(Object::make_nil_obj()));
    }

    if (match({ NUMBER })) {
      return shared_ptr<Expr<Object>>(
        new Literal<Object>(Object::make_num_obj(previous().literal.num)));
    }
    if (match({ STRING })) {
      return shared_ptr<Expr<Object>>(
        new Literal<Object>(Object::make_str_obj(previous().literal.str)));
    }

    if (match({ IDENTIFIER })) {
      return shared_ptr<Expr<Object>>(
        new Variable<Object>(previous()));
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

shared_ptr<Stmt> Parser::varDeclaration() {
  Token name = consume(IDENTIFIER, "Expect variable name.");
  shared_ptr<Expr<Object>> initializer;
  if (match({ EQUAL })) {
    initializer = expression();
  }

  consume(SEMICOLON, "Expect ';' after variable declaration.");
  shared_ptr<Stmt> var(new Var(name, initializer));
  return var;
}


shared_ptr<Stmt> Parser::declaration() {
  try {
    if (match({ CLASS })) return classDeclaration();
    if (match({ FUN })) return function("function");
    if (match({ VAR })) return varDeclaration();
    return statement();
  } catch (runtime_error error) {
    synchronize();
    return nullptr;
  }
}

shared_ptr<Stmt> Parser::classDeclaration() {
    Token name = consume(IDENTIFIER, "Expect class name.");
    consume(LEFT_BRACE, "Expect '{' before class body.");

    vector<shared_ptr<Function>> methods;
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
      methods.push_back(function("method"));
    }
    consume(RIGHT_BRACE, "Expect '}' after class body.");

    return shared_ptr<Stmt>(new Class(name, methods));
}

vector<shared_ptr<Stmt>> Parser::parse() {
  vector<shared_ptr<Stmt>> statements;
  while (!isAtEnd()) {
    statements.push_back(declaration());
  }
  return statements;
}
