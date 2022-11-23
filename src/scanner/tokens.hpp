#pragma once

#include <util/types.hpp>



enum TokenType {
    // 'NONE' = no transitions from the current token
    // 'NO_TOKEN' = final state, but don't write any tokens
    NONE,
    NO_TOKEN,
    
    // Punctuators
    COMMA,
    DOT,
    COLON,
    SEMICOLON,
    OPEN_SQUARE_BRACKET,
    CLOSE_SQUARE_BRACKET,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    OPEN_BRACE,
    CLOSE_BRACE,

    // Operators
    ASSIGN,
    EQUALS,
    GREATER,
    GREATER_OR_EQUAL,
    LESS,
    LESS_OR_EQUAL,
    ADD,
    ADD_ASSIGN,
    SUBTRACT,
    SUBTRACT_ASSIGN,
    MULTIPLY,
    MULTIPLY_ASSIGN,
    DIVIDE,
    DIVIDE_ASSIGN,
    MODULUS,
    MODULUS_ASSIGN,
    REFERENCE,
    DEREFERENCE,

    // Identifiers
    IDENTIFIER,

    // Literals
    TRUE,
    FALSE,
    NUMBER,

    // Datatypes
    BOOL,
    INT4,
    INT8,
    INT16,
    INT32,
    INT64,

    // Keywords
    NOT,
    OR,
    AND,
    CONST,
    IF,
    ELSE,
    SWITCH,
    CASE,
    DEFAULT,
    FOR,
    WHILE,
    CONTINUE,
    BREAK,
    RETURN,
    OUTPUT,
    INPUT
};

struct Token { 
    TokenType type;
    string text;
};

