// Copyright 2020.2.22 <Copyright hulin>

#include <memory>
#include <vector>
#include <map>

#include "./Stmt.hpp"
#include "./Expr.hpp"
#include "./Interpreter.hpp"
#include "./Resolver.hpp"
#include "./Token.hpp"
#include "./lox.hpp"

using std::vector;
using std::map;
using std::shared_ptr;
using std::make_shared;

Resolver::Resolver(shared_ptr<Interpreter> interpreter_):
    interpreter(interpreter_) { }

Object Resolver::visitLiteralExpr(shared_ptr<Literal<Object>> expr) {
    return Object::make_nil_obj();
}

Object Resolver::visitAssignExpr(shared_ptr<Assign<Object>> expr) {
    resolve(expr->value);
    resolveLocal(expr, expr->name);
    return Object::make_nil_obj();
}

Object Resolver::visitBinaryExpr(shared_ptr<Binary<Object>> expr) {
    resolve(expr->left);
    resolve(expr->right);
    return Object::make_nil_obj();
}

Object Resolver::visitGroupingExpr(shared_ptr<Grouping<Object>> expr) {
    resolve(expr->expression);
    return Object::make_nil_obj();
}

Object Resolver::visitUnaryExpr(shared_ptr<Unary<Object>> expr) {
    resolve(expr->right);
    return Object::make_nil_obj();
}

Object Resolver::visitVariableExpr(shared_ptr<Variable<Object>> expr) {
    if (scopes.size() != 0) {
        auto last = scopes.back();
        auto searched = last.find(expr->name.lexeme);
        if (searched != last.end() && !searched->second) {
            lox::error(expr->name.line,
                "Cannot read local variable in its own initializer.");
        }
    }
    resolveLocal(expr, expr->name);
    return Object::make_nil_obj();
}

Object Resolver::visitLogicalExpr(shared_ptr<Logical<Object>> expr) {
    resolve(expr->left);
    resolve(expr->right);
    return Object::make_nil_obj();
}

Object Resolver::visitCallExpr(shared_ptr<Call<Object>> expr) {
    resolve(expr->callee);

    for (auto argument : expr->arguments) {
      resolve(argument);
    }
    return Object::make_nil_obj();
}
Object Resolver::visitGetExpr(shared_ptr<Get<Object>> expr) {
    resolve(expr->object);
    return Object::make_nil_obj();
}

Object Resolver::visitSetExpr(shared_ptr<Set<Object>> expr) {
    resolve(expr->value);
    resolve(expr->object);
    return Object::make_nil_obj();
}

Object Resolver::visitThisExpr(shared_ptr<This<Object>> expr) {
    if (currentClass == CLASS_NONE) {
        lox::error(expr->keyword.line,
          "Cannot use 'this' outside of a class.");
        return Object::make_nil_obj();
    }
    resolveLocal(expr, expr->keyword);
    return Object::make_nil_obj();
}

Object Resolver::visitSuperExpr(shared_ptr<Super<Object>> expr) {
    if (currentClass == CLASS_NONE) {
      lox::error(expr->keyword.line,
          "Cannot use 'super' outside of a class.");
    } else if (currentClass != SUBCLASS) {
      lox::error(expr->keyword.line,
          "Cannot use 'super' in a class with no superclass.");
    }
    resolveLocal(expr, expr->keyword);
    return Object::make_nil_obj();
}


void Resolver::visitExpressionStmt(const Expression& stmt) {
    resolve(stmt.expression);
}

void Resolver::visitPrintStmt(const Print& stmt) {
    resolve(stmt.expression);
}

void Resolver::visitVarStmt(const Var& stmt) {
    declare(stmt.name);
    if (stmt.initializer != nullptr) {
      resolve(stmt.initializer);
    }
    define(stmt.name);
}

void Resolver::visitBlockStmt(const Block& stmt) {
    beginScope();
    resolve(stmt.statements);
    endScope();
}

void Resolver::visitClassStmt(const Class& stmt) {
    ClassType enclosingClass = currentClass;
    currentClass = CLASS;

    declare(stmt.name);
    define(stmt.name);

    if (stmt.superclass != nullptr &&
        stmt.name.lexeme == stmt.superclass->name.lexeme) {
        lox::error(stmt.superclass->name.line,
          "A class cannot inherit from itself.");
    }

    if (stmt.superclass != nullptr) {
        currentClass = SUBCLASS;
        resolve(stmt.superclass);
    }

    if (stmt.superclass != nullptr) {
        beginScope();
        auto backed = scopes.back();
        backed["super"] = true;
        scopes.pop_back();
        scopes.emplace_back(backed);
    }

    beginScope();
    auto back = scopes.back();
    back["this"] = true;
    scopes.pop_back();
    scopes.emplace_back(back);

    for (auto method : stmt.methods) {
      FunctionType declaration = METHOD;
      if (method->name.lexeme == "init") {
        declaration = INITIALIZER;
      }
      resolveFunction(method, declaration);
    }

    endScope();

    if (stmt.superclass != nullptr) endScope();

    currentClass = enclosingClass;
}

void Resolver::visitIfStmt(const If& stmt) {
    resolve(stmt.condition);
    resolve(stmt.thenBranch);
    if (stmt.elseBranch != nullptr) {
        resolve(stmt.elseBranch);
    }
}

void Resolver::visitWhileStmt(const While& stmt) {
    resolve(stmt.condition);
    resolve(stmt.body);
}

void Resolver::visitFunctionStmt(shared_ptr<Function> stmt) {
    declare(stmt->name);
    define(stmt->name);
    resolveFunction(stmt, FUNCTION);
}

void Resolver::visitReturnStmt(const Return& stmt) {
    if (currentFunction == FUNCTION_NONE) {
      lox::error(stmt.name.line, "Cannot return from top-level code.");
    }

    if (stmt.value != nullptr) {
        if (currentFunction == INITIALIZER) {
            lox::error(stmt.name.line,
                "Cannot return a value from an initializer.");
        }
        resolve(stmt.value);
    }
}


void Resolver::beginScope() {
    map<string, bool> m;
    scopes.push_back(m);
}

void Resolver::endScope() {
    scopes.pop_back();
}

void Resolver::resolve(vector<shared_ptr<Stmt>> statements) {
    for (auto statement : statements) {
        resolve(statement);
    }
}

void Resolver::resolve(shared_ptr<Stmt> stmt) {
    stmt->accept(shared_from_this());
}

void Resolver::resolve(shared_ptr<Expr<Object>> expr) {
    expr->accept(shared_from_this());
}

void Resolver::declare(Token name) {
    if (scopes.empty()) return;
    map<string, bool> scope = scopes.back();
    if (scope.find(name.lexeme) != scope.end()) {
      lox::error(name.line,
          "Variable with this name already declared in this scope.");
    }
    scope[name.lexeme] = false;
    scopes.pop_back();
    scopes.emplace_back(scope);
}

void Resolver::define(Token name) {
    if (scopes.empty()) return;
    map<string, bool> scope = scopes.back();
    scope[name.lexeme] = true;
    scopes.pop_back();
    scopes.emplace_back(scope);
}

void Resolver::resolveLocal(shared_ptr<Expr<Object>> expr, Token name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
        auto searched = scopes[i].find(name.lexeme);
        if (searched != scopes[i].end()) {
            interpreter->resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
    // Not found. Assume it is global.
}

void Resolver::resolveFunction(
    shared_ptr<Function> function,
    FunctionType type
) {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for (Token param : function->params) {
      declare(param);
      define(param);
    }
    resolve(function->body);
    endScope();
    currentFunction = enclosingFunction;
}