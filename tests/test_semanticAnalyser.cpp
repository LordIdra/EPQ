#include "semanticAnalyser/scope.hpp"
#include "semanticAnalyser/scopeTraverser.hpp"
#include "semanticAnalyser/semanticAnalyser.hpp"
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



auto Expect(const IdentifierSymbol x1, const IdentifierSymbol x2) -> bool {
    return (x1.address == x2.address) && (x1.scope == x2.scope) && (x1.type == x2.type);
}



TEST_CASE("[6|SMA] Semantic Analyser invalid program 1") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_fail_1.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    semanticAnalyser::Reset();

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
    semanticAnalyser::Reset();

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
    semanticAnalyser::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    semanticAnalyser::Analyse(abstractSyntaxTree);

    REQUIRE(errors::GetErrorCode() == errors::UNKNOWN_IDENTIFIER);
}

TEST_CASE("[6|SMA] Semantic Analyser invalid program 4") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_fail_4.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    semanticAnalyser::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    semanticAnalyser::Analyse(abstractSyntaxTree);

    REQUIRE(errors::GetErrorCode() == errors::UNKNOWN_IDENTIFIER);
}

TEST_CASE("[6|SMA] Semantic Analyser invalid program 5") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_fail_5.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    semanticAnalyser::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    semanticAnalyser::Analyse(abstractSyntaxTree);

    REQUIRE(errors::GetErrorCode() == errors::MISMATCHED_TYPE);
}

TEST_CASE("[6|SMA] Semantic Analyser invalid program 6") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_fail_6.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    semanticAnalyser::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    semanticAnalyser::Analyse(abstractSyntaxTree);
    REQUIRE(errors::GetErrorCode() == errors::MISMATCHED_TYPE);
}

TEST_CASE("[6|SMA] Semantic Analyser invalid program 7") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_fail_7.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    semanticAnalyser::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    semanticAnalyser::Analyse(abstractSyntaxTree);
    REQUIRE(errors::GetErrorCode() == errors::INCORRECT_NUMBER_OF_ARGUMENTS);
}

TEST_CASE("[6|SMA] Semantic Analyser invalid program 8") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_fail_8.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    semanticAnalyser::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    semanticAnalyser::Analyse(abstractSyntaxTree);
    REQUIRE(errors::GetErrorCode() == errors::MISMATCHED_TYPE);
}

TEST_CASE("[6|SMA] Semantic Analyser invalid program 9") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_fail_9.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    semanticAnalyser::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    semanticAnalyser::Analyse(abstractSyntaxTree);
    REQUIRE(errors::GetErrorCode() == errors::MISMATCHED_TYPE);
}

TEST_CASE("[6|SMA] Semantic Analyser invalid program 10") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_fail_10.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    semanticAnalyser::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    semanticAnalyser::Analyse(abstractSyntaxTree);
    REQUIRE(errors::GetErrorCode() == errors::REDECLARATION);
}

