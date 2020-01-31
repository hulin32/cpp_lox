// Copyright 2020 <Copyright hulin>

#include <string>
#include <iostream>
#include <list>
#include <memory>
#include <algorithm>

#include "./Expr.hpp"
#include "./Token.hpp"

using std::string;
using std::list;
using std::cout;
using std::endl;
using std::shared_ptr;

Literal::Literal(string value_): value(value_) { }
string Literal::accept(shared_ptr<Visitor> visitor) {
  return visitor->visitLiteralExpr(*this);
}

Assign::Assign(Token name_, shared_ptr<Expr> value_):
name(name_), value(value_) {}
string Assign::accept(shared_ptr<Visitor> visitor) {
  return visitor->visitAssignExpr(*this);
}


Binary::Binary(
  shared_ptr<Expr> left_,
  Token operation,
  shared_ptr<Expr> right_
): left(left_), operation(operation), right(right_) { }
string Binary::accept(shared_ptr<Visitor> visitor) {
  return visitor->visitBinaryExpr(*this);
}

Grouping::Grouping(shared_ptr<Expr> expression_): expression(expression_) { }
string Grouping::accept(shared_ptr<Visitor> visitor) {
  return visitor->visitGroupingExpr(*this);
}

Unary::Unary(Token operation, shared_ptr<Expr> right_):
 operation(operation), right(right_) { }
string Unary::accept(shared_ptr<Visitor> visitor) {
  return visitor->visitUnaryExpr(*this);
}

// Call::Call(unique_ptr<Expr> callee, Token paren, list<unique_ptr<Expr>> arguments_):
// paren(paren) {
//   callee = std::move(callee);
//   arguments = std::move(arguments_);
// }
// string Call::accept(unique_ptr<Visitor> visitor) {
//   return visitor->visitCallExpr(*this);
// }


// Get::Get(unique_ptr<Expr> object_, Token name): name(name) {
//   object = std::move(object_);
// }
// string Get::accept(unique_ptr<Visitor> visitor) {
//   return visitor->visitGetExpr(*this);
// }

// Logical::Logical(
//   unique_ptr<Expr> left_,
//   Token operation,
//   unique_ptr<Expr> right_
// ): operation(operation) {
//   left = std::move(left_);
//   right = std::move(right_);
// }
// string Logical::accept(unique_ptr<Visitor> visitor) {
//   return visitor->visitLogicalExpr(*this);
// }


// Set::Set(
//   unique_ptr<Expr> object_,
//   Token name,
//   unique_ptr<Expr> value_
// ): name(name) {
//   object = std::move(object_);
//   value = std::move(value_);
// }
// string Set::accept(unique_ptr<Visitor> visitor) {
//   return visitor->visitSetExpr(*this);
// }

// Super::Super(Token keyword, Token method): keyword(keyword), method(method) {}
// string Super::accept(unique_ptr<Visitor> visitor) {
//   return visitor->visitSuperExpr(*this);
// }

// This::This(Token keyword): keyword(keyword) {}
// string This::accept(unique_ptr<Visitor> visitor) {
//   return visitor->visitThisExpr(*this);
// }

// Variable::Variable(Token name): name(name) {}
// string Variable::accept(unique_ptr<Visitor> visitor) {
//   return visitor->visitVariableExpr(*this);
// }
