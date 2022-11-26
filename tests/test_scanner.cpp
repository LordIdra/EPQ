#include "scanner/tokens.hpp"
#include <catch2/catch_test_macros.hpp>

#include <scanner/scanner.hpp>



TEST_CASE("Invalid program") {
    vector<string> input = {
        "for (5£££int4 i = 0; i < +; i-+) {",
        "   death++;",
        "} # do the thing"
    };
    
    scanner::Reset();
    scanner::Scan(input);

    REQUIRE(scanner::GetError());
};

TEST_CASE("Valid but nonsensical program") {
    vector<string> input = {
        "I AM + BAYEH\n",
        "AND 1 SPEAK FOR. (THE) TREES\n",
    };
    vector<Token> expected = {
        {IDENTIFIER, "I"}, 
        {IDENTIFIER, "AM"}, 
        {ADD, "+"}, 
        {IDENTIFIER, "BAYEH"}, 

        {IDENTIFIER, "AND"},
        {NUMBER, "1"},
        {IDENTIFIER, "SPEAK"},
        {IDENTIFIER, "FOR"},
        {DOT, "."},
        {OPEN_PARENTHESIS, "("},
        {IDENTIFIER, "THE"},
        {CLOSE_PARENTHESIS, ")"},
        {IDENTIFIER, "TREES"},
    };

    scanner::Reset();
    vector<Token> output = scanner::Scan(input);

    REQUIRE(!scanner::GetError());
    REQUIRE(output.size() == expected.size());
    for (int i = 0; i < output.size(); i++) {
        REQUIRE(expected.at(i).type == output.at(i).type);
        REQUIRE(expected.at(i).text == output.at(i).text);
    }
}

TEST_CASE("All tokens") {
    vector<string> input = {
        "# This isn't a comment, definitely not\n",
        "@,.:;[](){=} == => =< += - -= ->\n",
        "0 12935 8942758706 -4829367864 -0\n",
        "*= /= %= >= <= <-\n",
        "and bool break continue const\n",
        "else for false input if\n",
        "int4 int8 int16 int32 int64\n",
        "not or output return true while\n",
        "identifiers Yes jUs KI skjnafionefne_AWFsanus_aWSngjeke jeamoieea_P _ PPPPP i jie n\n"
    };
    vector<Token> expected = {
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
        {GREATER_OR_EQUAL, "=>"},
        {LESS_OR_EQUAL, "=<"},
        {ADD_ASSIGN, "+="},
        {SUBTRACT, "-"},
        {SUBTRACT_ASSIGN, "-="},
        {REFERENCE, "->"},

        {NUMBER, "0"},
        {NUMBER, "12935"},
        {NUMBER, "8942758706"},
        {NUMBER, "-4829367864"},
        {NUMBER, "-0"},

        {MULTIPLY_ASSIGN, "*="},
        {DIVIDE_ASSIGN, "/="},
        {MODULUS_ASSIGN, "%="},
        {GREATER_OR_EQUAL, ">="},
        {LESS_OR_EQUAL, "<="},
        {DEREFERENCE, "<-"},

        {AND, "and"},
        {BOOL, "bool"},
        {BREAK, "break"},
        {CONTINUE, "continue"},
        {CONST, "const"},

        {ELSE, "else"},
        {FOR, "for"},
        {FALSE, "false"},
        {INPUT, "input"},
        {IF, "if"},

        {INT4, "int4"},
        {INT8, "int8"},
        {INT16, "int16"},
        {INT32, "int32"},
        {INT64, "int64"},

        {NOT, "not"},
        {OR, "or"},
        {OUTPUT, "output"},
        {RETURN, "return"},
        {TRUE, "true"},
        {WHILE, "while"},

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
    };

    scanner::Reset();
    vector<Token> output = scanner::Scan(input);

    REQUIRE(!scanner::GetError());
    for (int i = 0; i < output.size(); i++) {
        CAPTURE(expected.at(i).type, output.at(i).type);
        CAPTURE(expected.at(i).text, output.at(i).text);
        REQUIRE(expected.at(i).type == output.at(i).type);
        REQUIRE(expected.at(i).text == output.at(i).text);
    }
}

TEST_CASE("Valid program") {
    vector<string> input = {
        "int64 Factorial(int32 x) {\n",
        "   int16 y = 1;\n",
        "   for (int8 i = 0; i < x; i+=1) {\n",
        "       y *= i;\n",
        "   }\n",
        "   return y;\n",
        "}\n",
        "int4 x = input;\n",
        "int4 z = -3;\n",
        "bool a = true;\n",
        "if (a == true) {\n",
        "   x /= z;\n",
        "}\n",
        "output Factorial(x);\n"
    };
    vector<Token> expected = {
        {INT64, "int64"},
        {IDENTIFIER, "Factorial"},
        {OPEN_PARENTHESIS, "("},
        {INT32, "int32"},
        {IDENTIFIER, "x"},
        {CLOSE_PARENTHESIS, ")"},
        {OPEN_BRACE, "{"},

        {INT16, "int16"},
        {IDENTIFIER, "y"},
        {ASSIGN, "="},
        {NUMBER, "1"},
        {SEMICOLON, ";"},

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

        {IDENTIFIER, "y"},
        {MULTIPLY_ASSIGN, "*="},
        {IDENTIFIER, "i"},
        {SEMICOLON, ";"},

        {CLOSE_BRACE, "}"},

        {RETURN, "return"},
        {IDENTIFIER, "y"},
        {SEMICOLON, ";"},

        {CLOSE_BRACE, "}"},

        {INT4, "int4"},
        {IDENTIFIER, "x"},
        {ASSIGN, "="},
        {INPUT, "input"},
        {SEMICOLON, ";"},

        {INT4, "int4"},
        {IDENTIFIER, "z"},
        {ASSIGN, "="},
        {NUMBER, "-3"},
        {SEMICOLON, ";"},

        {BOOL, "bool"},
        {IDENTIFIER, "a"},
        {ASSIGN, "="},
        {TRUE, "true"},
        {SEMICOLON, ";"},

        {IF, "if"},
        {OPEN_PARENTHESIS, "("},
        {IDENTIFIER, "a"},
        {EQUALS, "=="},
        {TRUE, "true"},
        {CLOSE_PARENTHESIS, ")"},
        {OPEN_BRACE, "{"},

        {IDENTIFIER, "x"},
        {DIVIDE_ASSIGN, "/="},
        {IDENTIFIER, "z"},
        {SEMICOLON, ";"},

        {CLOSE_BRACE, "}"},

        {OUTPUT, "output"},
        {IDENTIFIER, "Factorial"},
        {OPEN_PARENTHESIS, "("},
        {IDENTIFIER, "x"},
        {CLOSE_PARENTHESIS, ")"},
        {SEMICOLON, ";"}
    };

    scanner::Reset();
    vector<Token> output = scanner::Scan(input);

    REQUIRE(!scanner::GetError());
    for (int i = 0; i < output.size(); i++) {
        CAPTURE(expected.at(i).type, output.at(i).type);
        CAPTURE(expected.at(i).text, output.at(i).text);
        REQUIRE(expected.at(i).type == output.at(i).type);
        REQUIRE(expected.at(i).text == output.at(i).text);
    }
}