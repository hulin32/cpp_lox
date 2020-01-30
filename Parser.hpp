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

using std::vector;
using std::shared_ptr;
using std::initializer_list;
using std::string;
using std::runtime_error;

class Parser {
 public:
    explicit Parser(vector<Token> tokens_): tokens(tokens_) { }
    shared_ptr<Expr> parse();
 private:
    vector<Token> tokens;
    int current = 0;
    shared_ptr<Expr> expression();
    shared_ptr<Expr> equality();
    shared_ptr<Expr> comparison();
    shared_ptr<Expr> addition();
    shared_ptr<Expr> multiplication();
    shared_ptr<Expr> unary();
    shared_ptr<Expr> primary();
    bool match(const initializer_list<TokenType> &types);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    Token consume(TokenType type, string message);
    runtime_error error(Token token, string message);
    void synchronize();
};

#endif  // PARSER_HPP_

