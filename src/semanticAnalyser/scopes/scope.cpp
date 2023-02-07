#include <semanticAnalyser/scope.hpp>

#include "semanticAnalyser/scopeManager.hpp"
#include "util/errors.hpp"
#include <iostream>
#include <unordered_map>

// IMPORTANT next thing to do is disable debug messages and replace the queue system because it's getting ridiculous


Scope::Scope() : parent(nullptr) {}
Scope::Scope(Scope* parent) : parent(parent) {}

auto Scope::EnterScope() -> Scope* {
    children.push_back(Scope(this));
    // This iterator should point to first element while tree is being constructed
    currentScope = children.begin();
    return &children.back();
}

auto Scope::ExitScope() -> Scope* {
    return parent;
}

auto Scope::AddIdentifier(const string &name, const IdentifierSymbol symbol) -> void {
    identifiers.insert(std::make_pair(name, symbol));
}

auto Scope::ContainsIdentifier(const string &identifier) -> bool {
    return identifiers.count(identifier) != 0;
}

auto Scope::GetIdentifier(const string &identifier) -> IdentifierSymbol {
    return identifiers.at(identifier);
}

auto Scope::GetIdentifiers() -> unordered_map<string, IdentifierSymbol> {
    return identifiers;
}

auto Scope::GetParent() -> Scope* {
    return parent;
}

auto Scope::GetNextChild() -> Scope* {
    Scope* temp = &(*currentScope);
    std::advance(currentScope, 1);
    return temp;
}

auto Scope::HaveAllChildrenBeenTraversed() -> bool {
    return (currentScope == children.end()) || (children.empty());
}

auto Scope::ResetTraversalPointer() -> void {
    currentScope = children.begin();
}