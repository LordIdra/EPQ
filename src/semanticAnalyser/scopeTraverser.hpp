#pragma once

#include "semanticAnalyser/scope.hpp"



class ScopeTraverser {
private:
    Scope *root;
    Scope *currentScope;

    auto Lookup(Scope *scope, const string &identifier) -> IdentifierSymbol;

public:
    ScopeTraverser();
    ScopeTraverser(Scope *root);

    auto Next() -> void;
    auto LocalLookup(const string &identifier) -> IdentifierSymbol;
};