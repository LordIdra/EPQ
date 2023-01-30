#include "semanticAnalyser/scopeManager.hpp"
#include "semanticAnalyser/scope.hpp"
#include <iostream>
#include <semanticAnalyser/semanticAnalyser.hpp>
#include <stack>



namespace ScopeManager {
    namespace {
        std::stack<unordered_map<string, IdentifierSymbol>> stack;
        Scope root;
        Scope* currentScope;

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

    auto AddIdentifier(const string &name, const IdentifierSymbol symbol) -> void {
        currentScope->AddIdentifier(name, symbol);
        stack.top().insert(std::make_pair(name, symbol));
    }

    auto LookupAllScopes(const string &name) -> IdentifierSymbol {
        return SearchStack(stack, name);
    }

    auto LookupScopes(const string &name) -> IdentifierSymbol {
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

    auto GetTree() -> Scope& {
        return root;
    }
}