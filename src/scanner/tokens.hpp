#pragma once

#include <util/types.h>



namespace tokens {
    struct Token { const string text; };

    // Punctuators
    struct COMMA                : Token {};
    struct DOT                  : Token {};
    struct COLON                : Token {};
    struct SEMICOLON            : Token {};
    struct OPEN_SQUARE_BRACKET  : Token {};
    struct CLOSE_SQUARE_BRACKET : Token {};
    struct OPEN_PARENTHESIS     : Token {};
    struct CLOSE_PARENTHESIS    : Token {};

    // Operators
    struct ASSIGN           : Token {};
    struct EQUALS           : Token {};
    struct GREATER          : Token {};
    struct GREATER_OR_EQUAL : Token {};
    struct LESS             : Token {};
    struct LESS_OR_EQUAL    : Token {};
    struct ADD              : Token {};
    struct ADD_ASSIGN       : Token {};
    struct SUBTRACT         : Token {};
    struct SUBTRACT_ASSIGN  : Token {};
    struct MULTIPLY         : Token {};
    struct MULTIPLY_ASSIGN  : Token {};
    struct DIVIDE           : Token {};
    struct DIVIDE_ASSIGN    : Token {};
    struct MODULUS          : Token {};
    struct MODULUS_ASSIGN   : Token {};
    struct REFERENCE        : Token {};
    struct DEREFERENCE      : Token {};

    // Literals
    struct TRUE     : Token {};
    struct FALSE    : Token {};
    struct NUMBER   : Token {};

    // Datatypes
    struct BOOL  : Token {};
    struct INT4  : Token {};
    struct INT8  : Token {};
    struct INT16 : Token {};
    struct INT32 : Token {};
    struct INT64 : Token {};

    // Keywords
    struct NOT      : Token {};
    struct OR       : Token {};
    struct AND      : Token {};
    struct CONST    : Token {};
    struct IF       : Token {};
    struct ELSE     : Token {};
    struct SWITCH   : Token {};
    struct CASE     : Token {};
    struct DEFAULT  : Token {};
    struct FOR      : Token {};
    struct WHILE    : Token {};
    struct CONTINUE : Token {};
    struct BREAK    : Token {};
    struct RETURN   : Token {};
}