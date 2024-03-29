#include "scopeTraverser.hpp"
#include "semanticAnalyser/scopes/scope.hpp"
#include "util/errors.hpp"
#include <iostream>



ScopeTraverser::ScopeTraverser()
    : root(nullptr), currentScope(nullptr) {}
    
ScopeTraverser::ScopeTraverser(Scope &root)
    : root(&root), currentScope(&root) {}

auto ScopeTraverser::Next() -> void {
    if (currentScope->HaveAllChildrenBeenTraversed()) {
        // Traverse up
        currentScope = currentScope->GetParent();

    } else {
        // Traverse to next child
        currentScope = currentScope->GetNextChild();
    }
}

auto ScopeTraverser::LocalLookup(const string &identifier) -> IdentifierSymbol {
    Scope* tempScope = currentScope;
    while (true) {
        if (tempScope->ContainsIdentifier(identifier)) {
            return tempScope->GetIdentifier(identifier);
        }
        if (tempScope->GetParent() == nullptr) {
            break;
        }
        tempScope = tempScope->GetParent();
    }
    cout << colors::CYAN << "Identifier " << colors::RED << identifier << colors::CYAN << " not in scope tree" << "\n";
    return IdentifierSymbol{SCOPE_ERROR, TYPE_ERROR, 0};
}

auto ScopeTraverser::GlobalLookup(const string &identifier) -> IdentifierSymbol {
    Scope* tempScope = root;
    tempScope->ResetTraversalPointer();
    while (true) {
        // Check if current scope contains identifiers
        if (tempScope->ContainsIdentifier(identifier)) {
            return tempScope->GetIdentifier(identifier);
        }

        // Traverse children if they have not all been traversed
        if (!tempScope->HaveAllChildrenBeenTraversed()) {
            tempScope = tempScope->GetNextChild();
            tempScope->ResetTraversalPointer();
        
        // Traverse up otherwise (if possible)
        } else if (tempScope->GetParent() != nullptr) {
            tempScope = tempScope->GetParent();

        // All nodes traversed
        } else {
            break;
        }
    }
    cout << colors::CYAN << "Identifier " << colors::RED << identifier << colors::CYAN << " not in scope tree" << "\n";
    return IdentifierSymbol{SCOPE_ERROR, TYPE_ERROR, 0};
}
