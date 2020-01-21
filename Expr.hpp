// Copyright 2020 <Copyright hulin>

#ifndef EXPR_HPP_
#define EXPR_HPP_

#include <string>
#include <list>
#include "./token.hpp"

using std::list;

class Assign;
class Binary;
class Call;
class Get;
class Grouping;
class Literal;
class Logical;
class Set;
class Super;
class This;
class Unary;
class Variable;

class Visitor {
 public:
    virtual string visitAssignExpr(const Assign& expr) = 0;
    virtual string visitBinaryExpr(const Binary& expr) = 0;
    virtual string visitCallExpr(const Call& expr) = 0;
    virtual string visitGetExpr(const Get& expr) = 0;
    virtual string visitGroupingExpr(const Grouping& expr) = 0;
    virtual string visitLiteralExpr(const Literal& expr) = 0;
    virtual string visitLogicalExpr(const Logical& expr) = 0;
    virtual string visitSetExpr(const Set& expr) = 0;
    virtual string visitSuperExpr(const Super& expr) = 0;
    virtual string visitThisExpr(const This& expr) = 0;
    virtual string visitUnaryExpr(const Unary& expr) = 0;
    virtual string visitVariableExpr(const Variable& expr) = 0;
};

class Expr {
 public:
    virtual string accept(Visitor* visitor) = 0;
};

class Assign: public Expr {
 public:
    Assign(Token name, Expr value);
    Token name;
    Expr value;
};

class Binary: public Expr {
 public:
    Binary(Expr left, Token operation, Expr right);
    Expr left;
    Token operation;
    Expr right;
};

class Call: public Expr {
 public:
    Call(Expr callee, Token paren, list<Expr> arguments);
    Expr callee;
    Token paren;
    list<Expr> arguments;
};

class Get: public Expr {
 public:
    Get(Expr object, Token name);
    Token name;
    Expr object;
};
class Grouping: public Expr {
 public:
    explicit Grouping(Expr expression);
    Expr expression;
};

class Literal: public Expr {
 public:
    explicit Literal(string value);
    string value;
};

class Logical: public Expr {
 public:
    Logical(Expr left, Token operation, Expr right);
    Expr left;
    Token operation;
    Expr right;
};

class Set: public Expr {
 public:
    Set(Expr object, Token name, Expr value);
    Expr object;
    Token name;
    Expr value;
};

class Super: public Expr {
 public:
    Super(Token keyword, Token method);
    Token keyword;
    Token method;
};

class This : public Expr{
 public:
    explicit This(Token keyword);
    Token keyword;
};

class Unary: public Expr {
 public:
    Unary(Token operation, Expr right);
    Token operation;
    Expr right;
};

class Variable: public Expr {
 public:
    explicit Variable(Token name);
    Token name;
};

#endif  // EXPR_HPP_
