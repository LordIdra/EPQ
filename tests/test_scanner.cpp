#include "scanner/tokens.hpp"
#include <catch2/catch_test_macros.hpp>

#include <scanner/scanner.hpp>



TEST_CASE("1") {
    vector<string> input = {
        "I AM + BAYEH",
        "AND 1 SPEAK FOR. (THE) TREES",
    };
    vector<Token> expected = {
        IDENTIFIER{"I"}, IDENTIFIER("AM"), ADD("+"), IDENTIFIER("BAYEH"), IDENTIFIER("AND"), NUMBER("1"), IDENTIFIER("SPEAK"), IDENTIFIER("FOR"), DOT("."), OPEN_PARENTHESIS("("), IDENTIFIER("THE"), CLOSE_PARENTHESIS(")"), IDENTIFIER("TREES")
    };
    vector<Token> output = scanner::Scan(input);
    REQUIRE(output == expected);
}