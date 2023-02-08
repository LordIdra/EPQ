#include "parser/parser.hpp"



namespace uses {
    auto Reference(const parser::TreeNode &node) -> void;
    auto Dereference(const parser::TreeNode &node) -> void;
    auto Argument(const parser::TreeNode &node) -> void;
    auto ArgumentList_0(const parser::TreeNode &node) -> void;
    auto Variable(const parser::TreeNode &node) -> void;
    auto FunctionCall(const parser::TreeNode &node) -> void;
}