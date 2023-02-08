#include "grammar/nonTerminals.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/scopes/scope.hpp"
#include "semanticAnalyser/scopes/scopeManager.hpp"
#include "semanticErrors.hpp"



namespace checks {
    namespace {
        int nextFreeAddress = 1;
    }

    auto Reset() -> void {
        nextFreeAddress = 1;
    }

    auto FreeAddresses(const int count) -> void {
        nextFreeAddress -= count;
    }

    auto EvaluateTermType(const parser::TreeNode &node) -> SymbolType {
        vector<SymbolType> types;

        // Term_14 -> Value
        if (node.token.type == Value) {
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

        // Term_14 -> Datatype OPEN_PARENTHESIS Term CLOSE_PARENTHESIS
        else if (node.token.type == Datatype) {
            const auto cast = node.parent;
            
        }
        
        // Everything else
        else {
            for (const auto &child : node.children) {
                SymbolType type = EvaluateTermType(child);
                if (type != TYPE_ERROR) {
                    types.push_back(type);
                }
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

    auto Declaration_0(const parser::TreeNode &node) -> void {
        const int address = nextFreeAddress++;

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
                actualType = TYPE_ERROR;
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
                semanticErrors::MismatchedType(node, expectedType, actualType);
                return;
            }

            ScopeManager::AddIntIdentifier(
                name, IdentifierSymbol{ScopeManager::CurrentScopeLevel(), actualType, address});
        }
    }

    auto Parameter(const parser::TreeNode &node) -> void {
        const int address = nextFreeAddress++;
        const auto name = node.children.at(1).token.text;
        const auto symbolType = typeMap.at(node.parent->children.at(0).children.at(0).token.type);

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

        ScopeManager::AddFunctionIdentifier(name, 
            IdentifierSymbol{ScopeManager::CurrentScopeLevel(), TYPE_FUNCTION, address},
            FunctionSymbol{parameterTypes, returnType});
    }

    auto Assignment(const parser::TreeNode &node) -> void {
        SymbolType expectedType;
        SymbolType actualType;

        const auto variable = node.children.at(0);
        const auto assignmentOperation = node.children.at(1);

        // Variable -> Dereference
        if (variable.children.at(0).token.type == Dereference) {
            // TODO (pointers)
        }

        // Variable -> IDENTIFIER IdentifierSuffix
        else {
            const string name = variable.children.at(0).token.text;
            expectedType = ScopeManager::LookupScopes(name).type;
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
            }
        }

        // AssignmentOperation -> (Everything Else) Term
        else {
            actualType = EvaluateTermType(assignmentOperation.children.at(1));
        }

        if (actualType != expectedType) {
            semanticErrors::MismatchedType(node, expectedType, actualType);
        }
    }

    auto SimpleStatement(const parser::TreeNode &node) -> void {
        // SimpleStatement -> OUTPUT Term SEMICOLON
        if (node.children.at(0).token.type == OUTPUT) {
            // TODO (new CPU section)
        }
    }

    auto FunctionCall(const parser::TreeNode &node) -> void {
        const string name = node.children.at(1).token.text;
        const IdentifierSymbol symbol = ScopeManager::LookupScopes(name);

        if (symbol.type == TYPE_ERROR) {
            semanticErrors::UnknownIdentifier(node);
            return;
        } 

        else if (symbol.type != TYPE_FUNCTION) {
            semanticErrors::MismatchedType(node, TYPE_FUNCTION, symbol.type);
            return;
        }

        // Check return type is non-void if the function return is being treated as a value
        if (node.token.type == Value) {
            const SymbolType returnType = ScopeManager::GetFunctionSymbol(name).returnType;
            if (symbol.type != returnType) {
                semanticErrors::MismatchedType(node, returnType, symbol.type);
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
                semanticErrors::MismatchedType(node, actualArguments.at(i), expectedArguments.at(i));
                return;
            }
        }
    }

    auto ReturnContents(const parser::TreeNode &node) -> void {
        const SymbolType expectedType = ScopeManager::GetCurrentFunctionSymbol().returnType;

        // ReturnContents -> NONE
        if (node.children.at(0).token.type == NONE) {
            if (expectedType != TYPE_VOID) {
                semanticErrors::MismatchedType(node, expectedType, TYPE_VOID);
                return;
            }
        }

        // ReturnContents -> Term
        else {
            if (expectedType == TYPE_VOID) {
                semanticErrors::MismatchedType(node, TYPE_VOID, checks::EvaluateTermType(node.children.at(0)));
                return;
            }
        }

        return;
    }
}