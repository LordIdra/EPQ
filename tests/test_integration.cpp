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

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT").address).first == 5);
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
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_4").address).first == 0);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_5").address).first == 10);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_6").address).first == 13);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_7").address).first == 3);
}

TEST_CASE("[7|INT] Integration 4") {
    const auto program = Compile("4");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_AND").address).first == 5);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_OR").address).first == 13);
}

TEST_CASE("[7|INT] Integration 5") {
    const auto program = Compile("5");
    simulator::Run(program, false);

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

TEST_CASE("[7|INT] Integration 7") {
    const auto program = Compile("7");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_1").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_2").address).first == registers::FALSE);
    
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_3").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_4").address).first == registers::TRUE);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_5").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_6").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_7").address).first == registers::FALSE);
}

TEST_CASE("[7|INT] Integration 8") {
    const auto program = Compile("8");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_1").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_2").address).first == registers::FALSE);
    
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_3").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_4").address).first == registers::TRUE);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_5").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_6").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_7").address).first == registers::TRUE);
}

TEST_CASE("[7|INT] Integration 9") {
    const auto program = Compile("9");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_1").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_2").address).first == registers::TRUE);
    
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_3").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_4").address).first == registers::FALSE);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_5").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_6").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_7").address).first == registers::FALSE);
}

TEST_CASE("[7|INT] Integration 10") {
    const auto program = Compile("10");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_1").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_2").address).first == registers::TRUE);
    
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_3").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_4").address).first == registers::FALSE);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_5").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_6").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_7").address).first == registers::TRUE);
}

TEST_CASE("[7|INT] Integration 11") {
    const auto program = Compile("11");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT").address).first == 3);
}

TEST_CASE("[7|INT] Integration 12") {
    const auto program = Compile("12");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT").address).first == 15);
}

TEST_CASE("[7|INT] Integration 13") {
    const auto program = Compile("13");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT").address).first == 12);
}

TEST_CASE("[7|INT] Integration 14") {
    const auto program = Compile("14");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_1").address).first == 2);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_2").address).first == 4);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_3").address).first == 2);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_4").address).first == 14);
}

TEST_CASE("[7|INT] Integration 15") {
    const auto program = Compile("15");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT").address).first == 2);
}

TEST_CASE("[7|INT] Integration 16") {
    const auto program = Compile("16");
    simulator::Run(program, true);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_1").address).first == 0);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_2").address).first == 0);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_3").address).first == 2);
    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT_4").address).first == 1);
}

TEST_CASE("[7|INT] Integration 17") {
    const auto program = Compile("17");
    simulator::Run(program, false);

    REQUIRE(simulator::GetData(symbolTable.SearchAllSavedTables("TEST_OUTPUT").address).first == 1);
}