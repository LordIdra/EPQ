#include "grammar/nonTerminals.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/symbolTable.hpp"
#include <semanticAnalyser/semanticAnalyser.hpp>



namespace semanticAnalyser {

    namespace {
        set<int> nonTerminalInts = {
            Reference, Dereference, Variable, ConstDeclaration, NonConstDeclaration_0, Parameter, NextArgument, ArgumentList_0
        };

        set<int> nonTerminalFunctions = {
            FunctionSequence, FunctionCall
        };

        auto AddIdentifier(symbolTable &table, const parser::TreeNode &node, const SymbolType type) {
            table.AddIdentifier(node.token.text, IdentifierSymbol{table.CurrentScopeLevel(), type});
        }

        auto Traverse(symbolTable &table, const parser::TreeNode &node) -> void {
            if (node.token.type == N_Block) {
                table.EnterScope();
                for (const parser::TreeNode &child : node.children) {
                    Traverse(table, child);
                }
                Traverse(table, node);
                table.ExitScope();
            }

            if (node.token.type == IDENTIFIER) {
                if (nonTerminalInts.count(node.parent->token.type) != 0) {
                    AddIdentifier(table, node, TYPE_INT);
                }
                if (nonTerminalFunctions.count(node.parent->token.type) != 0) {
                    AddIdentifier(table, node, TYPE_FUNCTION);
                }
            }
        }
    }

    auto Analyse(const parser::TreeNode &abstractSyntaxTree) -> void {
        symbolTable table;
        Traverse(table, abstractSyntaxTree);
    }
    
}