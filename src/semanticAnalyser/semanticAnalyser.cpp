#include "grammar/nonTerminals.hpp"
#include "grammar/symbolNames.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/scope.hpp"
#include "semanticAnalyser/scopeManager.hpp"
#include "util/errors.hpp"
#include <iostream>
#include <semanticAnalyser/semanticAnalyser.hpp>
#include <stack>



namespace semanticAnalyser {

    namespace {
        set<int> nonTerminalIntDeclarations = {
            Declaration_0, Parameter};
        
        set<int> nonTerminalFunctionDeclarations = {
            FunctionDeclaration};

        set<int> nonTerminalIntAndConstIntUses = {
            Reference, Dereference, Argument, ArgumentList_0, Variable};

        set<int> nonTerminalFunctionUses = {
            FunctionCall};
        
        int nextFreeAddress = 1;

        auto AddIdentifier(const parser::TreeNode &node, const SymbolType type) -> void {
            int address = 0;
            if (type == TYPE_FUNCTION) {
                address = 0; // Determined during compile time
            } else {
                address = nextFreeAddress;
                nextFreeAddress++;
            }
            ScopeManager::AddIdentifier(node.token.text, IdentifierSymbol{
                ScopeManager::CurrentScopeLevel(), type, address});
        }

        auto UnknownIdentifier(const parser::TreeNode &node) -> void {
            errors::AddError(errors::UNKNOWN_IDENTIFIER,
                colors::AMBER + "[line " + std::to_string(node.token.line) + "]" +
                colors::RED + " Unkown Identifier:" + colors::CYAN + node.token.text + colors::WHITE);
        }

        auto MismatchedType(const parser::TreeNode &node, const string &type1, const string &type2) -> void {
            errors::AddError(errors::MISMATCHED_TYPE,
                colors::AMBER + "[line " + std::to_string(node.token.line) + "]" +
                colors::RED + " Mismatched Type: identifier " + colors::CYAN + node.token.text + 
                colors::RED + " should be " + colors::CYAN + type1 + 
                colors::RED + " but is " + colors::CYAN + type2 + 
                colors::WHITE);
        }

        // Forward declaration so the next few functions can have a circular dependency
        auto Traverse(const parser::TreeNode &node) -> void;

        auto TraverseFunctionDeclaration(const parser::TreeNode &node) -> void {
            if (node.children.empty()) {
                return;
            }
            Traverse(node.children.at(0));
            Traverse(node.children.at(1));
            ScopeManager::EnterScope();
            Traverse(node.children.at(2));
            Traverse(node.children.at(3));
            ScopeManager::ExitScope();
            Traverse(node.children.at(4));
        }

        auto TraverseBlock(const parser::TreeNode &node) -> void {
            if (node.children.empty()) {
                return;
            }
            ScopeManager::EnterScope();
            Traverse(node.children.at(0));
            Traverse(node.children.at(1));
            Traverse(node.children.at(2));
            ScopeManager::ExitScope();
        }

        auto TraverseBlockWithoutEnteringScope(const parser::TreeNode &node) -> void {
            if (node.children.empty()) {
                return;
            }
            Traverse(node.children.at(0));
            Traverse(node.children.at(1));
            Traverse(node.children.at(2));
        }

        auto TraverseFor(const parser::TreeNode &node) -> void {
            ScopeManager::EnterScope();
            Traverse(node.children.at(0));
            Traverse(node.children.at(1));
            Traverse(node.children.at(2));
            Traverse(node.children.at(3));
            TraverseBlockWithoutEnteringScope(node.children.at(4));
            ScopeManager::ExitScope();
        }

        auto TraverseWhile(const parser::TreeNode &node) -> void {
            ScopeManager::EnterScope();
            Traverse(node.children.at(0));
            Traverse(node.children.at(1));
            Traverse(node.children.at(2));
            Traverse(node.children.at(3));
            TraverseBlockWithoutEnteringScope(node.children.at(4));
            ScopeManager::ExitScope();
        }

        auto CheckIdentifierDeclaration(const parser::TreeNode &node) -> void {
            if (nonTerminalIntDeclarations.count(node.parent->token.type) != 0) {
                AddIdentifier(node, TYPE_INT);
                return;
            }

            if (nonTerminalFunctionDeclarations.count(node.parent->token.type) != 0) {
                AddIdentifier(node, TYPE_FUNCTION);
                return;
            }
        }

        auto CheckIntType(const parser::TreeNode &node) -> void {
            const IdentifierSymbol symbol = ScopeManager::LookupAllScopes(node.token.text);
            if (symbol.type == TYPE_ERROR) {
                UnknownIdentifier(node);
            } else if (symbol.type == TYPE_FUNCTION) {
                MismatchedType(node, "integer", "function");
            }
        }

        auto CheckFunctionType(const parser::TreeNode &node) -> void {
            const IdentifierSymbol symbol = ScopeManager::LookupAllScopes(node.token.text);
            if (symbol.type == TYPE_ERROR) {
                UnknownIdentifier(node);
            } else if (symbol.type == TYPE_INT) {
                MismatchedType(node, "function", "integer");
            }
        }

        auto CheckIdentifierUsages(const parser::TreeNode &node) {
            // Usages
            if (nonTerminalIntAndConstIntUses.count(node.parent->token.type) != 0) {
                CheckIntType(node);
                return;
            }

            if (nonTerminalFunctionUses.count(node.parent->token.type) != 0) {
                CheckFunctionType(node);
                return;
            }
        }

        auto Traverse(const parser::TreeNode &node) -> void {
            if ((node.token.type == N_Block) || (node.token.type == L_Block)) {
                TraverseBlock(node);
                return;
            }

            if (node.token.type == FunctionDeclaration) {
                TraverseFunctionDeclaration(node);
                return;
            }

            if (node.token.type == For) {
                TraverseFor(node);
                return;
            }

            if (node.token.type == While) {
                TraverseWhile(node);
                return;
            }

            if (node.token.type == IDENTIFIER) {
                CheckIdentifierDeclaration(node);
                CheckIdentifierUsages(node);
            }

            for (const parser::TreeNode &child : node.children) {
                Traverse(child);
            }
        }
    }

    auto Reset() -> void {
        nextFreeAddress = 1;
        ScopeManager::Reset();
    }

    auto Analyse(const parser::TreeNode &abstractSyntaxTree) -> Scope& {
        ScopeManager::EnterScope();
        Traverse(abstractSyntaxTree);
        ScopeManager::ExitScope();
        return ScopeManager::GetTree();
    }
    
    auto FreeAddresses(int count) -> void {
        nextFreeAddress -= count;
    }
}