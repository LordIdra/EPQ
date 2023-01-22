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



class SymbolTable {
private:
    vector<unordered_map<string, IdentifierSymbol>> savedTables;
    vector<StackOperation> savedStackOperations;
    std::stack<unordered_map<string, IdentifierSymbol>> stack;

    int savedTableIndex = 0;
    int stackOperationIndex = 0;

public:
    auto Push() -> void;
    auto Pop() -> void;
    auto AddIdentifier(const string &name, const IdentifierSymbol symbol) -> void;

    auto GetSavedTables() const -> vector<unordered_map<string, IdentifierSymbol>>;
    auto GetSavedStackOperations() const -> vector<StackOperation>;
    auto GetStack() const -> std::stack<unordered_map<string, IdentifierSymbol>>;

    auto Next() -> void;
    auto LookupAllScopes(const string &identifier) -> IdentifierSymbol;
    auto SearchAllSavedTables(const string &identifier) const -> IdentifierSymbol;
};
