#pragma once

#include "semanticAnalyser/scope.hpp"
#include <stack>

#include <util/types.hpp>



namespace ScopeManager {
    auto Reset() -> void;
    
    auto EnterScope() -> void;
    auto ExitScope() -> void;
    auto CurrentScopeLevel() -> SymbolScope;

    auto AddIntIdentifier(const string &name, const IdentifierSymbol symbol) -> void;
    auto AddFunctionIdentifier(const string &name, const IdentifierSymbol identifierSymbol, const FunctionSymbol functionSymbol) -> void;

    auto LookupAllScopes(const string &name) -> IdentifierSymbol;
    auto LookupScopes(const string &name) -> IdentifierSymbol;

    auto GetCurrentFunctionSymbol() -> FunctionSymbol;

    auto GetTree() -> Scope&;
};