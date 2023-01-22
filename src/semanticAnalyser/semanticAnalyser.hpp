#pragma once

#include "parser/parser.hpp"
#include "semanticAnalyser/symbolTable.hpp"
#include <semanticAnalyser/symbolTableGenerator.hpp>



namespace semanticAnalyser {
    auto Reset() -> void;
    auto Analyse(const parser::TreeNode &abstractSyntaxTree) -> SymbolTable;
    auto FreeAddresses(int count) -> void;
}