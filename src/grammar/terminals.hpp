#pragma once

#include <util/types.hpp>



enum Terminal {
    // 'NONE' = the state does not correspond to a token
    // 'NO_TOKEN' = final state, but don't write any tokens
    NONE,
    NEWLINE,
    NO_TOKEN,
    COMMENT,

    // Punctuators
    POINTER,
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
    NOT_EQUALS,
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
    VOID,
    INT1,
    INT2,
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
    ELSEIF,
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

const int FIRST_TERMINAL = NONE;
const int LAST_TERMINAL = INPUT;

struct Token { 
    int type;
    string text;
};