#include "grammar/nonTerminals.hpp"
#include "grammar/symbolNames.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/scope.hpp"
#include "semanticAnalyser/scopeManager.hpp"
#include "semanticAnalyser/scopeTraverser.hpp"
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

        auto Redeclaration(const parser::TreeNode &node) -> void {
            errors::AddError(errors::REDECLARATION,
                colors::AMBER + "[line " + std::to_string(node.token.line) + "]" +
                colors::RED + " Identifier " + colors::CYAN + node.token.text + 
                colors::RED + " was redeclared" +
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

        auto AddIntIdentifier(const parser::TreeNode &node) -> void {
            const int address = nextFreeAddress;
            nextFreeAddress++;
            SymbolType symbolType;

            // Check that the variable has not already been declared
            if (ScopeManager::ScopesContain(node.token.text)) {
                Redeclaration(node);
            }

            if (node.parent->token.type == Declaration_0) {
                // [[ Identifier -> Declaration_0 -> Declaration <- Datatype <- INT4/INT8/etc ]]
                 symbolType = integerTypeMap.at(node.parent->parent->children.at(0).children.at(0).token.type);

            } else {
                // [[ Identifier -> Parameter <- Datatype <- INT4/INT8/etc ]]
                symbolType = integerTypeMap.at(node.parent->children.at(0).children.at(0).token.type);
            }

            ScopeManager::AddIntIdentifier(node.token.text, 
                IdentifierSymbol{ScopeManager::CurrentScopeLevel(), symbolType, address});
        }

        auto AddFunctionIdentifier(const parser::TreeNode &node) -> void {
            const int address = 0; // Determined during code generation
            vector<SymbolType> parameterTypes;
            SymbolType returnType;

            // Check that the function has not already been declared
            if (ScopeManager::ScopesContain(node.token.text)) {
                Redeclaration(node);
            }

            // VoidableDatatype -> VOID
            // [[ Identifier -> FunctionDeclaration <- VoidableDatatype <- VOID ]]
            if (node.parent->children.at(0).children.at(0).token.type == VOID) {
                returnType = TYPE_VOID;

            // VoidableDatatype -> Datatype
            // [[ Identifier -> FunctionDeclaration <- VoidableDatatype <- Datatype <- INT4/INT8/etc ]]
            } else {
                returnType = integerTypeMap.at(node.parent->children.at(0).children.at(0).children.at(0).token.type);
            }

            ScopeManager::AddFunctionIdentifier(node.token.text, 
                IdentifierSymbol{ScopeManager::CurrentScopeLevel(), TYPE_FUNCTION, address},
                FunctionSymbol{parameterTypes, returnType});
        }

        auto CheckIntType(const parser::TreeNode &node) -> void {
            const IdentifierSymbol symbol = ScopeManager::LookupScopes(node.token.text);
            if (symbol.type == TYPE_ERROR) {
                UnknownIdentifier(node);
            } else if (symbol.type == TYPE_FUNCTION) {
                MismatchedType(node, "integer", "function");
            }
        }

        auto CheckFunctionType(const parser::TreeNode &node) -> void {
            const IdentifierSymbol symbol = ScopeManager::LookupScopes(node.token.text);

            if (symbol.type == TYPE_ERROR) {
                UnknownIdentifier(node);
            } else if (IsInt(symbol.type)) {
                MismatchedType(node, "function", symbolNames.at(symbol.type));
            }

            // Check return type is non-void if the function return is being treated as a value
            if (node.parent->parent->token.type == Value) {
                const SymbolType returnType = ScopeManager::GetFunctionSymbol(node.token.text).returnType;
                if (!IsInt(returnType)) {
                    MismatchedType(node, "non-integer", "integer");
                }
            }

            // Check that the number of arguments we're calling the function with matches up
            // TODO
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
                        MismatchedType(node, "non-void", "void");
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