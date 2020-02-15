// Copyright 2020 <Copyright hulin>

#ifndef STMT_HPP_
#define STMT_HPP_

#include <string>
#include <iostream>
#include <memory>
#include "./Token.hpp"
#include "./Expr.hpp"

using std::string;
using std::shared_ptr;
using std::cout;
using std::endl;

class Expression;
class Print;

class Visitor_Stmt {
 public:
    virtual ~Visitor_Stmt() = default;
    virtual void visitExpressionStmt(const Expression& stmt) = 0;
    virtual void visitPrintStmt(const Print& stmt) = 0;
};

class Stmt {
 public:
    virtual void accept(shared_ptr<Visitor_Stmt> visitor) = 0;
    virtual ~Stmt() = default;  // for derived class
};


class Expression: public Stmt {
 public:
    explicit Expression(shared_ptr<Expr<Object>> expression_):
        expression(expression_) { }
    void accept(shared_ptr<Visitor_Stmt> visitor) override {
        visitor->visitExpressionStmt(*this);
    }
    shared_ptr<Expr<Object>> expression;
};


class Print: public Stmt {
 public:
    explicit Print(shared_ptr<Expr<Object>> expression_):
        expression(expression_) { }
    void accept(shared_ptr<Visitor_Stmt> visitor) override {
        visitor->visitPrintStmt(*this);
    }
    shared_ptr<Expr<Object>> expression;
};

#endif  // STMT_HPP_
