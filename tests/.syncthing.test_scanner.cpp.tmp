#include "grammar/terminals.hpp"
#include "readfile/readfile.hpp"
#include "util/errors.hpp"
#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <scanner/scanner.hpp>



TEST_CASE("[3|SCN] Invalid program") {
    vector<string> input = readfile::Read("../../tests/resources/scanner_fail_1.txt");
    
    errors::Reset();
    scanner::Reset();
    scanner::Scan(input);

    REQUIRE(errors::GetErrorCode() == errors::UNRECOGNIZED_SYMBOL);
};

TEST_CASE("[3|SCN] Valid but nonsensical program") {
    vector<string> input = readfile::Read("../../tests/resources/scanner_fail_2.txt");
    vector<Token> expected = {
        {IDENTIFIER, "I"}, 
        {IDENTIFIER, "AM"}, 
        {ADD, "+"},
        {IDENTIFIER, "BAYEH"},
        {NEWLINE, ""},

        {IDENTIFIER, "AND"},
        {NUMBER, "1"},
        {IDENTIFIER, "SPEAK"},
        {IDENTIFIER, "FOR"},
        {DOT, "."},
        {OPEN_PARENTHESIS, "("},
        {IDENTIFIER, "THE"},
        {CLOSE_PARENTHESIS, ")"},
        {IDENTIFIER, "TREES"},
        {NEWLINE, ""},
    };

    errors::Reset();
    scanner::Reset();
    vector<Token> output = scanner::Scan(input);

    REQUIRE(errors::GetErrorCode() == NONE);
    REQUIRE(output.size() == expected.size());

    for (int i = 0; i < expected.size(); i++) {
        REQUIRE(expected.at(i).type == output.at(i).type);
        REQUIRE(expected.at(i).text == output.at(i).text);
    }
}

TEST_CASE("[3|SCN] All tokens") {
    vector<string> input = readfile::Read("../../tests/resources/scanner_pass_1.txt");
    vector<Token> expected = {
        {NEWLINE, ""},

        {POINTER, "@"},
        {COMMA, ","},
        {DOT, "."},
        {COLON, ":"},
        {SEMICOLON, ";"},
        {OPEN_SQUARE_BRACKET, "["},
        {CLOSE_SQUARE_BRACKET, "]"},
        {OPEN_PARENTHESIS, "("},
        {CLOSE_PARENTHESIS, ")"},
        {OPEN_BRACE, "{"},
        {ASSIGN, "="},
        {CLOSE_BRACE, "}"},
        {EQUALS, "=="},
        {NOT_EQUALS, "!="},
        {GREATER_OR_EQUAL, "=>"},
        {LESS_OR_EQUAL, "=<"},
        {ADD_ASSIGN, "+="},
        {SUBTRACT, "-"},
        {SUBTRACT_ASSIGN, "-="},
        {REFERENCE, "->"},
        {NEWLINE, ""},

        {NUMBER, "0"},
        {NUMBER, "12935"},
        {NUMBER, "8942758706"},
        {NUMBER, "-4829367864"},
        {NUMBER, "-0"},
        {NEWLINE, ""},

        {MULTIPLY_ASSIGN, "*="},
        {DIVIDE_ASSIGN, "/="},
        {MODULUS_ASSIGN, "%="},
        {GREATER_OR_EQUAL, ">="},
        {LESS_OR_EQUAL, "<="},
        {DEREFERENCE, "<-"},
        {NEWLINE, ""},

        {AND, "and"},
        {BREAK, "break"},
        {CALL, "call"},
        {CASE, "case"},
        {CONTINUE, "continue"},
        {SEMICOLON, ";"},
        {NEWLINE, ""},

        {DEFAULT, "default"},
        {ELSE, "else"},
        {ELSEIF, "elseif"},
        {FOR, "for"},
        {FALSE, "false"},
        {INPUT, "input"},
        {IF, "if"},
        {SEMICOLON, ";"},
        {NEWLINE, ""},

        {INT4, "int4"},
        {INT8, "int8"},
        {INT12, "int12"},
        {INT16, "int16"},
        {INT32, "int32"},
        {INT64, "int64"},
        {SEMICOLON, ";"},
        {NEWLINE, ""},

        {NOT, "not"},
        {OR, "or"},
        {OUTPUT, "output"},
        {RETURN, "return"},
        {SWITCH, "switch"},
        {TRUE, "true"},
        {VOID, "void"},
        {WHILE, "while"},
        {SEMICOLON, ";"},
        {NEWLINE, ""},

        {IDENTIFIER, "identifiers"},
        {IDENTIFIER, "Yes"},
        {IDENTIFIER, "jUs"},
        {IDENTIFIER, "KI"},
        {IDENTIFIER, "skjnafionefne_AWFsanus_aWSngjeke"},
        {IDENTIFIER, "jeamoieea_P"},
        {IDENTIFIER, "_"},
        {IDENTIFIER, "PPPPP"},
        {IDENTIFIER, "i"},
        {IDENTIFIER, "jie"},
        {IDENTIFIER, "n"},
        {NEWLINE, ""}};

    errors::Reset();
    scanner::Reset();
    vector<Token> output = scanner::Scan(input);

    REQUIRE(errors::GetErrorCode() == NONE);
    REQUIRE(output.size() == expected.size());

    for (int i = 0; i < output.size(); i++) {
        CAPTURE(expected.at(i).type, output.at(i).type);
        CAPTURE(expected.at(i).text, output.at(i).text);
        REQUIRE(expected.at(i).type == output.at(i).type);
        REQUIRE(expected.at(i).text == output.at(i).text);
    }
}