TEST_CASE("[6|SMA] Semantic Analyser valid program 1") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_pass_1.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    semanticAnalyser::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    Scope& scope = semanticAnalyser::Analyse(abstractSyntaxTree);
    ScopeTraverser traverser = ScopeTraverser(scope);

    errors::OutputErrors();
    REQUIRE(errors::GetErrorCode() == errors::NONE);

    traverser.Next();
        REQUIRE(Expect(traverser.LocalLookup("Factorial"), IdentifierSymbol{SCOPE_GLOBAL, TYPE_FUNCTION, 0}));
        traverser.Next();
            REQUIRE(Expect(traverser.LocalLookup("x"), IdentifierSymbol{SCOPE_PARAMETER, TYPE_INT32, 1}));
            traverser.Next();
                REQUIRE(Expect(traverser.LocalLookup("y"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT16, 2}));
                traverser.Next();
                    REQUIRE(Expect(traverser.LocalLookup("i"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT32, 3}));
                traverser.Next();
            traverser.Next();
        traverser.Next();

        REQUIRE(Expect(traverser.LocalLookup("main"), IdentifierSymbol{SCOPE_GLOBAL, TYPE_FUNCTION, 0}));
        traverser.Next();
            traverser.Next();
                REQUIRE(Expect(traverser.LocalLookup("x"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT64, 1}));
                REQUIRE(Expect(traverser.LocalLookup("z"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT4, 2}));
                REQUIRE(Expect(traverser.LocalLookup("a"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT64, 3}));
                traverser.Next();
                traverser.Next();
            traverser.Next();
        traverser.Next();
    traverser.Next();

    REQUIRE(Expect(traverser.GlobalLookup("Factorial"), IdentifierSymbol{SCOPE_GLOBAL, TYPE_FUNCTION, 0}));
    REQUIRE(Expect(traverser.GlobalLookup("i"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT32, 3}));
    REQUIRE(Expect(traverser.GlobalLookup("a"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT64, 3}));
}

TEST_CASE("[6|SMA] Semantic Analyser valid program 2") {
    const vector<string> input = readfile::Read("../../tests/resources/semantic_analysis_pass_2.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();
    semanticAnalyser::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    const parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    Scope& scope = semanticAnalyser::Analyse(abstractSyntaxTree);
    ScopeTraverser traverser = ScopeTraverser(scope);

    errors::OutputErrors();
    REQUIRE(errors::GetErrorCode() == errors::NONE);

    traverser.Next();
    REQUIRE(Expect(traverser.LocalLookup("literally_die"), IdentifierSymbol{SCOPE_GLOBAL, TYPE_FUNCTION, 0}));
        traverser.Next();
            REQUIRE(Expect(traverser.LocalLookup("x"), IdentifierSymbol{SCOPE_PARAMETER, TYPE_INT4, 1}));
            traverser.Next();
            traverser.Next();
        traverser.Next();

        REQUIRE(Expect(traverser.LocalLookup("Factorial"), IdentifierSymbol{SCOPE_GLOBAL, TYPE_FUNCTION, 0}));
        traverser.Next();
            REQUIRE(Expect(traverser.LocalLookup("x"), IdentifierSymbol{SCOPE_PARAMETER, TYPE_INT32, 1}));
            traverser.Next();
                REQUIRE(Expect(traverser.LocalLookup("y"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT16, 2}));
                traverser.Next();
                    REQUIRE(Expect(traverser.LocalLookup("i"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT4, 3}));
                traverser.Next();
                REQUIRE(Expect(traverser.LocalLookup("z"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT4, 3}));
            traverser.Next();
        traverser.Next();

        REQUIRE(Expect(traverser.LocalLookup("main"), IdentifierSymbol{SCOPE_GLOBAL, TYPE_FUNCTION, 0}));
        traverser.Next();
            traverser.Next();
                REQUIRE(Expect(traverser.LocalLookup("x"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT4, 1}));
                REQUIRE(Expect(traverser.LocalLookup("z"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT4, 2}));
                REQUIRE(Expect(traverser.LocalLookup("a"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT12, 3}));
                traverser.Next();
                    REQUIRE(Expect(traverser.LocalLookup("u"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT8, 4}));
                traverser.Next();
            traverser.Next();
        traverser.Next();
    traverser.Next();

    REQUIRE(Expect(traverser.GlobalLookup("literally_die"), IdentifierSymbol{SCOPE_GLOBAL, TYPE_FUNCTION, 0}));
    REQUIRE(Expect(traverser.GlobalLookup("i"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT4, 3}));
    REQUIRE(Expect(traverser.GlobalLookup("main"), IdentifierSymbol{SCOPE_GLOBAL, TYPE_FUNCTION, 0}));
    REQUIRE(Expect(traverser.GlobalLookup("u"), IdentifierSymbol{SCOPE_LOCAL, TYPE_INT8, 4}));
}