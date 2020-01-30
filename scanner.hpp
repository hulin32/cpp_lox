// Copyright 2020 <Copyright hulin>

#ifndef SCANNER_HPP_
#define SCANNER_HPP_

#include <string>
#include <vector>
#include <map>
#include "./Token.hpp"

using std::string;
using std::map;
using std::vector;

class Scanner {
 private:
    string source;
    vector<Token> tokens;
    static map<string, TokenType> keywords;
    int start = 0;
    int current = 0;
    int line = 1;
    void scanToken();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, Object literal);
    bool match(char expected);
    char peek();
    void generateString();
    void generateNumber();
    bool isDigit(char c);
    char peekNext();
    void identifier();
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);

 public:
    explicit Scanner(string source);
    vector<Token> scanTokens();
    bool isAtEnd();
};

#endif  // SCANNER_HPP_


