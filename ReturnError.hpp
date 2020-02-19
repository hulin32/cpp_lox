// Copyright 2020 <Copyright hulin>
#ifndef RETURN_ERROR_HPP_
#define RETURN_ERROR_HPP_

#include <stdexcept>
#include "./Token.hpp"

using std::runtime_error;

class ReturnError: public runtime_error {
 public:
    Object value;
    explicit ReturnError(Object value_): runtime_error(""), value(value_) {}
};

#endif  //  RETURN_ERROR_HPP_
