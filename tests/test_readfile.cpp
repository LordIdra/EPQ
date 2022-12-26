#include <catch2/catch_test_macros.hpp>

#include <readfile/readfile.hpp>



TEST_CASE("[1|RFL] Invalid File") {
    vector<string> actualLines = readfile::Read("this file does not exist");
    vector<string> expectedLines = {};
    REQUIRE(expectedLines == actualLines);
}

TEST_CASE("[1|RFL] Success 1") {
    vector<string> actualLines = readfile::Read("../../tests/resources/hello.txt");
    vector<string> expectedLines = {
        "Hi mate, my name's John. I come from the land of the free.",
        "",
        "In ma country, we don't have no SLT to impose no nonsense on us. We're free men yo, and we do whatever we want.",
        "",
        "",
        "",
        "",
        "",
        "Ay"
    };
    REQUIRE(expectedLines == actualLines);
}

TEST_CASE("[1|RFL] Success 2") {
    vector<string> actualLines = readfile::Read("../../tests/resources/mr.bayeh");
    vector<string> expectedLines = {
        "Year 9s are terrifying",
        "",
        "",
        "",
        "",
        "Also screw bureaucracy"
    };
    REQUIRE(expectedLines == actualLines);
}