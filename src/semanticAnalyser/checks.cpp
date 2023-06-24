#include "grammar/nonTerminals.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/scopes/scope.hpp"
#include "semanticAnalyser/scopes/scopeManager.hpp"
#include "semanticErrors.hpp"
#include "util/errors.hpp"
#include <iostream>



namespace checks {
    namespace {
        int nextFreeAddress = 1;
        const int MAX_ADDRESS = 2048 - 48;

        auto EvaluateDatatypeType(const parser::TreeNode &node) -> SymbolType {
            return typeMap.at(node.children.at(0).token.type);
        }

        auto EvaluateVariableType(const parser::TreeNode &node) -> SymbolType {
            // Variable -> Reference
            if (node.children.at(0).token.type == Reference) {
                return TYPE_INT12;
            }

            // Variable -> Dereference
            if (node.children.at(0).token.type == Dereference) {
                return TYPE_INT4;
            }

            // Variable -> IDENTIFIER IdentifierSuffix
            const string identifier = node.children.at(0).token.text;
            SymbolType type = ScopeManager::LookupScopes(identifier).type;
            if (type == TYPE_ERROR) {
                semanticErrors::UnknownIdentifier(node.children.at(0));
            }
            return type;
        }

        auto EvaluateFunctionCallType(const parser::TreeNode &node) -> SymbolType {
            // FunctionCall -> CALL IDENTIFIER ArgumentList_1
            const string identifier = node.children.at(1).token.text;
            const SymbolType returnType = ScopeManager::GetFunctionSymbol(identifier).returnType;
            if (returnType == TYPE_ERROR) {
                semanticErrors::UnknownIdentifier(node.children.at(1));
            }
            return returnType;
        }

        auto EvaluateValueType(const parser::TreeNode &node) -> SymbolType {
            const auto child = node.children.at(0);
            switch (child.token.type) {
            
            // Value -> Variable
            case Variable:
                return EvaluateVariableType(child);

            // Value -> Literal
            case Literal:
                return TYPE_INT4;

            // Value -> FunctionCall
            case FunctionCall:
                return EvaluateFunctionCallType(child);

            default:
                return TYPE_ERROR;
            }
        }
    }

    auto Reset() -> void {
        nextFreeAddress = 1;
    }

    auto FreeAddresses(const int count) -> void {
        nextFreeAddress -= count;
    }

    auto EvaluateTermType(const parser::TreeNode &node) -> SymbolType {
        vector<SymbolType> types;
        
        // These must evaluate to a type (if they return TYPE_ERROR, something has gone wrong)
        if (node.token.type == Term_14) {
            const auto child = node.children.at(0);
            switch (child.token.type) {

            // Term_14 -> Datatype OPEN_PARENTHESIS Term CLOSE_PARENTHESIS
            case Datatype:
                types.push_back(EvaluateDatatypeType(node.children.at(0)));
                break;

            // Term_14 -> Value
            case Value:
                types.push_back(EvaluateValueType(node.children.at(0)));
                break;

            // Term_14 -> OPEN_PARENTHESIS Term CLOSE_PARENTHESIS
            case OPEN_PARENTHESIS:
                types.push_back(EvaluateTermType(node.children.at(1)));
                break;
            }
        }
        
        // Everything else (TYPE_ERROR means it hasn't evaluated to a type)
        else {
            for (const auto &child : node.children) {
                SymbolType type = EvaluateTermType(child);
                if (type != TYPE_ERROR) {
                    types.push_back(type);
                }
            }
        }

        if (types.empty()) {
            return TYPE_ERROR;
        }

        // Verify all types in the non-terminal are the same
        const SymbolType initialType = types.at(0);
        for (const auto type : types) {
            if (type != initialType) {
                semanticErrors::MismatchedTermType(node, initialType, type);
                return TYPE_ERROR;
            }
        }

        return initialType;
    }

    auto Declaration_0(const parser::TreeNode &node) -> void {
        const int address = nextFreeAddress++;

        if (address > MAX_ADDRESS) {
            errors::AddError(errors::OUT_OF_MEMORY, "Out of SRAM memory.");
        }

        // Declaration_0 -> OPEN_SQUARE_BRACKET NUMBER CLOSE_SQUARE_BRACKET IDENTIFIER
        if (node.children.at(0).token.type == OPEN_SQUARE_BRACKET) {
            // TODO (lists)
        }

        // Declaration_0 -> IDENTIFIER ASSIGN InputTerm
        else {
            const string name = node.children.at(0).token.text;
            const auto expectedType = typeMap.at(node.parent->children.at(0).children.at(0).token.type);
            SymbolType actualType;

            // InputTerm -> INPUT
            if (node.children.at(2).children.at(0).token.type == INPUT) {
                actualType = TYPE_INT4;
                // TODO (new CPU section)
            }

            // InputTerm -> Term
            else {
                actualType = EvaluateTermType(node.children.at(2).children.at(0));
            }

            if (ScopeManager::ScopesContain(name)) {
                semanticErrors::Redeclaration(node);
                return;
            }

            if (actualType != expectedType) {
                semanticErrors::MismatchedIdentifierType(node.children.at(0), expectedType, actualType);
                return;
            }

            ScopeManager::AddIntIdentifier(
                name, IdentifierSymbol{ScopeManager::CurrentScopeLevel(), actualType, address});
        }
    }

