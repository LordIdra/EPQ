#include <catch2/catch_test_macros.hpp>

#include "grammar/terminals.hpp"
#include "grammar/nonTerminals.hpp"
#include "parser/sets/first.hpp"
#include "parser/sets/follow.hpp"
#include "parser/table.hpp"

#include <grammar/productions.hpp>
#include <iostream>



TEST_CASE("[4|GRM] First set") {
    // danger these tests are empty for now as a result of frequent changes to the grammar
    // danger if the FIRST section is modified, add tests back prior to doing so
    struct Test { int left; vector<set<int>> right; };
    vector<Test> tests {};
    first::ComputeFirstSet();

    for (Test t : tests) {
        CAPTURE(t.left, t.right);
        REQUIRE(first::GetFirstSet().at(t.left) == t.right);
    }
}

TEST_CASE("[4|GRM] Follow set") {
    // danger these tests are empty for now as a result of frequent changes to the grammar
    // danger if the FOLLOW section is modified, add tests back prior to doing so
    struct Test { int left; set<int> right; };
    vector<Test> tests {};
    
    first::ComputeFirstSet();
    follow::ComputeFollowSet();

    for (Test t : tests) {
        CAPTURE(t.left, t.right, follow::GetFollowSet().at(t.left));
        REQUIRE(follow::GetFollowSet().at(t.left).size() == t.right.size());
        REQUIRE(follow::GetFollowSet().at(t.left) == t.right);
    }
}

TEST_CASE("[4|GRM] Table generation") {
    // Pretty much just makes sure the table generaiton code doesn't crash the compiler
    // Parser test is enough to ensure table generation works properly
    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();
}