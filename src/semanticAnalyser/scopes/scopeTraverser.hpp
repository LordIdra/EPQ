#pragma once

#include "semanticAnalyser/scopes/scope.hpp"



class ScopeTraverser {
private:
    Scope *root;
    Scope *currentScope;

public:
    ScopeTraverser();
    ScopeTraverser(Scope &root);

    auto Next() -> void;
    auto LocalLookup(const string &identifier) -> IdentifierSymbol;
    auto GlobalLookup(const string &identifier) -> IdentifierSymbol;
};