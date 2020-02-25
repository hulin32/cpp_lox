// Copyright 2020 <Copyright hulin>

#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <stdexcept>

#include "./Interpreter.hpp"
#include "./Expr.hpp"
#include "./Token.hpp"
#include "./Stmt.hpp"
#include "./RuntimeError.hpp"
#include "./Environment.hpp"
#include "./LoxCallable.hpp"
#include "./LoxInstance.hpp"
#include "./LoxClass.hpp"
#include "./LoxFunction.hpp"
#include "./ReturnError.hpp"
#include "./lox.hpp"

using std::stod;
using std::shared_ptr;
using std::cout;
using std::endl;
using std::vector;
using std::exception;
using std::to_string;

bool endsWith(std::string str, std::string suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.substr(str.length() - suffix.length()) == suffix;
}

void Interpreter::interpret(vector<shared_ptr<Stmt>> statements) {
    try {
      for (auto statement : statements) {
        execute(statement);
      }
    } catch (RuntimeError error) {
      lox::runtimeError(error);
    }
}

string Interpreter::stringify(Object object) {
    string text = object.toString();
    string check_str = ".000000";
    if (endsWith(text, check_str)) {
        return text.erase(text.size() - check_str.size());
    }
    return text;
}


Object Interpreter::visitLiteralExpr(shared_ptr<Literal<Object>> expr) {
    switch (expr->value.type) {
        case Object::Object_bool:
            return Object::make_bool_obj(expr->value.boolean);
        case Object::Object_nil:
            return Object::make_nil_obj();
        case Object::Object_num:
            return Object::make_num_obj(expr->value.num);
        default:
            return Object::make_str_obj(expr->value.str);
    }
}

Object Interpreter::visitGroupingExpr(shared_ptr<Grouping<Object>> expr) {
    return evaluate(expr->expression);
}

Object Interpreter::visitUnaryExpr(shared_ptr<Unary<Object>> expr) {
    Object right = evaluate(expr->right);
    switch (expr->operation.type) {
        case BANG:
            return Object::make_bool_obj(!isTruthy(right));
        case MINUS:
            return Object::make_num_obj(-stod(right.toString()));
        default:
            // Unreachable.
            return Object::make_nil_obj();
    }
}

Object Interpreter::visitBinaryExpr(shared_ptr<Binary<Object>> expr) {
    Object left = evaluate(expr->left);
    Object right = evaluate(expr->right);
    bool result_bool = false;
    string result_str = "";
    double result_num = 0;
    Object foo;
    switch (expr->operation.type) {
      case GREATER:
        checkNumberOperands(expr->operation, left, right);
        result_bool = left.num > right.num;
        return Object::make_bool_obj(result_bool);
      case GREATER_EQUAL:
        checkNumberOperands(expr->operation, left, right);
        result_bool = left.num >= right.num;
        return Object::make_bool_obj(result_bool);
      case LESS:
        checkNumberOperands(expr->operation, left, right);
        result_bool = left.num < right.num;
        foo = Object::make_bool_obj(result_bool);
        foo = Object::make_bool_obj(result_bool);
        return Object::make_bool_obj(result_bool);
      case LESS_EQUAL:
        checkNumberOperands(expr->operation, left, right);
        result_bool = left.num <= right.num;
        return Object::make_bool_obj(result_bool);
      case MINUS:
        checkNumberOperand(expr->operation, right);
        result_num = left.num - right.num;
        return Object::make_num_obj(result_num);
      case PLUS:
        if (left.type == Object::Object_num &&
            right.type == Object::Object_num) {
            result_num = left.num + right.num;
            return Object::make_num_obj(result_num);
        }
        if (left.type == Object::Object_str &&
            right.type == Object::Object_str) {
            result_str = left.toString() + right.toString();
            return Object::make_str_obj(result_str);
        }
         throw RuntimeError(expr->operation,
            "Operands must be two numbers or two strings.");
      case SLASH:
        checkNumberOperands(expr->operation, left, right);
        result_num = left.num / right.num;
        return Object::make_num_obj(result_num);
      case STAR:
        checkNumberOperands(expr->operation, left, right);
        result_num = left.num * right.num;
        return Object::make_num_obj(result_num);
      case BANG_EQUAL: return Object::make_bool_obj(!isEqual(left, right));
      case EQUAL_EQUAL: return Object::make_bool_obj(isEqual(left, right));
      default:
        return Object::make_nil_obj();
    }
}

Object Interpreter::visitAssignExpr(shared_ptr<Assign<Object>> expr) {
    Object value = evaluate(expr->value);

    // environment->assign(expr->name, value);
    auto distance = locals.find(expr);
    if (distance != locals.end()) {
      environment->assignAt(distance->second, expr->name, value);
    } else {
      globals->assign(expr->name, value);
    }
    return value;
}

Object Interpreter::visitLogicalExpr(shared_ptr<Logical<Object>> expr) {
    Object left = evaluate(expr->left);
    if (expr->operation.type == OR) {
      if (isTruthy(left)) return left;
    } else {
      if (!isTruthy(left)) return left;
    }
    return evaluate(expr->right);
}

Object Interpreter::visitVariableExpr(shared_ptr<Variable<Object>> expr) {
    return lookUpVariable(expr->name, expr);
}

Object Interpreter::lookUpVariable(
    Token name, shared_ptr<Expr<Object>> expr) {
    auto distance = locals.find(expr);
    if (distance != locals.end()) {
      return environment->getAt(distance->second, name.lexeme);
    } else {
      return globals->get(name);
    }
}

