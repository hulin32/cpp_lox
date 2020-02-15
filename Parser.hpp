// Copyright 2020-1-29 <Copyright hulin>

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <initializer_list>
#include "./Token.hpp"
#include "./Expr.hpp"
#include "./Stmt.hpp"

using std::vector;
using std::shared_ptr;
using std::initializer_list;
using std::string;
using std::runtime_error;

class Parser {
 public:
    explicit Parser(vector<Token> tokens_): tokens(tokens_) { }
    vector<shared_ptr<Stmt>> parse();
 private:
    vector<Token> tokens;
    int current = 0;
    shared_ptr<Expr<Object>> expression();
    shared_ptr<Expr<Object>> equality();
    shared_ptr<Expr<Object>> comparison();
    shared_ptr<Expr<Object>> addition();
    shared_ptr<Expr<Object>> multiplication();
    shared_ptr<Expr<Object>> unary();
    shared_ptr<Expr<Object>> primary();
    bool match(const initializer_list<TokenType> &types);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    Token consume(TokenType type, string message);
    runtime_error error(Token token, string message);
    void synchronize();
    shared_ptr<Stmt> statement();
    shared_ptr<Stmt> printStatement();
    shared_ptr<Stmt> expressionStatement();
    shared_ptr<Stmt> declaration();
    shared_ptr<Stmt> varDeclaration();
};

#endif  // PARSER_HPP_

