#pragma once

#include "parser/parser.hpp"
#include "semanticAnalyser/scopes/scope.hpp"



namespace checks {
    auto Reset() -> void;
    auto FreeAddresses(const int count) -> void;
    auto EvaluateTermType(const parser::TreeNode &node) -> SymbolType;

    auto Declaration_0(const parser::TreeNode &node) -> void;
    auto Parameter(const parser::TreeNode &node) -> void;
    auto FunctionDeclaration(const parser::TreeNode &node) -> void;
    auto SimpleStatement(const parser::TreeNode &node) -> void;
    auto FunctionCall(const parser::TreeNode &node) -> void;
    auto ReturnContents(const parser::TreeNode &node) -> void;

    auto Assignment(const parser::TreeNode &node) -> void;
}