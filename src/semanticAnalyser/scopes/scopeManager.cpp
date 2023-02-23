#include "semanticAnalyser/scopes/scopeManager.hpp"
#include "semanticAnalyser/scopes/scope.hpp"
#include <iostream>
#include <semanticAnalyser/semanticAnalyser.hpp>
#include <stack>



namespace ScopeManager {
    namespace {
        stack<unordered_map<string, IdentifierSymbol>> stack_;
        unordered_map<string, FunctionSymbol> functionSymbols;
        Scope root;
        Scope* currentScope;
        string currentFunctionName;

        auto SearchStack(stack<unordered_map<string, IdentifierSymbol>> stack_, const string &name) ->  IdentifierSymbol {
            // If there are no tables to search, the name does not exist
            if (stack_.empty()) {
                return IdentifierSymbol{SCOPE_ERROR, TYPE_ERROR, 0};
            }

            // Pop top table off the stack
            unordered_map<string, IdentifierSymbol> table = stack_.top();
            stack_.pop();

            // If the name exists in the table we just popped, return the IdentifierSymbol it maps to
            if (table.count(name) != 0) {
                return table.at(name);
            }

            // If not, continue searching the next table
            return SearchStack(stack_, name);
        }
    }

    auto Reset() -> void {
        while (!stack_.empty()) { stack_.pop(); }
        functionSymbols.clear();
        root = Scope();
        currentScope = &root;
    }

    auto EnterScope() -> void {
        currentScope = currentScope->EnterScope();
        stack_.push(unordered_map<string, IdentifierSymbol>{});
    }

    auto ExitScope() -> void {
        for (const auto &identifier : stack_.top()) {
            if (IsInt(identifier.second.type)) {
                semanticAnalyser::FreeAddresses(1);
            }
        }
        currentScope = currentScope->ExitScope();
        stack_.pop();
    }


    auto CurrentScopeLevel() -> SymbolScope {
        if (stack_.size() == 1) {
            return SCOPE_GLOBAL;
        }
        if (stack_.size() == 2) {
            return SCOPE_PARAMETER;
        }
        return SCOPE_LOCAL;
    }

    auto AddIntIdentifier(const string &name, const IdentifierSymbol symbol) -> void {
        currentScope->AddIdentifier(name, symbol);
        stack_.top().insert(make_pair(name, symbol));
    }

    auto AddFunctionIdentifier(const string &name, const IdentifierSymbol identifierSymbol, const FunctionSymbol &functionSymbol) -> void {
        currentScope->AddIdentifier(name, identifierSymbol);
        stack_.top().insert(make_pair(name, identifierSymbol));
        functionSymbols.insert(make_pair(name, functionSymbol));
        currentFunctionName = name;
    }

    auto LookupScopes(const string &name) -> IdentifierSymbol {
        return SearchStack(stack_, name);
    }

    auto LookupTopScope(const string &name) -> IdentifierSymbol {
        // If there is no table to search, the symbol obviously does not exist
        if (stack_.empty()) {
            return IdentifierSymbol{SCOPE_ERROR, TYPE_ERROR, 0};
        }

        // If the name exists in the table we just popped, return the IdentifierSymbol it maps to
        if (stack_.top().count(name) != 0) {
            return stack_.top().at(name);
        }

        // The name does not exist in the top table
        return IdentifierSymbol{SCOPE_ERROR, TYPE_ERROR, 0};
    }

    auto ScopesContain(const string &name) -> bool {
        IdentifierSymbol symbol = LookupScopes(name);
        return (symbol.type != TYPE_ERROR) || (symbol.scope != SCOPE_ERROR);
    }

    auto GetFunctionSymbol(const string &name) -> FunctionSymbol {
        if (functionSymbols.count(name) == 0) {
            return FunctionSymbol{vector<SymbolType>(), TYPE_ERROR};
        }
        return functionSymbols.at(name);
    }

    auto GetCurrentFunctionSymbol() -> FunctionSymbol {
        return functionSymbols.at(currentFunctionName);
    }

    auto GetTree() -> Scope& {
        return root;
    }
}