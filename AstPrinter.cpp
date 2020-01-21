// Copyright 2020 <Copyright hulin>

#include <utility>
#include <string>
#include <vector>
#include "./Expr.hpp"
#include "./AstPrinter.hpp"

using std::string;

string AstPrinter::print(Expr* expr) {
    return expr->accept(this);
}

string AstPrinter::visitAssignExpr(const Assign& expr) {
    return "";
}
string AstPrinter::visitBinaryExpr(const Binary& expr) {
    return parenthesize(expr.operation.lexeme, expr.left, expr.right);
}

string AstPrinter::visitCallExpr(const Call& expr) {
    return "";
}

string AstPrinter::visitGetExpr(const Get& expr) {
    return "";
}

string AstPrinter::visitGroupingExpr(const Grouping& expr) {
    return parenthesize("group", expr.expression);
}

string AstPrinter::visitLiteralExpr(const Literal& expr) {
    if (expr.value == "") return "nil";
    return expr.value;
}

string AstPrinter::visitLogicalExpr(const Logical& expr) {
    return "";
}

string AstPrinter::visitSetExpr(const Set& expr) {
    return "";
}

string AstPrinter::visitSuperExpr(const Super& expr) {
    return "";
}

string AstPrinter::visitThisExpr(const This& expr) {
    return "";
}

string AstPrinter::visitUnaryExpr(const Unary& expr) {
    return parenthesize(expr.operation.lexeme, expr.right);
}

string AstPrinter::visitVariableExpr(const Variable& expr) {
    return "";
}

string AstPrinter::parenthesize_recursive() {
    return "";
}

template <typename R,  typename... OtherArgs>
string AstPrinter::parenthesize_recursive(R expr, OtherArgs&... exprs) {
    string str;
    str = expr.accept(this);
    return str + " " + parenthesize_recursive(forward<OtherArgs>(exprs)...);
}

template <typename... OtherArgs>
string AstPrinter::parenthesize(string name, OtherArgs&... exprs) {
    string m_ast;
    m_ast += "(" + name;
    m_ast += parenthesize_recursive(forward<OtherArgs>(exprs)...);
    m_ast += ")";
    return m_ast;
}
