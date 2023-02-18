#pragma once

#include <semanticAnalyser/semanticAnalyser.hpp>



namespace generator {
    auto Reset() -> void;
    auto Generate(parser::TreeNode &_node, Scope &_scopeTree, const bool _debugMode) -> void;
    auto GetProgram() -> const vector<string>&;
    auto GetComments() -> const unordered_map<int, string>&;
}