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
    TYPE_INT
};



struct IdentifierSymbol {
    const SymbolScope scope;
    const SymbolType type;
};



class symbolTable {
private:
    std::stack<unordered_map<string, IdentifierSymbol>> stack;

    auto SearchStack(std::stack<unordered_map<string, IdentifierSymbol>> stack, const string &name) -> IdentifierSymbol;

public:
    auto EnterScope() -> void;
    auto ExitScope() -> void;
    auto CurrentScopeLevel() -> SymbolScope;

    auto AddIdentifier(const string &name, const IdentifierSymbol symbol) -> void;
    auto LookupAllScopes(const string &name) -> IdentifierSymbol;
    auto LookupScopes(const string &name) -> IdentifierSymbol;
};