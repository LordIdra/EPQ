#include "grammar/nonTerminals.hpp"
#include "grammar/symbolNames.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/symbolTable.hpp"
#include "util/errors.hpp"
#include <iostream>
#include <semanticAnalyser/semanticAnalyser.hpp>



namespace semanticAnalyser {

    namespace {
        set<int> nonTerminalIntDeclarations = {
            ConstDeclaration, NonConstDeclaration_0, NextArgument, ArgumentList_0};

        set<int> nonTerminalFunctionDeclarations = {
            FunctionDeclaration};

        set<int> nonTerminalIntUses = {
            Reference, Dereference, Parameter, Variable};

        set<int> nonTerminalFunctionUses = {
            FunctionCall};

        auto AddIdentifier(symbolTable &table, const parser::TreeNode &node, const SymbolType type) {
            table.AddIdentifier(node.token.text, IdentifierSymbol{table.CurrentScopeLevel(), type});
        }

        auto UnknownIdentifier(const parser::TreeNode &node) -> void {
            errors::AddError(errors::UNKNOWN_IDENTIFIER,
                colors::AMBER + "[line " + std::to_string(node.token.line) + "]" + // todo add line number
                colors::RED + " Unkown Identifier:" + colors::CYAN + node.token.text + colors::WHITE);
        }

        auto MismatchedType(const parser::TreeNode &node, const string &type1, const string &type2) -> void {
            errors::AddError(errors::MISMATCHED_TYPE,
                colors::AMBER + "[line " + std::to_string(node.token.line) + "]" + // todo add line number
                colors::RED + " Mismatched Type: identifier " + colors::CYAN + node.token.text + 
                colors::RED + " should be " + colors::CYAN + type1 + 
                colors::RED + " but is " + colors::CYAN + type2 + 
                colors::WHITE);
        }

        // Forward declaration so the next few functions can have a circular dependency
        auto Traverse(symbolTable &table, const parser::TreeNode &node) -> void;

        auto EnterNewScopeAndTraverseChildren(symbolTable &table, const parser::TreeNode &node) -> void {
            table.EnterScope();
            for (const parser::TreeNode &child : node.children) {
                Traverse(table, child);
            }
            table.ExitScope();
        }

        auto CheckIdentifierDeclaration(symbolTable &table, const parser::TreeNode &node) -> void {
            if (nonTerminalIntDeclarations.count(node.parent->token.type) != 0) {
                AddIdentifier(table, node, TYPE_INT);
                return;
            }

            if (nonTerminalFunctionDeclarations.count(node.parent->token.type) != 0) {
                AddIdentifier(table, node, TYPE_FUNCTION);
                return;
            }
        }

        auto CheckIntType(symbolTable &table, const parser::TreeNode &node) -> void {
            const IdentifierSymbol symbol = table.LookupAllScopes(node.token.text);
            if (symbol.type == TYPE_ERROR) {
                UnknownIdentifier(node);
            } else if (symbol.type == TYPE_FUNCTION) {
                MismatchedType(node, "integer", "function");
            }
        }

        auto CheckFunctionType(symbolTable &table, const parser::TreeNode &node) -> void {
            const IdentifierSymbol symbol = table.LookupAllScopes(node.token.text);
            if (symbol.type == TYPE_ERROR) {
                UnknownIdentifier(node);
            } else if (symbol.type == TYPE_INT) {
                MismatchedType(node, "function", "integer");
            }
        }

        auto CheckIdentifierUsages(symbolTable &table, const parser::TreeNode &node) {
            // Usages
            if (nonTerminalIntUses.count(node.parent->token.type) != 0) {
                CheckIntType(table, node);
                return;
            }

            if (nonTerminalFunctionUses.count(node.parent->token.type) != 0) {
                CheckFunctionType(table, node);
                return;
            }
        }

        auto Traverse(symbolTable &table, const parser::TreeNode &node) -> void {
            if (node.token.type == N_Block) {
                EnterNewScopeAndTraverseChildren(table, node);
                return;
            }

            if (node.token.type == FunctionDeclaration) {
                EnterNewScopeAndTraverseChildren(table, node);
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

    auto Analyse(const parser::TreeNode &abstractSyntaxTree) -> void {
        symbolTable table;
        Traverse(table, abstractSyntaxTree);
    }
    
}