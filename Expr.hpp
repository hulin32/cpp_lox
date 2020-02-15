// Copyright 2020 <Copyright hulin>

#ifndef EXPR_HPP_
#define EXPR_HPP_

#include <string>
#include <iostream>
#include <list>
#include <memory>
#include "./Token.hpp"

using std::list;
using std::string;
using std::shared_ptr;
using std::cout;
using std::endl;

template<class R>
class Literal;

template<class R>
class Assign;

template<class R>
class Binary;

template<class R>
class Grouping;

template<class R>
class Unary;

template <class R>
class Visitor {
 public:
    virtual ~Visitor() = default;
    virtual R visitLiteralExpr(const Literal<R>& expr) = 0;
    virtual R visitAssignExpr(const Assign<R>& expr) = 0;
    virtual R visitBinaryExpr(const Binary<R>& expr) = 0;
    virtual R visitGroupingExpr(const Grouping<R>& expr) = 0;
    virtual R visitUnaryExpr(const Unary<R>& expr) = 0;
    // virtual string visitCallExpr(const Call& expr) = 0;
    // virtual string visitSetExpr(const Set& expr) = 0;
    // virtual string visitSuperExpr(const Super& expr) = 0;
    // virtual string visitThisExpr(const This& expr) = 0;
    // virtual string visitVariableExpr(const Variable& expr) = 0;
};

template<class R>
class Expr {
 public:
    virtual R accept(shared_ptr<Visitor<R>> visitor) = 0;
    virtual ~Expr() = default;  // for derived class
};

template<class R>
class Literal: public Expr<R> {
 public:
    explicit Literal(R value_): value(value_) { }
    R accept(shared_ptr<Visitor<R>> visitor) override {
        return visitor->visitLiteralExpr(*this);
    }
    R value;
};

template <class R>
class Assign: public Expr<R> {
 public:
    Assign(Token name_, shared_ptr<Expr<R>> value_):
    name(name_), value(value_) {}

    R accept(shared_ptr<Visitor<R>> visitor) override {
        return visitor->visitAssignExpr(*this);
    }
    Token name;
    shared_ptr<Expr<R>> value;
};

template <class R>
class Binary: public Expr<R> {
 public:
    Binary(
        shared_ptr<Expr<R>> left_,
        Token operation,
        shared_ptr<Expr<R>> right_):
    left(left_), operation(operation), right(right_) { }

    R accept(shared_ptr<Visitor<R>> visitor) override {
      return visitor->visitBinaryExpr(*this);
    }
    shared_ptr<Expr<R>> left;
    Token operation;
    shared_ptr<Expr<R>> right;
};

template <class R>
class Grouping: public Expr<R> {
 public:
    explicit Grouping(shared_ptr<Expr<R>> expression_):
    expression(expression_) { }

    R accept(shared_ptr<Visitor<R>> visitor) override {
        return visitor->visitGroupingExpr(*this);
    }
    shared_ptr<Expr<R>> expression;
};

template <class R>
class Unary: public Expr<R> {
 public:
    Unary(Token operation, shared_ptr<Expr<R>> right_):
    operation(operation), right(right_) { }

    R accept(shared_ptr<Visitor<R>> visitor) override {
      return visitor->visitUnaryExpr(*this);
    }
    Token operation;
    shared_ptr<Expr<R>> right;
};

// template <class R>
// class Call: public Expr<R> {
//  public:
//     Call(
//         shared_ptr<Expr<R>> callee_,
//         Token paren_,
//         list<shared_ptr<Expr<R>>> arguments_
//     ): callee(callee_), paren(paren_), arguments(arguments_){ }
//     R accept(shared_ptr<Visitor<R>> visitor) override {
//       return visitor->visitCallExpr(*this);
//     }
//     shared_ptr<Expr<R>> callee;
//     Token paren;
//     list<shared_ptr<Expr<R>>> arguments;
// };

// template <class R>
// class Get: public Expr<R> {
//  public:
//     Get(shared_ptr<Expr<R>> object, Token name);
//     R accept(shared_ptr<Visitor<R>> visitor) override {
//       return visitor->visitGetExpr(*this);
//     }
//     Token name;
//     shared_ptr<Expr<R>> object;
// };

// template <class R>
// class Logical: public Expr<R> {
//  public:
//     Logical(shared_ptr<Expr<R>> left, Token operation, shared_ptr<Expr<R>> right);
//     R accept(shared_ptr<Visitor<R>> visitor) override {
//         return visitor->visitLogicalExpr(*this);
//     };
//     shared_ptr<Expr<R>> left;
//     Token operation;
//     shared_ptr<Expr<R>> right;
// };

// template <class R>
// class Set: public Expr<R> {
//  public:
//     Set(shared_ptr<Expr<R>> object, Token name, shared_ptr<Expr<R>> value);
//     R accept(shared_ptr<Visitor<R>> visitor) override {
//         return visitor->visitSetExpr(*this);
//     };
//     shared_ptr<Expr<R>> object;
//     Token name;
//     shared_ptr<Expr<R>> value;
// };

// template <class R>
// class Super: public Expr<R> {
//  public:
//     Super(Token keyword, Token method);
//     R accept(shared_ptr<Visitor<R>> visitor) override {
//         return visitor->visitSuperExpr(*this);
//     };
//     Token keyword;
//     Token method;
// };

// template <class R>
// class This : public Expr<R>{
//  public:
//     explicit This(Token keyword);
//     R accept(shared_ptr<Visitor<R>> visitor) override {
//         return visitor->visitThisExpr(*this);
//     };
//     Token keyword;
// };

// template <class R>
// class Variable: public Expr<R> {
//  public:
//     explicit Variable(Token name);
//     R accept(shared_ptr<Visitor<R>> visitor) override {
//         return visitor->visitVariableExpr(*this);
//     };
//     Token name;
// };

#endif  // EXPR_HPP_
