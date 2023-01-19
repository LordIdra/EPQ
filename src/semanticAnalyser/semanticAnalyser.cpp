#include "grammar/nonTerminals.hpp"
#include "grammar/symbolNames.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/symbolTable.hpp"
#include "semanticAnalyser/symbolTableGenerator.hpp"
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

        auto AddIdentifier(SymbolTableGenerator &table, const parser::TreeNode &node, const SymbolType type) -> void {
            int address = 0;
            if (type == TYPE_FUNCTION) {
                address = 0; // Determined during compile time
            } else {
                address = nextFreeAddress;
                nextFreeAddress++;
            }
            table.AddIdentifier(node.token.text, IdentifierSymbol{
                table.CurrentScopeLevel(), type, address});
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
        auto Traverse(SymbolTableGenerator &table, const parser::TreeNode &node) -> void;

        auto TraverseFunctionDeclaration(SymbolTableGenerator &table, const parser::TreeNode &node) -> void {
            if (node.children.empty()) {
                return;
            }
            Traverse(table, node.children.at(0));
            Traverse(table, node.children.at(1));
            table.EnterScope();
            Traverse(table, node.children.at(2));
            Traverse(table, node.children.at(3));
            table.ExitScope();
            Traverse(table, node.children.at(4));
        }

        auto TraverseBlock(SymbolTableGenerator &table, const parser::TreeNode &node) -> void {
            if (node.children.empty()) {
                return;
            }
            table.EnterScope();
            Traverse(table, node.children.at(0));
            Traverse(table, node.children.at(1));
            Traverse(table, node.children.at(2));
            table.ExitScope();
        }

        auto TraverseBlockWithoutEnteringScope(SymbolTableGenerator &table, const parser::TreeNode &node) -> void {
            if (node.children.empty()) {
                return;
            }
            Traverse(table, node.children.at(0));
            Traverse(table, node.children.at(1));
            Traverse(table, node.children.at(2));
        }

        auto TraverseFor(SymbolTableGenerator &table, const parser::TreeNode &node) -> void {
            table.EnterScope();
            Traverse(table, node.children.at(0));
            Traverse(table, node.children.at(1));
            Traverse(table, node.children.at(2));
            Traverse(table, node.children.at(3));
            TraverseBlockWithoutEnteringScope(table, node.children.at(4));
            table.ExitScope();
        }

        auto TraverseWhile(SymbolTableGenerator &table, const parser::TreeNode &node) -> void {
            table.EnterScope();
            Traverse(table, node.children.at(0));
            Traverse(table, node.children.at(1));
            Traverse(table, node.children.at(2));
            Traverse(table, node.children.at(3));
            TraverseBlockWithoutEnteringScope(table, node.children.at(4));
            table.ExitScope();
        }

        auto CheckIdentifierDeclaration(SymbolTableGenerator &table, const parser::TreeNode &node) -> void {
            if (nonTerminalIntDeclarations.count(node.parent->token.type) != 0) {
                AddIdentifier(table, node, TYPE_INT);
                return;
            }

            if (nonTerminalFunctionDeclarations.count(node.parent->token.type) != 0) {
                AddIdentifier(table, node, TYPE_FUNCTION);
                return;
            }
        }

        auto CheckIntType(SymbolTableGenerator &table, const parser::TreeNode &node) -> void {
            const IdentifierSymbol symbol = table.LookupAllScopes(node.token.text);
            if (symbol.type == TYPE_ERROR) {
                UnknownIdentifier(node);
            } else if (symbol.type == TYPE_FUNCTION) {
                MismatchedType(node, "integer", "function");
            }
        }

        auto CheckFunctionType(SymbolTableGenerator &table, const parser::TreeNode &node) -> void {
            const IdentifierSymbol symbol = table.LookupAllScopes(node.token.text);
            if (symbol.type == TYPE_ERROR) {
                UnknownIdentifier(node);
            } else if (symbol.type == TYPE_INT) {
                MismatchedType(node, "function", "integer");
            }
        }

        auto CheckIdentifierUsages(SymbolTableGenerator &table, const parser::TreeNode &node) {
            // Usages
            if (nonTerminalIntAndConstIntUses.count(node.parent->token.type) != 0) {
                CheckIntType(table, node);
                return;
            }

            if (nonTerminalFunctionUses.count(node.parent->token.type) != 0) {
                CheckFunctionType(table, node);
                return;
            }
        }

        auto Traverse(SymbolTableGenerator &table, const parser::TreeNode &node) -> void {
            if ((node.token.type == N_Block) || (node.token.type == L_Block)) {
                TraverseBlock(table, node);
                return;
            }

            if (node.token.type == FunctionDeclaration) {
                TraverseFunctionDeclaration(table, node);
                return;
            }

            if (node.token.type == For) {
                TraverseFor(table, node);
                return;
            }

            if (node.token.type == While) {
                TraverseWhile(table, node);
                return;
            }

            if (node.token.type == IDENTIFIER) {
                CheckIdentifierDeclaration(table, node);
                CheckIdentifierUsages(table, node);
            }

            for (const parser::TreeNode &child : node.children) {
                Traverse(table, child);
            }
        }
    }

    auto Analyse(const parser::TreeNode &abstractSyntaxTree) -> SymbolTable {
        SymbolTableGenerator tableGenerator;
        tableGenerator.EnterScope();
        Traverse(tableGenerator, abstractSyntaxTree);
        tableGenerator.ExitScope();
        return tableGenerator.GetTable();
    }
    
    auto FreeAddresses(int count) -> void {
        nextFreeAddress -= count;
    }
}