    auto Parameter(const parser::TreeNode &node) -> void {
        const int address = nextFreeAddress++;
        const auto name = node.children.at(1).token.text;
        const auto symbolType = typeMap.at(node.children.at(0).children.at(0).token.type);

        // Check that the variable has not already been declared
        if (ScopeManager::ScopesContain(name)) {
            semanticErrors::Redeclaration(node);
            return;
        }

        ScopeManager::AddIntIdentifier(name, 
            IdentifierSymbol{ScopeManager::CurrentScopeLevel(), symbolType, address});
    }

    auto FunctionDeclaration(const parser::TreeNode &node) -> void {
        if (node.children.empty()) {
            return;
        }

        const string name = node.children.at(1).token.text;
        const int address = 0; // Determined during code generation

        vector<SymbolType> parameterTypes;
        SymbolType returnType;

        // Check that the function has not already been declared
        if (ScopeManager::ScopesContain(name)) {
            semanticErrors::Redeclaration(node);
            return;
        }

        // VoidableDatatype -> VOID
        // [[ FunctionDeclaration <- VoidableDatatype <- VOID ]]
        if (node.children.at(0).children.at(0).token.type == VOID) {
            returnType = TYPE_VOID;
        }

        // VoidableDatatype -> Datatype
        // [[ FunctionDeclaration <- VoidableDatatype <- Datatype <- INT4/INT8/etc ]]
        else {
            returnType = typeMap.at(node.children.at(0).children.at(0).children.at(0).token.type);
        }

        // [[ FunctionDeclaration <- ParameterList_1 <- ParameterList_0]]
        const auto parameterList_0 = node.children.at(2).children.at(1);

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

        int parameterSize = 0;
        for (SymbolType type : parameterTypes) {
            parameterSize += typeSizes.at(type);
        }

        if (parameterSize > 7) {
            errors::AddError(errors::TOO_MANY_PARAMETERS, colors::RED + "The total size of a function's parameters cannot exceed 7 nibbles");
        }

        ScopeManager::AddFunctionIdentifier(name, 
            IdentifierSymbol{ScopeManager::CurrentScopeLevel(), TYPE_FUNCTION, address},
            FunctionSymbol{parameterTypes, returnType});
    }

    auto Assignment(const parser::TreeNode &node) -> void {
        const auto variable = node.children.at(0);
        const auto assignmentOperation = node.children.at(1);

        SymbolType expectedType = EvaluateVariableType(variable);
        SymbolType actualType;

        if (expectedType == TYPE_ERROR) {
            return;
        }

        // AssignmentOperation -> INCREMENT
        if (assignmentOperation.children.at(0).token.type == INCREMENT) {
            return;
        }

        // AssignmentOperation -> DECREMENT
        if (assignmentOperation.children.at(0).token.type == DECREMENT) {
            return;
        }

        // AssignmentOperation -> ASSIGN InputTerm
        if (assignmentOperation.children.at(0).token.type == ASSIGN) {

            const auto inputTerm = assignmentOperation.children.at(1);

            // InputTerm -> Term
            if (inputTerm.children.at(0).token.type == Term) {
                actualType = EvaluateTermType(inputTerm.children.at(0));
            }

            // InputTerm -> INPUT
            else {
                // TODO (new CPU section)
                actualType = TYPE_INT4;
            }
        }

        // AssignmentOperation -> (Everything Else) Term
        else {
            actualType = EvaluateTermType(assignmentOperation.children.at(1));
        }

        if (actualType != expectedType) {
            semanticErrors::MismatchedTermType(node, expectedType, actualType);
        }
    }

    auto SimpleStatement(const parser::TreeNode &node) -> void {
        // SimpleStatement -> OUTPUT Term SEMICOLON
        if (node.children.at(0).token.type == OUTPUT) {
            const SymbolType type = EvaluateTermType(node.children.at(1));
            // TODO (new CPU section)
        }
    }

    auto FunctionCall(const parser::TreeNode &node) -> void {
        const string name = node.children.at(1).token.text;
        const IdentifierSymbol symbol = ScopeManager::LookupScopes(name);

        if (symbol.type == TYPE_ERROR) {
            semanticErrors::UnknownIdentifier(node.children.at(1));
            return;
        }

        if (symbol.type != TYPE_FUNCTION) {
            semanticErrors::MismatchedIdentifierType(node, TYPE_FUNCTION, symbol.type);
            return;
        }

        // Check return type is non-void if the function return is being treated as a value
        if (node.token.type == Value) {
            const SymbolType returnType = ScopeManager::GetFunctionSymbol(name).returnType;
            if (symbol.type != returnType) {
                semanticErrors::MismatchedIdentifierType(node, returnType, symbol.type);
                return;
            }
        }

        // Check that the number of arguments we're calling the function with matches up
        const auto argumentList0 = node.children.at(2).children.at(1);

        vector<SymbolType> expectedArguments = ScopeManager::GetFunctionSymbol(name).parameterTypes;
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
                semanticErrors::MismatchedArgumentType(node.children.at(1), actualArguments.at(i), expectedArguments.at(i));
                return;
            }
        }
    }

    auto ReturnContents(const parser::TreeNode &node) -> void {
        const SymbolType expectedType = ScopeManager::GetCurrentFunctionSymbol().returnType;

        // ReturnContents -> NONE
        if (node.children.at(0).token.type == NONE) {
            if (expectedType != TYPE_VOID) {
                semanticErrors::MismatchedIdentifierType(node, expectedType, TYPE_VOID);
                return;
            }
        }

        // ReturnContents -> Term
        else {
            if (expectedType == TYPE_VOID) {
                semanticErrors::MismatchedTermType(node, TYPE_VOID, checks::EvaluateTermType(node.children.at(0)));
                return;
            }
        }
    }
}