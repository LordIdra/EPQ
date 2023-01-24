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

enum StackOperation {
    PUSH,
    POP
};

struct IdentifierSymbol {
    const SymbolScope scope;
    const SymbolType type;
    const int address;
};



class Scope {
private:
    Scope* parent;
    vector<Scope> children;
    unordered_map<string, IdentifierSymbol> identifiers;

public:
    auto EnterScope() -> void;
    auto ExitScope() -> void;
    auto AddIdentifier(const string &name, const IdentifierSymbol symbol) -> void;

    auto Next() -> void;
    auto LookupCurrentScopes(const string &identifier) -> IdentifierSymbol;
    auto LookupAllScopes(const string &identifier) const -> IdentifierSymbol;
};
