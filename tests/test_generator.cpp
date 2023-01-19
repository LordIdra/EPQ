#include "semanticAnalyser/symbolTable.hpp"
#include "simulator/simulator.hpp"
#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <util/errors.hpp>
#include <readfile/readfile.hpp>
#include <scanner/scanner.hpp>
#include <parser/sets/first.hpp>
#include <parser/sets/follow.hpp>
#include <parser/table.hpp>
#include <parser/parser.hpp>
#include <semanticAnalyser/semanticAnalyser.hpp>
#include <generator/generator.hpp>



TEST_CASE("[7|CGE] Code Generator 1") {
    const vector<string> input = readfile::Read("../../tests/resources/generator_1.txt");
    
    errors::Reset();
    parser::Reset();
    scanner::Reset();

    const vector<Token> scannedInput = scanner::Scan(input);

    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();

    parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    SymbolTable table = semanticAnalyser::Analyse(abstractSyntaxTree);

    errors::OutputErrors();
    
    REQUIRE(errors::GetErrorCode() == errors::NONE);

    vector<string> generatedCode = generator::Generate(abstractSyntaxTree, table);

    for (string code : generatedCode) {
        std::cout << code << "\n";
    }

    simulator::Reset();
    simulator::Run(generatedCode, false);
}