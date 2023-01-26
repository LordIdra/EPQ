#pragma once

#include <stack>
#include <util/types.hpp>



enum SymbolScope {
    SCOPE_ERROR,
    SCOPE_LOCAL,
    SCOPE_PARAMETER,
    SCOPE_GLOBAL
};

enum SymbolType {
    TYPE_ERROR,
    TYPE_FUNCTION,
    TYPE_INT,
};

inline auto IsInt(const SymbolType type) -> bool {
    return type == TYPE_INT;
}

struct IdentifierSymbol {
    const SymbolScope scope;
    const SymbolType type;
    const int address;
};



class Scope {
private:
    Scope& parent;
    list<Scope> children;
    list<Scope>::iterator currentScope;
    unordered_map<string, IdentifierSymbol> identifiers;

public:
    Scope();
    Scope(Scope* parent);
    
    auto EnterScope() -> Scope*;
    auto ExitScope() -> Scope*;

    auto AddIdentifier(const string &name, const IdentifierSymbol symbol) -> void;
    auto ContainsIdentifier(const string &identifier) -> bool;
    auto GetIdentifier(const string &identifier) -> IdentifierSymbol;

    auto GetParent() -> Scope*;
    auto GetNextChild() -> Scope*;
    auto HaveAllChildrenBeenTraversed() -> bool;
};
