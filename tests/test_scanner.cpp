#include "scanner/tokens.hpp"
#include <catch2/catch_test_macros.hpp>

#include <scanner/scanner.hpp>



TEST_CASE("Invalid program") {
    vector<string> input = {
        "for (5£££int4 i = 0; i < +; i-+) {",
        "   death++;",
        "} # do the thing"
    };
    scanner::Scan(input);
    REQUIRE(scanner::GetError());
};

TEST_CASE("Valid but nonsensical program") {
    vector<string> input = {
        "I AM + BAYEH",
        "AND 1 SPEAK FOR. (THE) TREES",
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
        {IDENTIFIER, "TREE"},
    };
    vector<Token> output = scanner::Scan(input);

    REQUIRE(output.size() == expected.size());
    for (int i = 0; i < output.size(); i++) {
        REQUIRE(typeid(expected.at(i)) == typeid(output.at(i).text));
    }
}

TEST_CASE("Valid program") {
    vector<string> input = {
        "int64 Factorial(int32 x) {",
        "   int16 y = 1;",
        "   for (int8 i = 0; i < x; i+=1) {",
        "       y *= i;",
        "   }",
        "   return y;",
        "}",
        "int4 x = input;",
        "int4 z = -3;",
        "bool a = true;",
        "if (a) {",
        "   x /= z;",
        "}",
        "output Factorial(x);"
    };
    vector<Token> expected = {
        {INT16, "int16"},
        {IDENTIFIER, "Factorial"},
        {OPEN_PARENTHESIS, "("},
        {INT4, "int4"},
        {IDENTIFIER, "x"},
        {CLOSE_PARENTHESIS, ")"},
        {OPEN_BRACE, "{"},

        {INT4, "int4"},
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

    vector<Token> output = scanner::Scan(input);

    REQUIRE(output.size() == expected.size());
    for (int i = 0; i < output.size(); i++) {
        REQUIRE(typeid(expected.at(i)) == typeid(output.at(i).text));
    }
}