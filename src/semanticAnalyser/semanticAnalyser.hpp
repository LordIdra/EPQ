#pragma once

#include "parser/parser.hpp"
#include <semanticAnalyser/scope.hpp>
#include <semanticAnalyser/scopeManager.hpp>



namespace semanticAnalyser {
    auto Reset() -> void;
    auto Analyse(const parser::TreeNode &abstractSyntaxTree) -> Scope;
    auto FreeAddresses(int count) -> void;
}