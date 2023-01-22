#include "generator/assembly.hpp"
#include "generator/registers.hpp"
#include "semanticAnalyser/symbolTable.hpp"
#include "simulator/simulator.hpp"
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
#include <generator/generator.hpp>



SymbolTable symbolTable;

auto Reset() -> void {
    errors::Reset();
    scanner::Reset();
    parser::Reset();
    semanticAnalyser::Reset();
    assembly::Reset();
    generator::Reset();
    simulator::Reset();
}

auto GenerateSets() -> void {
    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();
}

auto Compile(const string &file) -> std::pair<vector<string>, unordered_map<int, string>> {
    Reset();

    const vector<string> input = readfile::Read("../../tests/resources/integration/" + file + ".txt");
    const vector<Token> scannedInput = scanner::Scan(input);

    GenerateSets();

    parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    symbolTable = semanticAnalyser::Analyse(abstractSyntaxTree);

    errors::OutputErrors();
    REQUIRE(errors::GetErrorCode() == errors::NONE);

    return generator::Generate(abstractSyntaxTree, symbolTable, false);
}

TEST_CASE("[7|INT] Integration 1") {
    const auto program = Compile("1");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT").address).first == 9);
}

TEST_CASE("[7|INT] Integration 2") {
    const auto program = Compile("2");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT").address).first == 8);
}

TEST_CASE("[7|INT] Integration 3") {
    const auto program = Compile("3");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_1").address).first == 4);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_2").address).first == 10);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_3").address).first == 1);
}

TEST_CASE("[7|INT] Integration 4") {
    const auto program = Compile("4");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_AND").address).first == 5);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_OR").address).first == 13);
}

TEST_CASE("[7|INT] Integration 5") {
    const auto program = Compile("5");
    simulator::Run(program, true);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_1").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_2").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_3").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_4").address).first == registers::FALSE);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_5").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_6").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_7").address).first == registers::TRUE);
}

TEST_CASE("[7|INT] Integration 6") {
    const auto program = Compile("6");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_1").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_2").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_3").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_4").address).first == registers::TRUE);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_5").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_6").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_7").address).first == registers::FALSE);
}