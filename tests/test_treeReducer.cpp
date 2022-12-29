#include <treeReducer/treeReducer.hpp>

#include <catch2/catch_test_macros.hpp>

#include <util/errors.hpp>
#include <readfile/readfile.hpp>
#include <scanner/scanner.hpp>
#include <parser/sets/first.hpp>
#include <parser/sets/follow.hpp>
#include <parser/table.hpp>
#include <parser/parser.hpp>
#include <semanticAnalyser/semanticAnalyser.hpp>
#include <treeReducer/treeReducer.hpp>
#include "test_parser_expected_3.hpp"
#include "treesIdentical.hpp"



TEST_CASE("[7|TRR] Tree Reducer") {
    const vector<string> input = readfile::Read("../../tests/resources/parser_ast_2.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    semanticAnalyser::Analyse(abstractSyntaxTree);

    const parser::TreeNode reducedSyntaxTree = treeReducer::Reduce(abstractSyntaxTree);
    
    REQUIRE(TreesIdentical(&reducedSyntaxTree, &expected_3));
}