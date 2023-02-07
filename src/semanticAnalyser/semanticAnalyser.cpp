#include "grammar/nonTerminals.hpp"
#include "grammar/symbolNames.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/scopes/scope.hpp"
#include "semanticAnalyser/scopes/scopeManager.hpp"
#include "semanticAnalyser/scopes/scopeTraverser.hpp"
#include "util/errors.hpp"
#include <iostream>
#include <semanticAnalyser/semanticAnalyser.hpp>
#include <stack>
#include <semanticAnalyser/declarations.hpp>
#include <semanticAnalyser/uses.hpp>



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

        auto Traverse(const parser::TreeNode &node) -> void {
            if ((node.token.type == N_Block) || (node.token.type == L_Block)) {
                TraverseBlock(node);
                return;
            }

            if (node.token.type == FunctionDeclaration) {
                // FunctionDeclaration -> None
                if (node.children.size() == 1) {
                    return;
                }
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
                switch (node.parent->token.type) {
                    // Function declarations
                    case FunctionDeclaration: declarations::FunctionDeclaration(node);  break;

                    // Integer declarations
                    case Declaration_0:  declarations::Declaration_0(node);  break;
                    case Parameter:      declarations::Parameter(node)       break;

                    // Integer uses
                    case Reference:       uses::Reference(node);       break;
                    case Dereference:     uses::Dereference(node);     break;
                    case Argument:        uses::Argument(node);        break;
                    case ArgumentList_0:  uses::ArgumentList_0(node);  break;
                    case Variable:        uses::Variable(node);        break;

                    // Function uses
                    case FunctionCall:  uses::FunctionCall(node);  break;
                }

                if (nonTerminalIntDeclarations.count(node.parent->token.type) != 0) {
                    AddIntIdentifier(node);
                    return;
                }

                if (nonTerminalFunctionDeclarations.count(node.parent->token.type) != 0) {
                    AddFunctionIdentifier(node);
                    return;
                }

                if (nonTerminalIntAndConstIntUses.count(node.parent->token.type) != 0) {
                    CheckIntType(node);
                    return;
                }

                if (nonTerminalFunctionUses.count(node.parent->token.type) != 0) {
                    CheckFunctionType(node);
                    return;
                }
            }

            if (node.token.type == ReturnContents) {
                const SymbolType correctType = ScopeManager::GetCurrentFunctionSymbol().returnType;
                // ReturnContents -> NONE
                if (node.children.at(0).token.type == NONE) {
                    if (correctType != TYPE_VOID) {
                        MismatchedType(node, "TODO", TYPE_VOID);
                        return;
                    }
                
                // ReturnContents -> Term
                } else {
                    if (!IsInt(correctType)) {
                        MismatchedType(node, "non-int", "int");
                        return;
                    }
                }
                return;
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