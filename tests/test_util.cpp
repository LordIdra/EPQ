#include "util/util.hpp"
#include <catch2/catch_test_macros.hpp>

#include <unordered_map>
#include <util/types.hpp>
#include <util/util.hpp>



TEST_CASE("[0|UTL] Power of integer") {
    struct Test { unsigned int x; unsigned int y; unsigned int result; };
    vector<Test> tests {
        {77756, 0, 1},
        {4, 2, 16},
        {4, 3, 64},
        {3, 3, 27},
        {5, 5, 3125}};
    for (Test t : tests) {
        CAPTURE(t.x, t.y, t.result);
        REQUIRE(Pow(t.x, t.y) == t.result);
    }
}


TEST_CASE("[0|UTL] Binary to denary") {
    struct Test { unsigned int x; unsigned int bits; string result; };
    vector<Test> tests {
        {0, 1, "0"},
        {0, 4, "0000"},
        {0, 13, "0000000000000"},
        {1, 2, "01"},
        {5, 3, "101"},
        {6, 4, "0110"},
        {34, 5, "00010"},
        {16, 4, "0000"},
        {21, 4, "0101"}};
    for (Test t : tests) {
        CAPTURE(t.x, t.bits, t.result);
        REQUIRE(DenaryToBinary(t.x, t.bits) == t.result);
    }
}
