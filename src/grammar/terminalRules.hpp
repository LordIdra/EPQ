#pragma once

#include "grammar/terminals.hpp"
#include <util/types.hpp>



// IDENTIFIER and NUMBER are done in stateGenerator.cpp due to being more complex rules
const unordered_map<Terminal, string> terminalRules = {
    // Whitespace
    {NO_TOKEN, " "},
    {NO_TOKEN, "\n"},
    {COMMENT, "#"},

    // Punctuators
    {COMMA, ","},
    {DOT, "."},
    {COLON, ":"},
    {SEMICOLON, ";"},
    {OPEN_SQUARE_BRACKET, "["},
    {CLOSE_SQUARE_BRACKET, "]"},
    {OPEN_PARENTHESIS, "("},
    {CLOSE_PARENTHESIS, ")"},
    {OPEN_BRACE, "{"},
    {CLOSE_BRACE, "}"},

    // Operators
    {ASSIGN, "="},
    {NOT_EQUALS, "!="},
    {EQUALS, "=="},
    {GREATER, ">"},
    {GREATER_OR_EQUAL, ">="},
    {LESS, "<"},
    {LESS_OR_EQUAL, "=<"},
    {INCREMENT, "++"},
    {DECREMENT, "--"},
    {ADD, "+"},
    {ADD_ASSIGN, "+="},
    {SUBTRACT, "-"},
    {SUBTRACT_ASSIGN, "-="},
    {MULTIPLY, "*"},
    {MULTIPLY_ASSIGN, "*="},
    {DIVIDE, "/"},
    {DIVIDE_ASSIGN, "/="},
    {MODULUS, "%"},
    {MODULUS_ASSIGN, "%="},
    {REFERENCE, "->"},
    {DEREFERENCE, "<-"},

    // Literals
    {TRUE, "true"},
    {FALSE, "false"},

    // Datatypes
    {VOID, "void"},
    {INT4, "int4"},
    {INT8, "int8"},
    {INT12, "int12"},
    {INT16, "int16"},
    {INT32, "int32"},
    {INT64, "int64"},

    // Keywords
    {NOT, "!"},
    {OR, "|"},
    {AND, "&"},
    {IF, "if"},
    {ELSE, "else"},
    {ELSEIF, "elseif"},
    {FOR, "for"},
    {WHILE, "while"},
    {CONTINUE, "continue"},
    {BREAK, "break"},
    {RETURN, "return"},
    {OUTPUT, "output"},
    {INPUT, "input"},
    {CALL, "call"},
};