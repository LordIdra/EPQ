#pragma once

#include <semanticAnalyser/semanticAnalyser.hpp>



namespace generator {
    auto Reset() -> void;
    auto Generate(parser::TreeNode &_node, const Scope &_scopeTree, const bool _debugMode) -> std::pair<vector<string>, unordered_map<int, string>>;
}