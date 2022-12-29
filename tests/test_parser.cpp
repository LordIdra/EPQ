#include <catch2/catch_test_macros.hpp>

#include "grammar/nonTerminals.hpp"
#include "grammar/symbolNames.hpp"
#include "grammar/terminals.hpp"
#include "parser/sets/first.hpp"
#include "parser/sets/follow.hpp"
#include "parser/table.hpp"
#include "readfile/readfile.hpp"
#include "scanner/scanner.hpp"
#include "parser/parser.hpp"
#include "util/errors.hpp"

#include "test_parser_expected_1.hpp"
#include "test_parser_expected_2.hpp"

#include <iostream>
#include <util/types.hpp>

#include "treesIdentical.hpp"



TEST_CASE("[5|PRS] Parser valid program 1") {
    vector<string> input = readfile::Read("../../tests/resources/parser_pass_1.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    parser::Parse(scannedInput);

    errors::OutputErrors();
    
    REQUIRE(errors::GetErrorCode() == NONE);
}

TEST_CASE("[5|PRS] Parser valid program 2") {
    vector<string> input = readfile::Read("../../tests/resources/parser_pass_2.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    parser::Parse(scannedInput);

    errors::OutputErrors();
    
    REQUIRE(errors::GetErrorCode() == NONE);
}

TEST_CASE("[5|PRS] Parser valid program 3") {
    vector<string> input = readfile::Read("../../tests/resources/parser_pass_3.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    parser::Parse(scannedInput);

    errors::OutputErrors();
    
    REQUIRE(errors::GetErrorCode() == NONE);
}

TEST_CASE("[5|PRS] Parser AST 1") {
    vector<string> input = readfile::Read("../../tests/resources/parser_ast_1.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode actual = parser::Parse(scannedInput);

    errors::OutputErrors();
    
    REQUIRE(errors::GetErrorCode() == NONE);
    REQUIRE(TreesIdentical(&actual, &expected_1));
}

TEST_CASE("[5|PRS] Parser AST 2") {
    vector<string> input = readfile::Read("../../tests/resources/parser_ast_2.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode actual = parser::Parse(scannedInput);

    errors::OutputErrors();
    
    REQUIRE(errors::GetErrorCode() == NONE);
    REQUIRE(TreesIdentical(&actual, &expected_2));
}