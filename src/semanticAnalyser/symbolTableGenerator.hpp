#pragma once

#include <semanticAnalyser/symbolTable.hpp>
#include <stack>

#include <util/types.hpp>



auto SearchStack(std::stack<unordered_map<string, IdentifierSymbol>> stack, const string &name) -> IdentifierSymbol;



class SymbolTableGenerator {
private:
    std::stack<unordered_map<string, IdentifierSymbol>> stack;
    SymbolTable table;

public:
    SymbolTableGenerator() = default;
    
    auto EnterScope() -> void;
    auto ExitScope() -> void;
    auto CurrentScopeLevel() -> SymbolScope;

    auto AddIdentifier(const string &name, const IdentifierSymbol symbol) -> void;
    auto LookupAllScopes(const string &name) -> IdentifierSymbol;
    auto LookupScopes(const string &name) -> IdentifierSymbol;

    auto GetTable() -> SymbolTable;
};