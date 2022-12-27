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



TEST_CASE("[6|SMA] Semantic Analyser valid program 1") {
    const vector<string> input = readfile::Read("../../tests/resources/parser_ast_1.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    semanticAnalyser::Analyse(abstractSyntaxTree);

    errors::OutputErrors();
    
    REQUIRE(errors::GetErrorCode() == NONE);
}