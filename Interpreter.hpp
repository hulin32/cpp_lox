// Copyright 2020 <Copyright hulin>

#ifndef INTERPRETER_HPP_
#define INTERPRETER_HPP_

#include <memory>
#include <utility>
#include <string>
#include <vector>

#include "./Expr.hpp"
#include "./Stmt.hpp"
#include "./Token.hpp"

using std::shared_ptr;
using std::string;
using std::vector;

class Interpreter:
    public Visitor<Object>,
    public Visitor_Stmt,
    public std::enable_shared_from_this<Interpreter>
{
 public:
    void interpret(vector<shared_ptr<Stmt>> statements);
    Object visitLiteralExpr(const Literal<Object>& expr);
    Object visitAssignExpr(const Assign<Object>& expr);
    Object visitBinaryExpr(const Binary<Object>& expr);
    Object visitGroupingExpr(const Grouping<Object>& expr);
    Object visitUnaryExpr(const Unary<Object>& expr);
    void visitExpressionStmt(const Expression& stmt);
    void visitPrintStmt(const Print& stmt);
 private:
    Object evaluate(shared_ptr<Expr<Object>> expr);
    void execute(shared_ptr<Stmt> stmt);
    bool isTruthy(Object object);
    bool isEqual(Object a, Object b);
    void checkNumberOperand(Token operation, Object operand);
    void checkNumberOperands(Token operation, Object left, Object right);
    string stringify(Object object);
};

#endif  // INTERPRETER_HPP_
