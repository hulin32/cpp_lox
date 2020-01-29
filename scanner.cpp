// Copyright 2020 <Copyright hulin>

#include <string>
#include <list>

#include "./Scanner.hpp"
#include "./Token.hpp"
#include "./lox.hpp"

using std::string;

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

Scanner::~Scanner() {
    for (Token *token : tokens) {
        delete token;
    }
}

list<Token*> Scanner::scanTokens() {
    while (!isAtEnd()) {
      // We are at the beginning of the next lexeme.
      start = current;
      scanToken();
    }

    tokens.push_back(new Token(TOKEN_EOF, "", "", line));
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
    addToken(type, "");
}

void Scanner::addToken(TokenType type, Object literal) {
    string text = source.substr(start, current);
    tokens.push_back(new Token(type, text, literal, line));
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
    // TODO(hulin) may need to change
    string value = source.substr(start + 1, current - 1);
    addToken(STRING, "");
}

void Scanner::generateNumber() {
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

void Scanner::identifier() {
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
