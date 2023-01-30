#pragma once

#include "semanticAnalyser/scope.hpp"
#include <stack>

#include <util/types.hpp>



namespace ScopeManager {
    auto Reset() -> void;
    
    auto EnterScope() -> void;
    auto ExitScope() -> void;
    auto CurrentScopeLevel() -> SymbolScope;

    auto AddIdentifier(const string &name, const IdentifierSymbol symbol) -> void;
    auto LookupAllScopes(const string &name) -> IdentifierSymbol;
    auto LookupScopes(const string &name) -> IdentifierSymbol;

    auto GetTree() -> Scope&;
};