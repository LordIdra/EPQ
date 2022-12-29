#include "semanticAnalyser/symbolTable.hpp"
#include <catch2/catch_test_macros.hpp>

#include <util/errors.hpp>
#include <readfile/readfile.hpp>
#include <scanner/scanner.hpp>
#include <parser/sets/first.hpp>
#include <parser/sets/follow.hpp>
#include <parser/table.hpp>
#include <parser/parser.hpp>
#include <semanticAnalyser/semanticAnalyser.hpp>



TEST_CASE("[6|SMA] Semantic Analyser invalid program 1") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_fail_1.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    semanticAnalyser::Analyse(abstractSyntaxTree);
    
    REQUIRE(errors::GetErrorCode() == errors::MISMATCHED_TYPE);
}

TEST_CASE("[6|SMA] Semantic Analyser invalid program 2") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_fail_2.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    semanticAnalyser::Analyse(abstractSyntaxTree);

    REQUIRE(errors::GetErrorCode() == errors::MISMATCHED_TYPE);
}

TEST_CASE("[6|SMA] Semantic Analyser invalid program 3") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_fail_3.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    semanticAnalyser::Analyse(abstractSyntaxTree);

    REQUIRE(errors::GetErrorCode() == errors::UNKNOWN_IDENTIFIER);
}