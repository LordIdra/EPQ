#pragma once

#include "parser/parser.hpp"
#include <semanticAnalyser/scopes/scope.hpp>
#include <semanticAnalyser/scopes/scopeManager.hpp>



namespace semanticAnalyser {
    auto Reset() -> void;
    auto Analyse(const parser::TreeNode &abstractSyntaxTree) -> Scope&;
    auto FreeAddresses(int count) -> void;
}