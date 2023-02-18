#pragma once

#include "generator/registers.hpp"
#include "grammar/symbolNames.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/scopes/scopeTraverser.hpp"
#include "util/errors.hpp"
#include <iostream>
#include <stack>



namespace generator {
    namespace {
        parser::TreeNode *node;
        std::stack<int> registerStack;
        std::stack<string> loopStartStack;
        std::stack<string> loopEndStack;
        std::stack<string> loopAssignmentStack;
        std::stack<string> branchEndStack;
        std::stack<string> branchFinalStack;
        ScopeTraverser scopeTraverser;

        bool debugMode;
    }

    auto PushRegister(const int r) -> void {
        if (debugMode) {
            std::cout << colors::CYAN << symbolNames.at(node->token.type) << colors::AMBER << " pushed" << colors::WHITE << "\n";
        }
        if (registerStack.size() >= 9) {
            std::cout << colors::RED << "Attempt to push to full register stack" << colors::WHITE << "\n";
            return;
        }
        registerStack.push(r);
    }

    auto PopRegister() -> int {
        if (debugMode) {
            std::cout << colors::CYAN << symbolNames.at(node->token.type) << colors::AMBER << " popped" << colors::WHITE << "\n";
        }
        if (registerStack.empty()) {
            std::cout << colors::RED << "Attempt to pop off empty register stack" << colors::WHITE << "\n";
            return -1;
        }
        const int r = registerStack.top();
        registerStack.pop();
        registers::Free(r);
        return r;
    }

    auto GetChildType(const int i) -> int {
        return node->children.at(i).token.type;
    }

    auto GetChildText(const int i) -> string {
        return node->children.at(i).token.text;
    }
}