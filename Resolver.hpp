// Copyright 2020.2.22 <Copyright hulin>
#ifndef RESOLVER_HPP_
#define RESOLVER_HPP_

#include <memory>
#include <vector>
#include <string>
#include <map>

#include "./Stmt.hpp"
#include "./Expr.hpp"
#include "./Interpreter.hpp"
#include "./Token.hpp"

using std::shared_ptr;
using std::vector;
using std::string;
using std::map;

class Resolver:
    public Visitor<Object>,
    public Visitor_Stmt,
    public std::enable_shared_from_this<Resolver> {
 public:
    vector<map<string, bool>> scopes;
    shared_ptr<Interpreter> interpreter;
    explicit Resolver(shared_ptr<Interpreter> interpreter);
    Object visitLiteralExpr(shared_ptr<Literal<Object>> expr);
    Object visitAssignExpr(shared_ptr<Assign<Object>> expr);
    Object visitBinaryExpr(shared_ptr<Binary<Object>> expr);
    Object visitGroupingExpr(shared_ptr<Grouping<Object>> expr);
    Object visitUnaryExpr(shared_ptr<Unary<Object>> expr);
    Object visitVariableExpr(shared_ptr<Variable<Object>> expr);
    Object visitLogicalExpr(shared_ptr<Logical<Object>> expr);
    Object visitCallExpr(shared_ptr<Call<Object>> expr);
    Object visitGetExpr(shared_ptr<Get<Object>> expr);
    Object visitSetExpr(shared_ptr<Set<Object>> expr);
    void visitExpressionStmt(const Expression& stmt);
    void visitPrintStmt(const Print& stmt);
    void visitVarStmt(const Var& stmt);
    void visitBlockStmt(const Block& stmt);
    void visitClassStmt(const Class& stmt);
    void visitIfStmt(const If& stmt);
    void visitWhileStmt(const While& stmt);
    void visitFunctionStmt(shared_ptr<Function> stmt);
    void visitReturnStmt(const Return& stmt);
    void resolve(vector<shared_ptr<Stmt>> statements);
    void resolve(shared_ptr<Stmt> stmt);
    void resolve(shared_ptr<Expr<Object>> expr);
 private:
    enum FunctionType {
        NONE,
        FUNCTION,
        METHOD
    };
    FunctionType currentFunction = NONE;
    void beginScope();
    void endScope();
    void declare(Token name);
    void define(Token name);
    void resolveLocal(shared_ptr<Expr<Object>>, Token name);
    void resolveFunction(shared_ptr<Function> function, FunctionType type);
};

#endif  // RESOLVER_HPP_
