// Copyright 2020 <Copyright hulin>

#ifndef INTERPRETER_HPP_
#define INTERPRETER_HPP_

#include <memory>
#include <utility>

#include "./Expr.hpp"
#include "./Token.hpp"

using std::shared_ptr;

class Interpreter:
    public Visitor<Object>,
    public std::enable_shared_from_this<Interpreter>
{
 public:
    Object visitLiteralExpr(const Literal<Object>& expr);
    Object visitAssignExpr(const Assign<Object>& expr);
    Object visitBinaryExpr(const Binary<Object>& expr);
    Object visitGroupingExpr(const Grouping<Object>& expr);
    Object visitUnaryExpr(const Unary<Object>& expr);
    Object evaluate(shared_ptr<Expr<Object>> expr);
};

#endif  // INTERPRETER_HPP_
