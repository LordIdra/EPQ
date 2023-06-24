#pragma once

#include "generator/dataValue.hpp"
#include "generator/dataValues.hpp"
#include "grammar/symbolNames.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/scopes/scopeTraverser.hpp"
#include "util/errors.hpp"
#include <iostream>
#include <stack>



namespace generator {
    namespace {
        parser::TreeNode *node;
        stack<int> dataStack;
        stack<string> loopStartStack;
        stack<string> loopEndStack;
        stack<string> loopAssignmentStack;
        stack<string> branchEndStack;
        stack<string> branchFinalStack;
        ScopeTraverser scopeTraverser;

        bool debugMode;
    }

    auto PushValue(const int r) -> void {
        if (debugMode) {
            cout << colors::CYAN << symbolNames.at(node->token.type) << colors::AMBER << " pushed" << colors::WHITE << "\n";
        }
        dataStack.push(r);
    }

    auto PopValue() -> int {
        if (debugMode) {
            cout << colors::CYAN << symbolNames.at(node->token.type) << colors::AMBER << " popped" << colors::WHITE << "\n";
        }
        if (dataStack.empty()) {
            cout << colors::RED << "Attempt to pop off empty data stack" << colors::WHITE << "\n";
            throw;
        }
        const int r = dataStack.top();
        dataStack.pop();
        return r;
    }

    auto GetChildType(const int i) -> int {
        return node->children.at(i).token.type;
    }

    auto GetChildText(const int i) -> string {
        return node->children.at(i).token.text;
    }
}