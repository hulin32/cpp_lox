// Copyright 2020 <Copyright hulin>

#ifndef ASTPRINTER_HPP_
#define ASTPRINTER_HPP_

#include <utility>
#include <string>
#include <vector>
#include <memory>
#include "./Expr.hpp"

using std::string;
using std::shared_ptr;

class AstPrinter: public Visitor, public std::enable_shared_from_this<AstPrinter> {
 public:
    string print(shared_ptr<Expr> expr);
    string visitLiteralExpr(const Literal& expr);
    string visitAssignExpr(const Assign& expr);
    string visitBinaryExpr(const Binary& expr);
   //  string visitCallExpr(const Call& expr);
   //  string visitGetExpr(const Get& expr);
   //  string visitGroupingExpr(const Grouping& expr);
   //  string visitLogicalExpr(const Logical& expr);
   //  string visitSetExpr(const Set& expr);
   //  string visitSuperExpr(const Super& expr);
   //  string visitThisExpr(const This& expr);
   //  string visitUnaryExpr(const Unary& expr);
   //  string visitVariableExpr(const Variable& expr);
};

#endif  // ASTPRINTER_HPP_
