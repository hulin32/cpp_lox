// Copyright 2020 <Copyright hulin>

#include <string>
#include <vector>
#include <iostream>

#include "./Scanner.hpp"
#include "./Token.hpp"
#include "./lox.hpp"

using std::string;
using std::stod;
using std::vector;
using std::cout;
using std::endl;

map<string, TokenType> Scanner::keywords = {
    {"and", AND},
    {"class",  CLASS},
    {"else",   ELSE},
    {"false",  FALSE},
    {"for",    FOR},
    {"fun",    FUN},
    {"if",     IF},
    {"nil",    NIL},
    {"or",     OR},
    {"print",  PRINT},
    {"return", RETURN},
    {"super",  SUPER},
    {"this",   THIS},
    {"true",   TRUE},
    {"var",    VAR},
    {"while",  WHILE},
};

Scanner::Scanner(string source): source(source) {}

vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
      // We are at the beginning of the next lexeme.
      start = current;
      scanToken();
    }

    Token token(TOKEN_EOF, "", Object::make_str_obj(""), line);
    tokens.push_back(token);
    return tokens;
}

bool Scanner::isAtEnd() {
    return current >= source.size();
}

void Scanner::scanToken() {
    char c = advance();

    switch (c) {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '-': addToken(MINUS); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case '*': addToken(STAR); break;
        case '!': addToken(match('=') ? BANG_EQUAL : BANG); break;
        case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
        case '<': addToken(match('=') ? LESS_EQUAL : LESS); break;
        case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;
        case '/':
            if (match('/')) {
                // A comment goes until the end of the line.
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                addToken(SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;
        case '\n':
            line++;
            break;
        case '"':
            generateString();
            break;
        default:
            if (isDigit(c)) {
                generateNumber();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                lox::error(line, "Unexpected character.");
            }
            break;
    }
}

char Scanner::advance() {
    current++;
    return source.at(current - 1);
}

void Scanner::addToken(TokenType type) {
    addToken(type, Object::make_str_obj(""));
}

void Scanner::addToken(TokenType type, Object literal) {
    string text = source.substr(start, current - start);
    Token token(type, text, literal, line);
    tokens.push_back(token);
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source.at(current) != expected) return false;
    current++;
    return true;
}

void Scanner::generateString() {
    while (peek() != '"' && !isAtEnd()) {
      if (peek() == '\n') line++;
      advance();
    }

    // Unterminated string.
    if (isAtEnd()) {
      lox::error(line, "Unterminated string.");
      return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    string value = source.substr(start + 1, current - start - 2);
    addToken(STRING, Object::make_str_obj(value));
}

void Scanner::generateNumber() {
    while (isDigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
      // Consume the "."
      advance();
      while (isDigit(peek())) advance();
    }

    double num = stod(source.substr(start, current - start));
    addToken(NUMBER, Object::make_num_obj(num));
}

void Scanner::identifier() {
    while (isAlphaNumeric(peek())) advance();
    string text = source.substr(start, current - start);
    auto found = keywords.find(text);
    TokenType type;
    if (found != keywords.end()) {
        type = found->second;
    } else {
        type = IDENTIFIER;
    }
    addToken(type);
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_';
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return source.at(current);
}

char Scanner::peekNext() {
    if (current + 1 >= source.size()) return '\0';
    return source.at(current + 1);
}
