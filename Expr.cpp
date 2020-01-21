// Copyright 2020 <Copyright hulin>

#include <string>
#include <iostream>
#include <list>

#include "./Expr.hpp"
#include "./token.hpp"

using std::string;
using std::list;
using std::cout;
using std::endl;

Assign::Assign(Token name, Expr value): name(name), value(value) {}
string Assign::accept(Visitor& visitor) {
  return visitor.visitAssignExpr(*this);
}

class Binary: public Expr {
 public:
    Binary(Expr left, Token operation, Expr right):
    left(left), operation(operation), right(right) {}

    string accept(Visitor& visitor) {
      return visitor.visitBinaryExpr(*this);
    }

    Expr left;
    Token operation;
    Expr right;
};

class Call: public Expr {
 public:
    Call(Expr callee, Token paren, list<Expr> arguments):
    callee(callee), paren(paren), arguments(arguments) {}

    string accept(Visitor& visitor) {
      return visitor.visitCallExpr(*this);
    }

    Expr callee;
    Token paren;
    list<Expr> arguments;
};

class Get: public Expr {
 public:
    Get(Expr object, Token name):
    name(name), object(object) {}

    string accept(Visitor& visitor) {
      return visitor.visitGetExpr(*this);
    }

    Token name;
    Expr object;
};

class Grouping: public Expr {
 public:
    explicit Grouping(Expr expression): expression(expression) {}

    string accept(Visitor& visitor) {
      return visitor.visitGroupingExpr(*this);
    }

    Expr expression;
};

class Literal: public Expr {
 public:
    explicit Literal(string value): value(value) {}

    string accept(Visitor& visitor) {
      return visitor.visitLiteralExpr(*this);
    }

    string value;
};

class Logical: public Expr {
 public:
    Logical(Expr left, Token operation, Expr right):
    left(left), operation(operation), right(right) {}

    string accept(Visitor& visitor) {
      return visitor.visitLogicalExpr(*this);
    }

    Expr left;
    Token operation;
    Expr right;
};

class Set: public Expr {
 public:
    Set(Expr object, Token name, Expr value):
    object(object), name(name), value(value) {}

    string accept(Visitor& visitor) {
      return visitor.visitSetExpr(*this);
    }

    Expr object;
    Token name;
    Expr value;
};

class Super: public Expr {
 public:
    Super(Token keyword, Token method):
    keyword(keyword), method(method) {}

    string accept(Visitor& visitor) {
      return visitor.visitSuperExpr(*this);
    }

    Token keyword;
    Token method;
};

class This : public Expr{
 public:
    explicit This(Token keyword): keyword(keyword) {}

    string accept(Visitor& visitor) {
      return visitor.visitThisExpr(*this);
    }

    Token keyword;
};

class Unary: public Expr {
 public:
    Unary(Token operation, Expr right):
    operation(operation), right(right) {}

    string accept(Visitor& visitor) {
      return visitor.visitUnaryExpr(*this);
    }

    Token operation;
    Expr right;
};

class Variable: public Expr {
 public:
    explicit Variable(Token name): name(name) {}

    string accept(Visitor& visitor) {
      return visitor.visitVariableExpr(*this);
    }

    Token name;
};
