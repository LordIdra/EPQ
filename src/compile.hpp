#pragma once

#include "semanticAnalyser/scopes/scopeTraverser.hpp"



namespace compile {
    auto Compile(const string &file) -> void;
    auto GetTraverser() -> ScopeTraverser&;
    auto GetProgram() -> const vector<string>&;
    auto GetComments() -> const unordered_map<int, string>&;
}