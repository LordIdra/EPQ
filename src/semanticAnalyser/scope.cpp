#include <semanticAnalyser/scope.hpp>

#include "semanticAnalyser/scopeManager.hpp"
#include "util/errors.hpp"
#include <iostream>
#include <unordered_map>



auto Scope::EnterScope() -> void {
    // IMPORTANT next thing to do is disable debug messages and replace the queue system because it's getting ridiculous
}

auto Scope::ExitScope() -> void {
}

auto Scope::AddIdentifier(const string &name, const IdentifierSymbol symbol) -> void {
}

auto Scope::Next() -> void {
    
}

auto Scope::LookupCurrentScopes(const string &identifier) -> IdentifierSymbol {
    
}

auto Scope::LookupAllScopes(const string &identifier) const -> IdentifierSymbol {
    
}