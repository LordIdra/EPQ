#pragma once

#include "grammar/terminals.hpp"



namespace parser {
    struct TreeNode {
        TreeNode *parent;
        Token token;
        vector<TreeNode> children;
    };

    auto Reset() -> void;
    auto Parse(vector<Token> tokens) -> TreeNode;
}