#pragma once

#include <semanticAnalyser/semanticAnalyser.hpp>



namespace generator {
    auto Generate(parser::TreeNode &_node, const SymbolTable &_symbolTable) -> vector<string>;
}