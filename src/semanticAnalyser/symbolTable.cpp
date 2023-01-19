#include "semanticAnalyser/symbolTableGenerator.hpp"
#include "util/errors.hpp"
#include <iostream>
#include <semanticAnalyser/symbolTable.hpp>



auto SymbolTable::Push() -> void {
    savedTables.emplace_back(unordered_map<string, IdentifierSymbol>{});
    savedStackOperations.push_back(PUSH);
}

auto SymbolTable::Pop() -> void {
    savedStackOperations.push_back(POP);
}

auto SymbolTable::AddIdentifier(const string &name, const IdentifierSymbol symbol) -> void {
    savedTables.at(savedTables.size()-1).insert(std::make_pair(name, symbol));
}

auto SymbolTable::GetSavedTables() const -> vector<unordered_map<string, IdentifierSymbol>> {
    return savedTables;
}

auto SymbolTable::GetSavedStackOperations() const -> vector<StackOperation> {
    return savedStackOperations;
}

auto SymbolTable::GetStack() const -> std::stack<unordered_map<string, IdentifierSymbol>> {
    return stack;
}

auto SymbolTable::Next() -> void {
    std::cout << colors::BOLD_GREEN << stackOperationIndex << colors::WHITE << "\n";
    if (savedStackOperations.at(stackOperationIndex) == PUSH) {
        stack.push(savedTables.at(savedTableIndex));
        savedTableIndex++;
    } else {
        stack.pop();
    }
    stackOperationIndex++;
}

auto SymbolTable::LookupAllScopes(const string &identifier) -> IdentifierSymbol {
    return SearchStack(stack, identifier);
}