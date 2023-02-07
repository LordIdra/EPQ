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

    auto FunctionCall(const parser::TreeNode &node) -> void {
        const IdentifierSymbol symbol = ScopeManager::LookupScopes(node.token.text);

        if (symbol.type == TYPE_ERROR) {
            semanticErrors::UnknownIdentifier(node);
            return;
        } else if (IsInt(symbol.type)) {
            semanticErrors::MismatchedType(node, TYPE_FUNCTION, "non-function");
            return;
        }

        // Check return type is non-void if the function return is being treated as a value
        if (node.parent->parent->token.type == Value) {
            const SymbolType returnType = ScopeManager::GetFunctionSymbol(node.token.text).returnType;
            if (!IsInt(returnType)) {
                semanticErrors::MismatchedType(node, "non-integer", "integer");
                return;
            }
        }

        // Check that the number of arguments we're calling the function with matches up
        const auto argumentList1 = node.parent->children.at(2);
        const auto argumentList0 = argumentList1.children.at(1);

        vector<SymbolType> expectedArguments = ScopeManager::GetFunctionSymbol(node.token.text).parameterTypes;
        vector<SymbolType> actualArguments;

        // ArgumentList0 -> NONE
        if (argumentList0.children.size() == 1) {
            if (expectedArguments.empty()) {
                return;
            }
            semanticErrors::IncorrectNumberOfArguments(node, expectedArguments.size(), actualArguments.size());
            return;
        }

        actualArguments.push_back(EvaluateTermType(argumentList0.children.at(0)));

        auto argument = argumentList0.children.at(1);

        // While NOT Argument -> None
        while (argument.children.size() == 3) {
            const SymbolType actualType = EvaluateTermType(argument);
            actualArguments.push_back(actualType);
            argument = argument.children.at(2);
        }

        // Check number of arguments matches
        if (expectedArguments.size() != actualArguments.size()) {
            semanticErrors::IncorrectNumberOfArguments(node, expectedArguments.size(), actualArguments.size());
            return;
        }

        // Check argument types match
        for (int i = 0; i < expectedArguments.size(); i++) {
            if (actualArguments.at(i) != expectedArguments.at(i)) {
                semanticErrors::MismatchedType(node, "TODO", "TODO");
                return;
            }
        }
    }
}