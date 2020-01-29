// Copyright 2020 <Copyright hulin>

#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <memory>
#include "./Expr.hpp"
#include "./AstPrinter.hpp"

using std::string;
using std::shared_ptr;
using std::cout;
using std::endl;

string AstPrinter::print(shared_ptr<Expr> expr) {
    return expr->accept(shared_from_this());
}

string AstPrinter::visitLiteralExpr(const Literal& expr) {
    if (expr.value == "") return "nil";
    return expr.value;
}

// string AstPrinter::visitAssignExpr(const Assign& expr) {
//     return "";
// }
// string AstPrinter::visitBinaryExpr(const Binary& expr) {
//     return parenthesize(expr.operation.lexeme, expr.left, expr.right);
// }

// string AstPrinter::visitCallExpr(const Call& expr) {
//     return "";
// }

// string AstPrinter::visitGetExpr(const Get& expr) {
//     return "";
// }

// string AstPrinter::visitGroupingExpr(const Grouping& expr) {
//     return parenthesize("group", expr.expression);
// }

// string AstPrinter::visitLogicalExpr(const Logical& expr) {
//     return "";
// }

// string AstPrinter::visitSetExpr(const Set& expr) {
//     return "";
// }

// string AstPrinter::visitSuperExpr(const Super& expr) {
//     return "";
// }

// string AstPrinter::visitThisExpr(const This& expr) {
//     return "";
// }

// string AstPrinter::visitUnaryExpr(const Unary& expr) {
//     return parenthesize(expr.operation.lexeme, expr.right);
// }

// string AstPrinter::visitVariableExpr(const Variable& expr) {
//     return "";
// }

string AstPrinter::parenthesize_recursive() {
    return "";
}

template <typename R,  typename... OtherArgs>
string AstPrinter::parenthesize_recursive(R expr, OtherArgs&... exprs) {
    string str;
    str = expr.accept(shared_from_this());
    return str + " " + parenthesize_recursive(exprs...);
}

template <typename... OtherArgs>
string AstPrinter::parenthesize(string name, OtherArgs&... exprs) {
    string m_ast;
    m_ast += "(" + name;
    m_ast += parenthesize_recursive(exprs...);
    m_ast += ")";
    return m_ast;
}
