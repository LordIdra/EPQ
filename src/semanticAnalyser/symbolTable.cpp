#include <iostream>
#include <semanticAnalyser/symbolTable.hpp>



auto symbolTable::SearchStack(std::stack<unordered_map<string, IdentifierSymbol>> stack, const string &name) -> IdentifierSymbol {
    // If there are no tables to search, the name does not exist
    if (stack.empty()) {
        return IdentifierSymbol{SCOPE_ERROR, TYPE_ERROR};
    }

    // Pop top table off the stack
    unordered_map<string, IdentifierSymbol> table = stack.top();
    stack.pop();
    
    // If the name exists in the table we just popped, return the IdentifierSymbol it maps to
    if (table.count(name) != 0) {
        return table.at(name);
    }

    // If not, continue searching the next table
    return SearchStack(stack, name);
}

symbolTable::symbolTable() {
    EnterScope();
}

auto symbolTable::EnterScope() -> void {
    stack.push(unordered_map<string, IdentifierSymbol>{});
}

auto symbolTable::ExitScope() -> void {
    stack.pop();
}


auto symbolTable::CurrentScopeLevel() -> SymbolScope {
    if (stack.size() == 1) {
        return SCOPE_GLOBAL;
    }
    if (stack.size() == 2) {
        return SCOPE_PARAMETER;
    }
    return SCOPE_LOCAL;
}

auto symbolTable::AddIdentifier(const string &name, const IdentifierSymbol symbol) -> void {
    stack.top().insert(std::make_pair(name, symbol));
}

auto symbolTable::LookupAllScopes(const string &name) -> IdentifierSymbol {
    return SearchStack(stack, name);
}

auto symbolTable::LookupScopes(const string &name) -> IdentifierSymbol {
    // If there is no table to search, the symbol obviously does not exist
    if (stack.empty()) {
        return IdentifierSymbol{SCOPE_ERROR, TYPE_ERROR};
    }

    // If the name exists in the table we just popped, return the IdentifierSymbol it maps to
    if (stack.top().count(name) != 0) {
        return stack.top().at(name);
    }

    // The name does not exist in the top table
    return IdentifierSymbol{SCOPE_ERROR, TYPE_ERROR};
}
