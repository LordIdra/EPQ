#include "generator/assembly.hpp"
#include "generator/generator.hpp"
#include "generator/memory.hpp"
#include "generator/registers.hpp"
#include "parser/parser.hpp"
#include "parser/sets/first.hpp"
#include "parser/table.hpp"
#include "readfile/readfile.hpp"
#include "scanner/scanner.hpp"
#include "semanticAnalyser/scopes/scopeTraverser.hpp"
#include "semanticAnalyser/semanticAnalyser.hpp"
#include "simulator/simulator.hpp"
#include "util/errors.hpp"
#include <iostream>

#include <parser/sets/first.hpp>
#include <parser/sets/follow.hpp>
#include <unordered_map>
#include <utility>



std::pair<ScopeTraverser, std::pair<vector<string>, unordered_map<int, string>>> compileError = 
    std::make_pair(ScopeTraverser(), std::make_pair(vector<string>(), unordered_map<int, string>()));



auto Reset() -> void {
    errors::Reset();
    scanner::Reset();
    parser::Reset();
    semanticAnalyser::Reset();
    assembly::Reset();
    memory::Reset();
    registers::Reset();
    generator::Reset();
    simulator::Reset();
}

auto GenerateSets() -> void {
    first::ComputeFirstSet();
    follow::ComputeFollowSet();
    table::GenerateTable();
}

auto CheckErrors() -> bool {
    if (errors::GetErrorCode() != errors::NONE) {
        errors::OutputErrors();
        return true;
    }
    return false;
}

auto Compile(const string &file) -> std::pair<ScopeTraverser, std::pair<vector<string>, unordered_map<int, string>>> {
    Reset();

    const vector<string> input = readfile::Read(file + ".txt");
    if (CheckErrors()) { return compileError; }

    const vector<Token> scannedInput = scanner::Scan(input);
    if (CheckErrors()) { return compileError; }

    GenerateSets();

    parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
    if (CheckErrors()) { return compileError; }

    Scope& scope = semanticAnalyser::Analyse(abstractSyntaxTree);
    if (CheckErrors()) { return compileError; }

    ScopeTraverser traverser = ScopeTraverser(scope);
    auto output = generator::Generate(abstractSyntaxTree, scope, false);
    if (CheckErrors()) { return compileError; }

    return std::make_pair<>(traverser, output);
}