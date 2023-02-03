#include "semanticAnalyser/scopeManager.hpp"
#include "semanticAnalyser/scope.hpp"
#include <iostream>
#include <semanticAnalyser/semanticAnalyser.hpp>
#include <stack>



namespace ScopeManager {
    namespace {
        std::stack<unordered_map<string, IdentifierSymbol>> stack;
        unordered_map<string, FunctionSymbol> functionSymbols;
        Scope root;
        Scope* currentScope;
        string currentFunctionName;

        auto SearchStack(std::stack<unordered_map<string, IdentifierSymbol>> stack, const string &name) ->  IdentifierSymbol {
            // If there are no tables to search, the name does not exist
            if (stack.empty()) {
                return IdentifierSymbol{SCOPE_ERROR, TYPE_ERROR, 0};
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
    }

    auto Reset() -> void {
        while (!stack.empty()) {
            stack.pop();
        }
        functionSymbols.clear();
        root = Scope();
        currentScope = &root;
    }

    auto EnterScope() -> void {
        currentScope = currentScope->EnterScope();
        stack.push(unordered_map<string, IdentifierSymbol>{});
    }

    auto ExitScope() -> void {
        for (const auto &identifier : stack.top()) {
            if (IsInt(identifier.second.type)) {
                semanticAnalyser::FreeAddresses(1);
            }
        }
        currentScope = currentScope->ExitScope();
        stack.pop();
    }


    auto CurrentScopeLevel() -> SymbolScope {
        if (stack.size() == 1) {
            return SCOPE_GLOBAL;
        }
        if (stack.size() == 2) {
            return SCOPE_PARAMETER;
        }
        return SCOPE_LOCAL;
    }

    auto AddIntIdentifier(const string &name, const IdentifierSymbol symbol) -> void {
        currentScope->AddIdentifier(name, symbol);
        stack.top().insert(std::make_pair(name, symbol));
    }

    auto AddFunctionIdentifier(const string &name, const IdentifierSymbol identifierSymbol, const FunctionSymbol functionSymbol) -> void {
        currentScope->AddIdentifier(name, identifierSymbol);
        stack.top().insert(std::make_pair(name, identifierSymbol));
        functionSymbols.insert(std::make_pair(name, functionSymbol));
        currentFunctionName = name;
    }

    auto LookupScopes(const string &name) -> IdentifierSymbol {
        return SearchStack(stack, name);
    }

    auto LookupTopScope(const string &name) -> IdentifierSymbol {
        // If there is no table to search, the symbol obviously does not exist
        if (stack.empty()) {
            return IdentifierSymbol{SCOPE_ERROR, TYPE_ERROR, 0};
        }

        // If the name exists in the table we just popped, return the IdentifierSymbol it maps to
        if (stack.top().count(name) != 0) {
            return stack.top().at(name);
        }

        // The name does not exist in the top table
        return IdentifierSymbol{SCOPE_ERROR, TYPE_ERROR, 0};
    }

    auto ScopesContain(const string &name) -> bool {
        IdentifierSymbol symbol = LookupScopes(name);
        return (symbol.type != TYPE_ERROR) || (symbol.scope != SCOPE_ERROR);
    }

    auto GetFunctionSymbol(const string &name) -> FunctionSymbol {
        return functionSymbols.at(name);
    }

    auto GetCurrentFunctionSymbol() -> FunctionSymbol {
        return functionSymbols.at(currentFunctionName);
    }

    auto GetTree() -> Scope& {
        return root;
    }
}