// Copyright 2020 <Copyright hulin>

#ifndef ASTPRINTER_HPP_
#define ASTPRINTER_HPP_

#include <utility>
#include <string>
#include <vector>
#include <memory>
#include "./Expr.hpp"

using std::string;

class AstPrinter: public Visitor {
 public:
    string print(Expr& expr);
    string visitLiteralExpr(const Literal expr);
   //  string visitAssignExpr(const Assign& expr);
   //  string visitBinaryExpr(const Binary& expr);
   //  string visitCallExpr(const Call& expr);
   //  string visitGetExpr(const Get& expr);
   //  string visitGroupingExpr(const Grouping& expr);
   //  string visitLogicalExpr(const Logical& expr);
   //  string visitSetExpr(const Set& expr);
   //  string visitSuperExpr(const Super& expr);
   //  string visitThisExpr(const This& expr);
   //  string visitUnaryExpr(const Unary& expr);
   //  string visitVariableExpr(const Variable& expr);
 private:
    string parenthesize_recursive();

    template <typename R,  typename... OtherArgs>
    string parenthesize_recursive(R expr, OtherArgs&... exprs);

    template <typename... OtherArgs>
    string parenthesize(string name, OtherArgs&... exprs);
};

#endif  // ASTPRINTER_HPP_
