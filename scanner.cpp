// Copyright 2020 <Copyright hulin>

#include <string>
#include <list>

#include "./scanner.h"
#include "./token.h"
#include "./lox.h"

using std::string;

map<string, TokenType> scanner::keywords = {
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

scanner::scanner(string source): source(source) {}

scanner::~scanner() {
    for (Token *token : tokens) {
        delete token;
    }
}

list<Token*> scanner::scanTokens() {
    while (!isAtEnd()) {
      // We are at the beginning of the next lexeme.
      start = current;
      scanToken();
    }

    tokens.push_back(new Token(TOKEN_EOF, "", "", line));
    return tokens;
}

bool scanner::isAtEnd() {
    return current >= source.size();
}

void scanner::scanToken() {
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

char scanner::advance() {
    current++;
    return source.at(current - 1);
}

void scanner::addToken(TokenType type) {
    addToken(type, "");
}

void scanner::addToken(TokenType type, Object literal) {
    string text = source.substr(start, current);
    tokens.push_back(new Token(type, text, literal, line));
}

bool scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source.at(current) != expected) return false;
    current++;
    return true;
}

void scanner::generateString() {
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
    // TODO(hulin) may need to change
    string value = source.substr(start + 1, current - 1);
    addToken(STRING, "");
}

void scanner::generateNumber() {
    while (isDigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
      // Consume the "."
      advance();
      while (isDigit(peek())) advance();
    }
    // TODO(hulin) double
    addToken(NUMBER, source.substr(start, current));
}

void scanner::identifier() {
    while (isAlphaNumeric(peek())) advance();
    string text = source.substr(start, current);
    auto found = keywords.find(text);
    TokenType type;
    if (found != keywords.end()) {
        type = found->second;
    } else {
        type = IDENTIFIER;
    }
    addToken(type);
}

bool scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_';
}

bool scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

char scanner::peek() {
    if (isAtEnd()) return '\0';
    return source.at(current);
}

char scanner::peekNext() {
    if (current + 1 >= source.size()) return '\0';
    return source.at(current + 1);
}
