#include "semanticAnalyser/symbolTable.hpp"
#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <unordered_map>
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

TEST_CASE("[6|SMA] Semantic Analyser valid program 1") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_pass_1.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    const SymbolTable actual = semanticAnalyser::Analyse(abstractSyntaxTree);

    SymbolTable expected;

    expected.Push();
        expected.AddIdentifier("Factorial", IdentifierSymbol{SCOPE_GLOBAL, TYPE_FUNCTION, 0});
        expected.Push();
            expected.AddIdentifier("x", IdentifierSymbol{SCOPE_PARAMETER, TYPE_INT, 1});
            expected.Push();
                expected.AddIdentifier("y", IdentifierSymbol{SCOPE_LOCAL, TYPE_INT, 2});
                expected.Push();
                    expected.AddIdentifier("i", IdentifierSymbol{SCOPE_LOCAL, TYPE_INT, 3});
                expected.Pop();
            expected.Pop();
        expected.Pop();

        expected.AddIdentifier("main", IdentifierSymbol{SCOPE_GLOBAL, TYPE_FUNCTION, 0});
        expected.Push();
            expected.Push();
                expected.AddIdentifier("x", IdentifierSymbol{SCOPE_LOCAL, TYPE_INT, 1});
                expected.AddIdentifier("z", IdentifierSymbol{SCOPE_LOCAL, TYPE_INT, 2});
                expected.AddIdentifier("a", IdentifierSymbol{SCOPE_LOCAL, TYPE_INT, 3});
                expected.Push();
                expected.Pop();
            expected.Pop();
        expected.Pop();
    expected.Pop();

    errors::OutputErrors();

    REQUIRE(errors::GetErrorCode() == errors::NONE);

    vector<StackOperation> actualStackOperations = actual.GetSavedStackOperations();
    vector<StackOperation> expectedStackOperations = expected.GetSavedStackOperations();

    vector<unordered_map<string, IdentifierSymbol>> actualTables = actual.GetSavedTables();
    vector<unordered_map<string, IdentifierSymbol>> expectedTables = expected.GetSavedTables();
    
    REQUIRE(actual.GetSavedStackOperations().size() == expected.GetSavedStackOperations().size());
    REQUIRE(actual.GetSavedTables().size() == expected.GetSavedTables().size());

    for (int i = 0; i < actualStackOperations.size(); i++) {
        REQUIRE(actualStackOperations.at(i) == expectedStackOperations.at(i));
    }

    for (int i = 0; i < actualTables.size(); i++) {
        REQUIRE(actualTables.at(i).size() == expectedTables.at(i).size());
        for (const auto &pair : expectedTables.at(i)) {
            REQUIRE(actualTables.at(i).at(pair.first).scope == expectedTables.at(i).at(pair.first).scope);
            REQUIRE(actualTables.at(i).at(pair.first).type == expectedTables.at(i).at(pair.first).type);
            REQUIRE(actualTables.at(i).at(pair.first).address == expectedTables.at(i).at(pair.first).address);
        }
    }
}

TEST_CASE("[6|SMA] Semantic Analyser valid program 2") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_pass_2.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    const SymbolTable actual = semanticAnalyser::Analyse(abstractSyntaxTree);

    SymbolTable expected;

    expected.Push();
        expected.AddIdentifier("literally_die", IdentifierSymbol{SCOPE_GLOBAL, TYPE_FUNCTION, 0});
        expected.Push();
            expected.AddIdentifier("x", IdentifierSymbol{SCOPE_PARAMETER, TYPE_INT, 1});
            expected.Push();
            expected.Pop();
        expected.Pop();

        expected.AddIdentifier("Factorial", IdentifierSymbol{SCOPE_GLOBAL, TYPE_FUNCTION, 0});
        expected.Push();
            expected.AddIdentifier("x", IdentifierSymbol{SCOPE_PARAMETER, TYPE_INT, 1});
            expected.Push();
                expected.AddIdentifier("y", IdentifierSymbol{SCOPE_LOCAL, TYPE_INT, 2});
                expected.Push();
                    expected.AddIdentifier("i", IdentifierSymbol{SCOPE_LOCAL, TYPE_INT, 3});
                expected.Pop();
            expected.Pop();
        expected.Pop();

        expected.AddIdentifier("main", IdentifierSymbol{SCOPE_GLOBAL, TYPE_FUNCTION, 0});
        expected.Push();
            expected.Push();
                expected.AddIdentifier("x", IdentifierSymbol{SCOPE_LOCAL, TYPE_INT, 1});
                expected.AddIdentifier("z", IdentifierSymbol{SCOPE_LOCAL, TYPE_INT, 2});
                expected.AddIdentifier("a", IdentifierSymbol{SCOPE_LOCAL, TYPE_INT, 3});
                expected.Push();
                    expected.AddIdentifier("u", IdentifierSymbol{SCOPE_LOCAL, TYPE_INT, 4});
                expected.Pop();
            expected.Pop();
        expected.Pop();
    expected.Pop();

    errors::OutputErrors();

    REQUIRE(errors::GetErrorCode() == errors::NONE);

    vector<StackOperation> actualStackOperations = actual.GetSavedStackOperations();
    vector<StackOperation> expectedStackOperations = expected.GetSavedStackOperations();

    vector<unordered_map<string, IdentifierSymbol>> actualTables = actual.GetSavedTables();
    vector<unordered_map<string, IdentifierSymbol>> expectedTables = expected.GetSavedTables();
    
    REQUIRE(actual.GetSavedStackOperations().size() == expected.GetSavedStackOperations().size());
    REQUIRE(actual.GetSavedTables().size() == expected.GetSavedTables().size());

    for (int i = 0; i < actualStackOperations.size(); i++) {
        REQUIRE(actualStackOperations.at(i) == expectedStackOperations.at(i));
    }

    for (int i = 0; i < actualTables.size(); i++) {
        REQUIRE(actualTables.at(i).size() == expectedTables.at(i).size());
        for (const auto &pair : expectedTables.at(i)) {
            REQUIRE(actualTables.at(i).at(pair.first).scope == expectedTables.at(i).at(pair.first).scope);
            REQUIRE(actualTables.at(i).at(pair.first).type == expectedTables.at(i).at(pair.first).type);
            REQUIRE(actualTables.at(i).at(pair.first).address == expectedTables.at(i).at(pair.first).address);
        }
    }
}
