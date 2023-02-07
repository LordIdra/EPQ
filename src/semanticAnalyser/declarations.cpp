#include "grammar/nonTerminals.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/scopes/scope.hpp"
#include "semanticAnalyser/scopes/scopeManager.hpp"
#include "semanticErrors.hpp"



namespace declarations {
    namespace {
        int nextFreeAddress = 1;

        auto AddIntIdentifier(const parser::TreeNode &node) -> void {
            const int address = nextFreeAddress;
            nextFreeAddress++;
            SymbolType symbolType;

            // Check that the variable has not already been declared
            if (ScopeManager::ScopesContain(node.token.text)) {
                semanticErrors::Redeclaration(node);
                return;
            }

            if (node.parent->token.type == Declaration_0) {
                // [[ Identifier -> Declaration_0 -> Declaration <- Datatype <- INT4/INT8/etc ]]
                 symbolType = typeMap.at(node.parent->parent->children.at(0).children.at(0).token.type);

            } else {
                // [[ Identifier -> Parameter <- Datatype <- INT4/INT8/etc ]]
                symbolType = typeMap.at(node.parent->children.at(0).children.at(0).token.type);
            }

            ScopeManager::AddIntIdentifier(node.token.text, 
                IdentifierSymbol{ScopeManager::CurrentScopeLevel(), symbolType, address});
        }
    }

    auto Declaration_0(const parser::TreeNode &node) -> void {
        AddIntIdentifier(node);
    }

    auto Parameter(const parser::TreeNode &node) -> void {
        AddIntIdentifier(node);
    }

    auto FunctionDeclaration(const parser::TreeNode &node) -> void {
        const int address = 0; // Determined during code generation
        vector<SymbolType> parameterTypes; // danger
        SymbolType returnType;

        // Check that the function has not already been declared
        if (ScopeManager::ScopesContain(node.token.text)) {
            semanticErrors::Redeclaration(node);
            return;
        }

        // VoidableDatatype -> VOID
        // [[ Identifier -> FunctionDeclaration <- VoidableDatatype <- VOID ]]
        if (node.parent->children.at(0).children.at(0).token.type == VOID) {
            returnType = TYPE_VOID;

        // VoidableDatatype -> Datatype
        // [[ Identifier -> FunctionDeclaration <- VoidableDatatype <- Datatype <- INT4/INT8/etc ]]
        } else {
            returnType = typeMap.at(node.parent->children.at(0).children.at(0).children.at(0).token.type);
        }

        // Get parameters
        const auto parameterList_1 = node.parent->children.at(2);
        const auto parameterList_0 = parameterList_1.children.at(1);

        // ParameterList_0 -> Parameter NextParameter
        if (parameterList_0.children.size() == 2) {
            const auto parameter = parameterList_0.children.at(0);
            const auto datatype = parameter.children.at(0);
            const auto type = datatype.children.at(0);
            parameterTypes.push_back(typeMap.at(type.token.type));

            auto nextParameter = parameterList_0.children.at(1);

            while (nextParameter.children.size() != 1) {
                const auto parameter = nextParameter.children.at(1);
                const auto datatype = parameter.children.at(0);
                const auto type = datatype.children.at(0);
                parameterTypes.push_back(typeMap.at(type.token.type));

                nextParameter = nextParameter.children.at(2);
            }
        }

        ScopeManager::AddFunctionIdentifier(node.token.text, 
            IdentifierSymbol{ScopeManager::CurrentScopeLevel(), TYPE_FUNCTION, address},
            FunctionSymbol{parameterTypes, returnType});
    }
}