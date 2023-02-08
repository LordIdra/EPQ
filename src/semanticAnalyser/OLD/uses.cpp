#include "grammar/nonTerminals.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/scopes/scope.hpp"
#include "semanticAnalyser/scopes/scopeManager.hpp"
#include "semanticAnalyser/semanticErrors.hpp"

namespace uses {
    namespace {
        auto EvaluateTermType(const parser::TreeNode &node) -> SymbolType {
            vector<SymbolType> types;

            if (node.token.type != Value) {
                for (const auto &child : node.children) {
                    SymbolType type = EvaluateTermType(child);
                    if (type != TYPE_ERROR) {
                        types.push_back(type);
                    }
                }
            } else {
                const auto child = node.children.at(0);
                switch (child.token.type) {
                case Variable:
                    // Variable -> IDENTIFIER IdentifierSuffix
                    if (child.children.size() == 2) {
                        const string identifier = child.children.at(1).token.text;
                        types.push_back(ScopeManager::LookupScopes(identifier).type);
                    // Variable -> Dereference
                    } else {
                        // TODO (pointers)
                    }
                    break;

                case Literal:
                    types.push_back(TYPE_INT32);
                    break;

                case FunctionCall:
                    const string identifier = child.children.at(1).token.text;
                    types.push_back(ScopeManager::GetFunctionSymbol(identifier).returnType);
                    break;
                }
            }

            // Select type with highest precedence
            SymbolType finalType = TYPE_ERROR;
            for (const auto type : types) {
                if (type > finalType) {
                    finalType = type;
                }
            }

            return finalType;
        }
    }
    
    auto CheckIntType(const parser::TreeNode &node) -> void {
        const IdentifierSymbol symbol = ScopeManager::LookupScopes(node.token.text);
        if (symbol.type == TYPE_ERROR) {
            semanticErrors::UnknownIdentifier(node);
            return;
        } else if (symbol.type == TYPE_FUNCTION) {
            semanticErrors::MismatchedType(node, "integer", TYPE_FUNCTION);
            return;
        }
    }

    auto Reference(const parser::TreeNode &node) -> void {
        // TODO (pointers)
    }

    auto Dereference(const parser::TreeNode &node) -> void {
        // TODO (pointers)
    }

    auto Argument(const parser::TreeNode &node) -> void {
        // Argument -> NONE
        if (node.children.size() == 1) {
            return;
        }

        // get type of argument
        // get type of ident
        const auto argumentType = ;
    }

    auto ArgumentList_0(const parser::TreeNode &node) -> void {

    }

    auto Variable(const parser::TreeNode &node) -> void {

    }
}