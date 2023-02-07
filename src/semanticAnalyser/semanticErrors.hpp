#include "parser/parser.hpp"
#include "semanticAnalyser/scopes/scope.hpp"



namespace semanticErrors {
    auto UnknownIdentifier(const parser::TreeNode &node) -> void;
    auto MismatchedType(const parser::TreeNode &node, const SymbolType &expected, const SymbolType &actual) -> void;
    auto IncorrectNumberOfArguments(const parser::TreeNode &node, const int &expected, const int &actual) -> void;
    auto Redeclaration(const parser::TreeNode &node) -> void;
}