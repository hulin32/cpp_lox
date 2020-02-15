// Copyright 2020 <Copyright hulin>


#include <string>
#include <memory>
#include <iostream>
#include <vector>

#include "./Interpreter.hpp"
#include "./Expr.hpp"
#include "./Token.hpp"
#include "./Stmt.hpp"
#include "./RuntimeError.hpp"
#include "./lox.hpp"

using std::stod;
using std::shared_ptr;
using std::cout;
using std::endl;
using std::vector;

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


Object Interpreter::visitLiteralExpr(const Literal<Object>& expr) {
    switch (expr.value.type) {
        case Object::Object_bool:
            return Object::make_bool_obj(expr.value.boolean);
        case Object::Object_nil:
            return Object::make_nil_obj();
        case Object::Object_num:
            return Object::make_num_obj(expr.value.num);
        default:
            return Object::make_str_obj(expr.value.str);
    }
}

Object Interpreter::visitGroupingExpr(const Grouping<Object>& expr) {
    return evaluate(expr.expression);
}

Object Interpreter::visitUnaryExpr(const Unary<Object>& expr) {
    Object right = evaluate(expr.right);
    switch (expr.operation.type) {
        case BANG:
            return Object::make_bool_obj(!isTruthy(right));
        case MINUS:
            return Object::make_num_obj(-stod(right.toString()));
        default:
            // Unreachable.
            return Object::make_nil_obj();
    }
}

Object Interpreter::visitBinaryExpr(const Binary<Object>& expr) {
    Object left = evaluate(expr.left);
    Object right = evaluate(expr.right);
    bool result_bool = false;
    string result_str = "";
    double result_num = 0;
    switch (expr.operation.type) {
      case GREATER:
        checkNumberOperands(expr.operation, left, right);
        result_bool = left.num > right.num;
        return Object::make_bool_obj(result_bool);
      case GREATER_EQUAL:
        checkNumberOperands(expr.operation, left, right);
        result_bool = left.num >= right.num;
        return Object::make_bool_obj(result_bool);
      case LESS:
        checkNumberOperands(expr.operation, left, right);
        result_bool = left.num < right.num;
        return Object::make_bool_obj(result_bool);
      case LESS_EQUAL:
        checkNumberOperands(expr.operation, left, right);
        result_bool = left.num <= right.num;
        return Object::make_bool_obj(result_bool);
      case MINUS:
        checkNumberOperand(expr.operation, right);
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
         throw RuntimeError(expr.operation,
            "Operands must be two numbers or two strings.");
      case SLASH:
        checkNumberOperands(expr.operation, left, right);
        result_num = left.num / right.num;
        return Object::make_num_obj(result_num);
      case STAR:
        checkNumberOperands(expr.operation, left, right);
        result_num = left.num * right.num;
        return Object::make_num_obj(result_num);
      case BANG_EQUAL: return Object::make_bool_obj(!isEqual(left, right));
      case EQUAL_EQUAL: return Object::make_bool_obj(isEqual(left, right));
      default:
        return Object::make_nil_obj();
    }
}

Object Interpreter::visitAssignExpr(const Assign<Object>& expr) {
    return Object::make_nil_obj();
}

Object Interpreter::visitVariableExpr(const Variable<Object>& expr) {
    return environment.get(expr.name);
}

void Interpreter::visitExpressionStmt(const Expression& stmt) {
    evaluate(stmt.expression);
}

void Interpreter::visitPrintStmt(const Print& stmt) {
    Object value = evaluate(stmt.expression);
    cout << stringify(value) << endl;
}

void Interpreter::visitVarStmt(const Var& stmt) {
    // cout << stringify(value) << endl;
    Object value = Object::make_nil_obj();
    if (stmt.initializer != nullptr) {
      value = evaluate(stmt.initializer);
    }
    environment.define(stmt.name.lexeme, value);
}

Object Interpreter::evaluate(shared_ptr<Expr<Object>> expr) {
    return expr->accept(shared_from_this());
}

void Interpreter::execute(shared_ptr<Stmt> stmt) {
    stmt->accept(shared_from_this());
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
