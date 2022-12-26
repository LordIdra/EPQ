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
#include <catch2/catch_test_macros.hpp>

#include "test_parser_expected_1.hpp"
#include "test_parser_expected_2.hpp"

#include <iostream>
#include <util/types.hpp>



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

auto TreesIdentical(const TreeNode *node1, const TreeNode *node2) -> bool {
    //std::cout << symbolNames.at(node1->token.type) << " " << symbolNames.at(node2->token.type) << "\n";
    if (node1->children.empty() && node2->children.empty()) {
        if (node1->token.type == NONE) {
            return true;
        }
        const bool contentsIdentical = (node1->token.type == node2->token.type)
            && (node1->token.text == node2->token.text);
        if (!contentsIdentical) {
            std::cout << colors::RED << "Mismatched tree node: " << 
                         colors::CYAN << node1->token.text << " " << node2->token.text << 
                         colors::WHITE << "\n";
        }
        return contentsIdentical;
    } else if (node1->children.size() == node2->children.size()) {
        bool childrenIdentical = true;
        for (int i = 0; i < node1->children.size(); i++) {
            if (!TreesIdentical(&(node1->children[i]), &(node2->children[i]))) {
                childrenIdentical = false;
            }
        }
        return childrenIdentical;
    }

    std::cout << colors::RED << "Number of children does not match " << 
                 colors::CYAN << symbolNames.at(node1->token.type) << " " << symbolNames.at(node2->token.type) <<
                 colors::WHITE << "\n";;
    return false;
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