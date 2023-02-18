#include "grammar/nonTerminals.hpp"
#include "grammar/symbolNames.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/scopes/scope.hpp"
#include "semanticAnalyser/scopes/scopeManager.hpp"
#include "semanticAnalyser/scopes/scopeTraverser.hpp"
#include "semanticAnalyser/semanticErrors.hpp"
#include "semanticAnalyser/checks.hpp"
#include "util/errors.hpp"
#include <iostream>
#include <semanticAnalyser/semanticAnalyser.hpp>
#include <stack>



namespace semanticAnalyser {

    namespace {
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
            TraverseBlockWithoutEnteringScope(node.children.at(2));
            ScopeManager::ExitScope();
        }

        auto TraverseWhile(const parser::TreeNode &node) -> void {
            ScopeManager::EnterScope();
            Traverse(node.children.at(0));
            Traverse(node.children.at(1));
            TraverseBlockWithoutEnteringScope(node.children.at(2));
            ScopeManager::ExitScope();
        }

        auto Traverse(const parser::TreeNode &node) -> void {

            switch (node.token.type) {
                case N_Block: TraverseBlock(node); return;
                case L_Block: TraverseBlock(node); return;
                case For: TraverseFor(node); return;
                case While: TraverseWhile(node); return;
                case Declaration_0: checks::Declaration_0(node); break;
                case Parameter: checks::Parameter(node); break;
                case FunctionDeclaration:
                    // FunctionDeclaration -> None
                    if (node.children.size() == 1) { return; }
                    // FunctionDeclaration -> VoidableDatatype IDENTIFIER ParameterList_1 N_Block FunctionDeclaration
                    checks::FunctionDeclaration(node);
                    TraverseFunctionDeclaration(node); 
                    return;
                case Assignment: checks::Assignment(node); break;
                case SimpleStatement: checks::SimpleStatement(node); break;
                case FunctionCall: checks::FunctionCall(node); break;
                case ReturnContents: checks::ReturnContents(node); break;
            }

            for (const parser::TreeNode &child : node.children) {
                Traverse(child);
            }
        }
    }

    auto Reset() -> void {
        checks::Reset();
        ScopeManager::Reset();
    }

    auto Analyse(const parser::TreeNode &abstractSyntaxTree) -> Scope& {
        ScopeManager::EnterScope();
        Traverse(abstractSyntaxTree);
        ScopeManager::ExitScope();
        return ScopeManager::GetTree();
    }
    
    auto FreeAddresses(const int count) -> void {
        checks::FreeAddresses(count);
    }
}