Object Interpreter::visitCallExpr(shared_ptr<Call<Object>> expr) {
    Object callee = evaluate(expr->callee);

    vector<Object> arguments;
    for (auto argument : expr->arguments) {
      arguments.push_back(evaluate(argument));
    }

    if (callee.type != Object::Object_fun &&
        callee.type != Object::Object_class) {
        throw RuntimeError(expr->paren,
          "Can only call functions and classes.");
    }

    shared_ptr<LoxCallable> callable;
    if (callee.type == Object::Object_fun) {
        callable = callee.function;
    }
    if (callee.type == Object::Object_class) {
        callable = callee.lox_class;
    }
    if (arguments.size() != callable->arity()) {
      throw RuntimeError(expr->paren, "Expected " +
          to_string(callable->arity()) + " arguments but got " +
          to_string(arguments.size()) + ".");
    }
    return callable->call(shared_from_this(), arguments);
}

Object Interpreter::visitGetExpr(shared_ptr<Get<Object>> expr) {
    Object object = evaluate(expr->object);
    if (object.type == Object::Object_instance) {
       return (object.instance)->get(expr->name);
    }

    throw RuntimeError(expr->name,
        "Only instances have properties.");
}

Object Interpreter::visitSetExpr(shared_ptr<Set<Object>> expr) {
    Object object = evaluate(expr->object);

    if (object.type != Object::Object_instance) {
      throw RuntimeError(expr->name, "Only instances have fields.");
    }

    Object value = evaluate(expr->value);
    object.instance->set(expr->name, value);
    return value;
}

Object Interpreter::visitThisExpr(shared_ptr<This<Object>> expr) {
    return lookUpVariable(expr->keyword, expr);
}

void Interpreter::visitExpressionStmt(const Expression& stmt) {
    evaluate(stmt.expression);
}

void Interpreter::visitPrintStmt(const Print& stmt) {
    Object value = evaluate(stmt.expression);
    cout << stringify(value) << endl;
}

void Interpreter::visitReturnStmt(const Return& stmt) {
    Object value;
    if (stmt.value != nullptr) value = evaluate(stmt.value);
    throw ReturnError(value);
}

void Interpreter::visitVarStmt(const Var& stmt) {
    Object value = Object::make_nil_obj();
    if (stmt.initializer != nullptr) {
      value = evaluate(stmt.initializer);
    }
    environment->define(stmt.name.lexeme, value);
}

void Interpreter::visitBlockStmt(const Block& stmt) {
    shared_ptr<Environment> env(new Environment(environment));
    executeBlock(stmt.statements, env);
}

void Interpreter::visitClassStmt(const Class& stmt) {
    environment->define(stmt.name.lexeme, Object::make_nil_obj());

    map<string, shared_ptr<LoxFunction>> methods;
    for (auto method : stmt.methods) {
        bool is_init = method->name.lexeme == "init";
        shared_ptr<LoxFunction> function(
            new LoxFunction(method, environment, is_init)
        );

        methods[method->name.lexeme] = function;
    }

    auto klass = shared_ptr<LoxClass>(new LoxClass(stmt.name.lexeme, methods));
    environment->assign(stmt.name, Object::make_class_obj(klass));
}

void Interpreter::visitWhileStmt(const While& stmt) {
    while (isTruthy(evaluate(stmt.condition))) {
      Object obj =  evaluate(stmt.condition);
      execute(stmt.body);
    }
}

void Interpreter::visitIfStmt(const If& stmt) {
    if (isTruthy(evaluate(stmt.condition))) {
      execute(stmt.thenBranch);
    } else if (stmt.elseBranch != nullptr) {
      execute(stmt.elseBranch);
    }
}

void Interpreter::visitFunctionStmt(shared_ptr<Function> stmt) {
    shared_ptr<LoxFunction> function(new LoxFunction(stmt, environment, false));
    Object obj = Object::make_fun_obj(function);
    environment->define(stmt->name.lexeme, obj);
}

Object Interpreter::evaluate(shared_ptr<Expr<Object>> expr) {
    return expr->accept(shared_from_this());
}

void Interpreter::execute(shared_ptr<Stmt> stmt) {
    stmt->accept(shared_from_this());
}

void Interpreter::resolve(shared_ptr<Expr<Object>> expr, int depth) {
    locals[expr] = depth;
}

void Interpreter::executeBlock(
    vector<shared_ptr<Stmt>> statements,
    shared_ptr<Environment> env
) {
    shared_ptr<Environment> previous = environment;
    environment = env;
    for (auto statement : statements) {
        execute(statement);
    }
    environment = previous;
  }

bool Interpreter::isTruthy(Object object) {
    if (object.type == Object::Object_nil) {
        return false;
    }
    if (object.type == Object::Object_bool) {
        return object.boolean;
    }
    return true;
}

bool Interpreter::isEqual(Object a, Object b) {
    if (a.type == Object::Object_nil
        && b.type == Object::Object_nil) {
            return true;
        }
    if (a.type == Object::Object_nil) return false;
    if (a.type == b.type) {
        switch (a.type) {
            case Object::Object_bool:
                return a.boolean == b.boolean;
            case Object::Object_nil:
                return a.nil == b.nil;
            case Object::Object_num:
                return a.num == b.num;
            case Object::Object_str:
                return a.str == b.str;
            default:
                return false;
        }
    } else {
        return false;
    }
}

void Interpreter::checkNumberOperand(Token operation, Object operand) {
    if (operand.type == Object::Object_num) return;
    throw RuntimeError(operation, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(
    Token operation,
    Object left,
    Object right
) {
    if (left.type == Object::Object_num &&
        right.type == Object::Object_num) return;
    throw RuntimeError(operation, "Operand must be a number.");
}
