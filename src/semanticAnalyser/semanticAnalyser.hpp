#pragma once

#include "parser/parser.hpp"
#include <semanticAnalyser/symbolTable.hpp>



namespace semanticAnalyser {
    auto Analyse(const parser::TreeNode &abstractSyntaxTree) -> void;
}