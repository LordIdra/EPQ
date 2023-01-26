#include "scopeTraverser.hpp"
#include "semanticAnalyser/scope.hpp"
#include "util/errors.hpp"
#include <iostream>



ScopeTraverser::ScopeTraverser()
    : root(nullptr), currentScope(nullptr) {}
ScopeTraverser::ScopeTraverser(Scope *root)
    : root(root), currentScope(root) {
    indices.push(0);
}

auto ScopeTraverser::Next() -> void {
    if (currentScope->HasNoChildren()) {
        // Traverse up
        std::cout << "traverse up" << "\n";
        currentScope = currentScope->GetParent();
        indices.pop();

    } else {
        if (indices.top() >= currentScope->GetNumberOfChildren()) {
            // Traverse up
            std::cout << "traverse up" << "\n";
            currentScope = currentScope->GetParent();
            indices.pop();
        } else {
            // Traverse to next child
            std::cout << "traverse next" << "\n";
            currentScope = currentScope->GetChild(indices.top());
            // Replace current index with next index
            const int nextIndex = indices.top() + 1;
            indices.pop();
            indices.push(nextIndex);
            // Start traversing the new set of children
            indices.push(0);
        }
    }
}

auto ScopeTraverser::LocalLookup(const string &identifier) -> IdentifierSymbol {
    Scope* tempScope = currentScope;
    while (tempScope->GetParent() != nullptr) {
        tempScope = tempScope->GetParent();
        if (tempScope->ContainsIdentifier(identifier)) {
            return tempScope->GetIdentifier(identifier);
        }
    }
    std::cout << colors::CYAN << "Identifier " << colors::RED << identifier << colors::CYAN << " not in scope tree" << "\n";
    return IdentifierSymbol{SCOPE_ERROR, TYPE_ERROR, 0};
}
