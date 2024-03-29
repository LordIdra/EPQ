#include "parser/parser.hpp"
#include "semanticAnalyser/scopes/scope.hpp"
#include "util/errors.hpp"



namespace semanticErrors {
    auto UnknownIdentifier(const parser::TreeNode &node) -> void {
        errors::AddError(errors::UNKNOWN_IDENTIFIER,
            colors::AMBER + "[line " + std::to_string(node.token.line) + "]" +
            colors::RED + " Unkown Identifier: " + colors::CYAN + node.token.text + colors::WHITE);
    }

    auto MismatchedIdentifierType(const parser::TreeNode &node, const SymbolType &expected, const SymbolType &actual) -> void {
        errors::AddError(errors::MISMATCHED_TYPE,
            colors::AMBER + "[line " + std::to_string(node.token.line) + "]" +
            colors::RED + " Mismatched Identifier Type: identifier " + colors::CYAN + node.token.text + 
            colors::RED + " should be " + colors::CYAN + typeNames.at(expected) + 
            colors::RED + " but found " + colors::CYAN + typeNames.at(actual) + 
            colors::WHITE);
    }

    auto MismatchedArgumentType(const parser::TreeNode &node, const SymbolType &expected, const SymbolType &actual) -> void {
        errors::AddError(errors::MISMATCHED_TYPE,
            colors::AMBER + "[line " + std::to_string(node.token.line) + "]" +
            colors::RED + " Mismatched Argument Type: function " + colors::CYAN + node.token.text + 
            colors::RED + " expected argument " + colors::CYAN + typeNames.at(expected) + 
            colors::RED + " but got " + colors::CYAN + typeNames.at(actual) + 
            colors::WHITE);
    }

    auto MismatchedTermType(const parser::TreeNode &node, const SymbolType &expected, const SymbolType &actual) -> void {
        errors::AddError(errors::MISMATCHED_TYPE,
            colors::AMBER + "[line " + std::to_string(node.token.line) + "]" +
            colors::RED + " Mismatched Term Type:" +
            colors::RED + " expected " + colors::CYAN + typeNames.at(expected) + 
            colors::RED + " but found " + colors::CYAN + typeNames.at(actual) + 
            colors::WHITE);
    }

    auto IncorrectNumberOfArguments(const parser::TreeNode &node, const int &expected, const int &actual) -> void {
        errors::AddError(errors::INCORRECT_NUMBER_OF_ARGUMENTS,
            colors::AMBER + "[line " + std::to_string(node.token.line) + "]" +
            colors::RED + " Incorrect number of arguments: identifier " + colors::CYAN + node.token.text + 
            colors::RED + " should have " + colors::CYAN + std::to_string(expected) + 
            colors::RED + " arguments but was provided with " + colors::CYAN + std::to_string(actual) + 
            colors::WHITE);
    }

    auto Redeclaration(const parser::TreeNode &node) -> void {
        errors::AddError(errors::REDECLARATION,
            colors::AMBER + "[line " + std::to_string(node.token.line) + "]" +
            colors::RED + " Identifier " + colors::CYAN + node.token.text + 
            colors::RED + " was redeclared" +
            colors::WHITE);
    }
}