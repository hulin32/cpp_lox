cmake_minimum_required(VERSION 3.5)

project(cpp_lox LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
        LoxInstance.cpp LoxInstance.hpp
        LoxClass.cpp LoxClass.hpp
        Resolver.cpp Resolver.hpp
        Token.cpp Token.hpp
        LoxFunction.cpp LoxFunction.hpp
        Environment.cpp Environment.hpp
        Interpreter.cpp Interpreter.hpp
        lox.cpp lox.hpp
        Parser.cpp Parser.hpp
        Scanner.cpp Scanner.hpp
        Stmt.hpp Expr.hpp
        main.cpp
        )
add_executable(cpp_lox ${SOURCE_FILES})
