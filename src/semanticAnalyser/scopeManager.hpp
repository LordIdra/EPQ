#pragma once

#include "semanticAnalyser/scope.hpp"
#include <stack>

#include <util/types.hpp>



auto SearchStack(std::stack<unordered_map<string, IdentifierSymbol>> stack, const string &name) -> IdentifierSymbol;



class ScopeManager {
private:
    std::stack<unordered_map<string, IdentifierSymbol>> stack;
    Scope root;
    Scope* currentScope;

public:
    ScopeManager();
    
    auto EnterScope() -> void;
    auto ExitScope() -> void;
    auto CurrentScopeLevel() -> SymbolScope;

    auto AddIdentifier(const string &name, const IdentifierSymbol symbol) -> void;
    auto LookupAllScopes(const string &name) -> IdentifierSymbol;
    auto LookupScopes(const string &name) -> IdentifierSymbol;

    auto GetTree() -> Scope;
};