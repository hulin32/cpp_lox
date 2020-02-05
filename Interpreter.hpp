// Copyright 2020 <Copyright hulin>

#ifndef INTERPRETER_HPP_
#define INTERPRETER_HPP_

#include <memory>
#include <utility>
#include <string>

#include "./Expr.hpp"
#include "./Token.hpp"

using std::shared_ptr;
using std::string;

class Interpreter:
    public Visitor<Object>,
    public std::enable_shared_from_this<Interpreter>
{
 public:
    Interpreter() {}
    void interpret(shared_ptr<Expr<Object>> expression);
 private:
    Object visitLiteralExpr(const Literal<Object>& expr);
    Object visitAssignExpr(const Assign<Object>& expr);
    Object visitBinaryExpr(const Binary<Object>& expr);
    Object visitGroupingExpr(const Grouping<Object>& expr);
    Object visitUnaryExpr(const Unary<Object>& expr);
    Object evaluate(shared_ptr<Expr<Object>> expr);
    bool isTruthy(Object object);
    bool isEqual(Object a, Object b);
    void checkNumberOperand(Token operation, Object operand);
    void checkNumberOperands(Token operation, Object left, Object right);
    string stringify(Object object);
};

#endif  // INTERPRETER_HPP_
