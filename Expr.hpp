// Copyright 2020 <Copyright hulin>

#ifndef EXPR_HPP_
#define EXPR_HPP_

#include <string>
#include <list>
#include <memory>
#include "./token.hpp"

using std::list;
using std::string;
using std::shared_ptr;

class Literal;
class Assign;
class Binary;

class Visitor {
 public:
    virtual ~Visitor() = default;
    virtual string visitLiteralExpr(const Literal& expr) = 0;
    virtual string visitAssignExpr(const Assign& expr) = 0;
    virtual string visitBinaryExpr(const Binary& expr) = 0;
    // virtual string visitCallExpr(const Call& expr) = 0;
    // virtual string visitGetExpr(const Get& expr) = 0;
    // virtual string visitGroupingExpr(const Grouping& expr) = 0;
    // virtual string visitLogicalExpr(const Logical& expr) = 0;
    // virtual string visitSetExpr(const Set& expr) = 0;
    // virtual string visitSuperExpr(const Super& expr) = 0;
    // virtual string visitThisExpr(const This& expr) = 0;
    // virtual string visitUnaryExpr(const Unary& expr) = 0;
    // virtual string visitVariableExpr(const Variable& expr) = 0;
};

class Expr {
 public:
    virtual string accept(shared_ptr<Visitor> visitor) = 0;
    virtual ~Expr() = default;
};

class Literal: public Expr {
 public:
    explicit Literal(string value_);
    string accept(shared_ptr<Visitor> visitor) override;
    string value;
};

class Assign: public Expr {
 public:
    Assign(Token name, shared_ptr<Expr> value);
    string accept(shared_ptr<Visitor> visitor) override;
    Token name;
    shared_ptr<Expr> value;
};

class Binary: public Expr {
 public:
    Binary(shared_ptr<Expr> left, Token operation, shared_ptr<Expr> right);
    string accept(shared_ptr<Visitor> visitor) override;
    shared_ptr<Expr> left;
    Token operation;
    shared_ptr<Expr> right;
};

// class Call: public Expr {
//  public:
//     Call(unique_ptr<Expr> callee, Token paren, list<unique_ptr<Expr>> arguments);
//     string accept(unique_ptr<Visitor> visitor) override;
//     unique_ptr<Expr> callee;
//     Token paren;
//     list<unique_ptr<Expr>> arguments;
// };

// class Get: public Expr {
//  public:
//     Get(unique_ptr<Expr> object, Token name);
//     string accept(unique_ptr<Visitor> visitor) override;
//     Token name;
//     unique_ptr<Expr> object;
// };
// class Grouping: public Expr {
//  public:
//     explicit Grouping(unique_ptr<Expr> expression);
//     string accept(unique_ptr<Visitor> visitor) override;
//     unique_ptr<Expr> expression;
// };


// class Logical: public Expr {
//  public:
//     Logical(unique_ptr<Expr> left, Token operation, unique_ptr<Expr> right);
//     string accept(unique_ptr<Visitor> visitor) override;
//     unique_ptr<Expr> left;
//     Token operation;
//     unique_ptr<Expr> right;
// };

// class Set: public Expr {
//  public:
//     Set(unique_ptr<Expr> object, Token name, unique_ptr<Expr> value);
//     string accept(unique_ptr<Visitor> visitor) override;
//     unique_ptr<Expr> object;
//     Token name;
//     unique_ptr<Expr> value;
// };

// class Super: public Expr {
//  public:
//     Super(Token keyword, Token method);
//     string accept(unique_ptr<Visitor> visitor) override;
//     Token keyword;
//     Token method;
// };

// class This : public Expr{
//  public:
//     explicit This(Token keyword);
//     string accept(unique_ptr<Visitor> visitor) override;
//     Token keyword;
// };

// class Unary: public Expr {
//  public:
//     Unary(Token operation, unique_ptr<Expr> right);
//     string accept(unique_ptr<Visitor> visitor) override;
//     Token operation;
//     unique_ptr<Expr> right;
// };

// class Variable: public Expr {
//  public:
//     explicit Variable(Token name);
//     string accept(unique_ptr<Visitor> visitor) override;
//     Token name;
// };

#endif  // EXPR_HPP_
