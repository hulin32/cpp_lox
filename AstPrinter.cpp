// Copyright 2020 <Copyright hulin>

#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <memory>
#include "./Expr.hpp"
#include "./AstPrinter.hpp"
#include "./token.hpp"

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

string AstPrinter::visitAssignExpr(const Assign& expr) {
    return "(" +
        expr.name.lexeme +
        " " +
        expr.value->accept(shared_from_this()) +
        " )";
}

string AstPrinter::visitBinaryExpr(const Binary& expr) {
    return "(" +
        expr.operation.lexeme +
        " " +
        expr.left->accept(shared_from_this()) +
        " " +
        expr.right->accept(shared_from_this()) +
        ")";
}

string AstPrinter::visitGroupingExpr(const Grouping& expr) {
    return "(group " +
        expr.expression->accept(shared_from_this()) +
        ")";
}

string AstPrinter::visitUnaryExpr(const Unary& expr) {
    return "("+
        expr.operation.lexeme +
        " " +
        expr.right->accept(shared_from_this()) +
        ")";
}

// string AstPrinter::visitCallExpr(const Call& expr) {
//     return "";
// }

// string AstPrinter::visitGetExpr(const Get& expr) {
//     return "";
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

// string AstPrinter::visitVariableExpr(const Variable& expr) {
//     return "";
// }