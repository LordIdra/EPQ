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

    auto LookupScopes(const string &name) -> IdentifierSymbol;
    auto LookupTopScope(const string &name) -> IdentifierSymbol;

    auto ScopesContain(const string &name) -> bool;

    auto GetFunctionSymbol(const string &name) -> FunctionSymbol;
    auto GetCurrentFunctionSymbol() -> FunctionSymbol;

    auto GetTree() -> Scope&;
};