TEST_CASE("[3|SCN] Valid program") {
    vector<string> input = readfile::Read("../../tests/resources/scanner_pass_2.txt");
    vector<Token> expected = {
        {INT64, "int64"},
        {IDENTIFIER, "Factorial"},
        {OPEN_PARENTHESIS, "("},
        {INT32, "int32"},
        {IDENTIFIER, "x"},
        {CLOSE_PARENTHESIS, ")"},
        {OPEN_BRACE, "{"},
        {NEWLINE, ""},

        {INT16, "int16"},
        {IDENTIFIER, "y"},
        {ASSIGN, "="},
        {NUMBER, "1"},
        {SEMICOLON, ";"},
        {NEWLINE, ""},

        {FOR, "for"},
        {OPEN_PARENTHESIS, "("},
        {INT8, "int8"},
        {IDENTIFIER, "i"},
        {ASSIGN, "="},
        {NUMBER, "0"},
        {SEMICOLON, ";"},
        {IDENTIFIER, "i"},
        {LESS, "<"},
        {IDENTIFIER, "x"},
        {SEMICOLON, ";"},
        {IDENTIFIER, "i"},
        {ADD_ASSIGN, "+="},
        {NUMBER, "1"},
        {CLOSE_PARENTHESIS, ")"},
        {OPEN_BRACE, "{"},
        {NEWLINE, ""},

        {IDENTIFIER, "y"},
        {MULTIPLY_ASSIGN, "*="},
        {IDENTIFIER, "i"},
        {SEMICOLON, ";"},
        {NEWLINE, ""},

        {CLOSE_BRACE, "}"},
        {NEWLINE, ""},

        {RETURN, "return"},
        {IDENTIFIER, "y"},
        {SEMICOLON, ";"},
        {NEWLINE, ""},

        {CLOSE_BRACE, "}"},
        {NEWLINE, ""},
        {NEWLINE, ""},

        {INT4, "int4"},
        {IDENTIFIER, "x"},
        {ASSIGN, "="},
        {INPUT, "input"},
        {SEMICOLON, ";"},
        {NEWLINE, ""},

        {INT4, "int4"},
        {IDENTIFIER, "z"},
        {ASSIGN, "="},
        {NUMBER, "-3"},
        {SEMICOLON, ";"},
        {NEWLINE, ""},

        {INT12, "int12"},
        {IDENTIFIER, "a"},
        {ASSIGN, "="},
        {TRUE, "true"},
        {SEMICOLON, ";"},
        {NEWLINE, ""},

        {IF, "if"},
        {OPEN_PARENTHESIS, "("},
        {IDENTIFIER, "a"},
        {EQUALS, "=="},
        {TRUE, "true"},
        {CLOSE_PARENTHESIS, ")"},
        {OPEN_BRACE, "{"},
        {NEWLINE, ""},

        {IDENTIFIER, "x"},
        {DIVIDE_ASSIGN, "/="},
        {IDENTIFIER, "z"},
        {SEMICOLON, ";"},
        {NEWLINE, ""},

        {CLOSE_BRACE, "}"},
        {NEWLINE, ""},

        {OUTPUT, "output"},
        {IDENTIFIER, "Factorial"},
        {OPEN_PARENTHESIS, "("},
        {IDENTIFIER, "x"},
        {CLOSE_PARENTHESIS, ")"},
        {SEMICOLON, ";"},
        {NEWLINE, ""}
    };

    errors::Reset();
    scanner::Reset();
    vector<Token> output = scanner::Scan(input);

    errors::OutputErrors();

    REQUIRE(errors::GetErrorCode() == NONE);
    REQUIRE(output.size() == expected.size());

    for (int i = 0; i < output.size(); i++) {
        CAPTURE(expected.at(i).type, output.at(i).type);
        CAPTURE(expected.at(i).text, output.at(i).text);
        REQUIRE(expected.at(i).type == output.at(i).type);
        REQUIRE(expected.at(i).text == output.at(i).text);
    }
}