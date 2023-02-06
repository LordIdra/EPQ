#pragma once

#include "grammar/terminals.hpp"
#include <stack>
#include <unordered_map>
#include <util/types.hpp>



enum SymbolScope {
    SCOPE_ERROR,
    SCOPE_LOCAL,
    SCOPE_PARAMETER,
    SCOPE_GLOBAL
};

enum SymbolType {
    TYPE_ERROR,
    TYPE_VOID,
    TYPE_FUNCTION,
    TYPE_INT4,
    TYPE_INT8,
    TYPE_INT12,
    TYPE_INT16,
    TYPE_INT32,
    TYPE_INT64,
};

const std::unordered_map<int, SymbolType> typeMap = {
    {VOID, TYPE_VOID},
    {INT4, TYPE_INT4},
    {INT8, TYPE_INT8},
    {INT12, TYPE_INT12},
    {INT16, TYPE_INT16},
    {INT32, TYPE_INT32},
    {INT64, TYPE_INT64},
};

const std::unordered_map<SymbolType, string> typeNames = {
    {TYPE_VOID, "void"},
    {TYPE_INT4, "int4"},
    {TYPE_INT8, "int8"},
    {TYPE_INT12, "int12"},
    {TYPE_INT16, "int16"},
    {TYPE_INT32, "int32"},
    {TYPE_INT64, "int64"},
};

inline auto IsInt(const SymbolType type) -> bool {
    return (type == TYPE_INT4) 
        || (type == TYPE_INT8) || (type == TYPE_INT12) || (type == TYPE_INT16) || (type == TYPE_INT32) || (type == TYPE_INT64);
}

struct IdentifierSymbol {
    const SymbolScope scope;
    const SymbolType type;
    const int address;
};

struct FunctionSymbol {
    const vector<SymbolType> parameterTypes;
    const SymbolType returnType;
};



class Scope {
private:
    Scope* parent;
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
    auto GetIdentifiers() -> unordered_map<string, IdentifierSymbol>;

    auto GetParent() -> Scope*;
    auto GetNextChild() -> Scope*;
    auto HaveAllChildrenBeenTraversed() -> bool;

    auto ResetTraversalPointer() -> void;
};
