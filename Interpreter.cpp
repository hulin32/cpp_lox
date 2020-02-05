// Copyright 2020 <Copyright hulin>

#include "./Interpreter.hpp"
#include "./Expr.hpp"
#include "./Token.hpp"

Object Interpreter::visitLiteralExpr(const Literal<Object>& expr) {
    if (expr.value == "") return Object::make_nil_obj();
    return Object::make_str_obj(expr.value);
}

Object Interpreter::visitGroupingExpr(const Grouping<Object>& expr) {
    return evaluate(expr.expression);
}

Object Interpreter::evaluate(shared_ptr<Expr<Object>> expr) {
    return expr->accept(shared_from_this());
}
