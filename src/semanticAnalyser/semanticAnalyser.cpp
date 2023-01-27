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

        auto AddIdentifier(ScopeManager &treeGenerator, const parser::TreeNode &node, const SymbolType type) -> void {
            int address = 0;
            if (type == TYPE_FUNCTION) {
                address = 0; // Determined during compile time
            } else {
                address = nextFreeAddress;
                nextFreeAddress++;
            }
            treeGenerator.AddIdentifier(node.token.text, IdentifierSymbol{
                treeGenerator.CurrentScopeLevel(), type, address});
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
        auto Traverse(ScopeManager &treeGenerator, const parser::TreeNode &node) -> void;

        auto TraverseFunctionDeclaration(ScopeManager &treeGenerator, const parser::TreeNode &node) -> void {
            if (node.children.empty()) {
                return;
            }
            Traverse(treeGenerator, node.children.at(0));
            Traverse(treeGenerator, node.children.at(1));
            treeGenerator.EnterScope();
            Traverse(treeGenerator, node.children.at(2));
            Traverse(treeGenerator, node.children.at(3));
            treeGenerator.ExitScope();
            Traverse(treeGenerator, node.children.at(4));
        }

        auto TraverseBlock(ScopeManager &treeGenerator, const parser::TreeNode &node) -> void {
            if (node.children.empty()) {
                return;
            }
            treeGenerator.EnterScope();
            Traverse(treeGenerator, node.children.at(0));
            Traverse(treeGenerator, node.children.at(1));
            Traverse(treeGenerator, node.children.at(2));
            treeGenerator.ExitScope();
        }

        auto TraverseBlockWithoutEnteringScope(ScopeManager &treeGenerator, const parser::TreeNode &node) -> void {
            if (node.children.empty()) {
                return;
            }
            Traverse(treeGenerator, node.children.at(0));
            Traverse(treeGenerator, node.children.at(1));
            Traverse(treeGenerator, node.children.at(2));
        }

        auto TraverseFor(ScopeManager &treeGenerator, const parser::TreeNode &node) -> void {
            treeGenerator.EnterScope();
            Traverse(treeGenerator, node.children.at(0));
            Traverse(treeGenerator, node.children.at(1));
            Traverse(treeGenerator, node.children.at(2));
            Traverse(treeGenerator, node.children.at(3));
            TraverseBlockWithoutEnteringScope(treeGenerator, node.children.at(4));
            treeGenerator.ExitScope();
        }

        auto TraverseWhile(ScopeManager &treeGenerator, const parser::TreeNode &node) -> void {
            treeGenerator.EnterScope();
            Traverse(treeGenerator, node.children.at(0));
            Traverse(treeGenerator, node.children.at(1));
            Traverse(treeGenerator, node.children.at(2));
            Traverse(treeGenerator, node.children.at(3));
            TraverseBlockWithoutEnteringScope(treeGenerator, node.children.at(4));
            treeGenerator.ExitScope();
        }

        auto CheckIdentifierDeclaration(ScopeManager &treeGenerator, const parser::TreeNode &node) -> void {
            if (nonTerminalIntDeclarations.count(node.parent->token.type) != 0) {
                AddIdentifier(treeGenerator, node, TYPE_INT);
                return;
            }

            if (nonTerminalFunctionDeclarations.count(node.parent->token.type) != 0) {
                AddIdentifier(treeGenerator, node, TYPE_FUNCTION);
                return;
            }
        }

        auto CheckIntType(ScopeManager &treeGenerator, const parser::TreeNode &node) -> void {
            const IdentifierSymbol symbol = treeGenerator.LookupAllScopes(node.token.text);
            if (symbol.type == TYPE_ERROR) {
                UnknownIdentifier(node);
            } else if (symbol.type == TYPE_FUNCTION) {
                MismatchedType(node, "integer", "function");
            }
        }

        auto CheckFunctionType(ScopeManager &treeGenerator, const parser::TreeNode &node) -> void {
            const IdentifierSymbol symbol = treeGenerator.LookupAllScopes(node.token.text);
            if (symbol.type == TYPE_ERROR) {
                UnknownIdentifier(node);
            } else if (symbol.type == TYPE_INT) {
                MismatchedType(node, "function", "integer");
            }
        }

        auto CheckIdentifierUsages(ScopeManager &treeGenerator, const parser::TreeNode &node) {
            // Usages
            if (nonTerminalIntAndConstIntUses.count(node.parent->token.type) != 0) {
                CheckIntType(treeGenerator, node);
                return;
            }

            if (nonTerminalFunctionUses.count(node.parent->token.type) != 0) {
                CheckFunctionType(treeGenerator, node);
                return;
            }
        }

        auto Traverse(ScopeManager &treeGenerator, const parser::TreeNode &node) -> void {
            if ((node.token.type == N_Block) || (node.token.type == L_Block)) {
                TraverseBlock(treeGenerator, node);
                return;
            }

            if (node.token.type == FunctionDeclaration) {
                TraverseFunctionDeclaration(treeGenerator, node);
                return;
            }

            if (node.token.type == For) {
                TraverseFor(treeGenerator, node);
                return;
            }

            if (node.token.type == While) {
                TraverseWhile(treeGenerator, node);
                return;
            }

            if (node.token.type == IDENTIFIER) {
                CheckIdentifierDeclaration(treeGenerator, node);
                CheckIdentifierUsages(treeGenerator, node);
            }

            for (const parser::TreeNode &child : node.children) {
                Traverse(treeGenerator, child);
            }
        }
    }

    auto Reset() -> void {
        nextFreeAddress = 1;
    }

    auto Analyse(const parser::TreeNode &abstractSyntaxTree) -> Scope {
        ScopeManager treeGenerator;
        treeGenerator.EnterScope();
        Traverse(treeGenerator, abstractSyntaxTree);
        treeGenerator.ExitScope();
        return treeGenerator.GetTree();
    }
    
    auto FreeAddresses(int count) -> void {
        nextFreeAddress -= count;
    }
}