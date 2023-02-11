#include "grammar/nonTerminals.hpp"
#include "grammar/productions.hpp"
#include "grammar/symbolNames.hpp"
#include "grammar/terminals.hpp"
#include "parser/sets/setUtil.hpp"
#include "parser/table.hpp"
#include "util/errors.hpp"
#include <iostream>
#include <parser/parser.hpp>

#include <stack>



namespace parser {
    namespace {
        std::stack<int> stack;
        TreeNode root;
        TreeNode *currentNode;
        int currentLine = 1;
        int currentTokenIndex = 0;

        auto IndexOfCurrentNode() -> int {
            int i = 0;
            while (currentNode != &(currentNode->parent->children.at(i))) {
                i++;
            }
            return i;
        }

        auto IsLastChild() -> bool {
            return currentNode == &(currentNode->parent->children.at(currentNode->parent->children.size()-1));
        }

        auto FormatStack(std::stack<int> stack) -> string {
            string str;
            while (!stack.empty()) {
                str += symbolNames.at(stack.top()) + " ";
                stack.pop();
            }
            return str;
        }

        auto HandleNewline() -> void {
            currentLine++;
            currentTokenIndex++;
        }

        auto HandleTerminal(const int currentTokenType, const string &currentTokenText) -> bool {
            // Returns 'true' if an error occurred

            //std::cout << colors::BOLD_GREEN << symbolNames.at(currentTokenType) << colors::WHITE << "\n";

            // Set current tree node (representing the current terminal) to have currentTokenText
            if (setUtil::IsSymbolTerminal(stack.top())) {
                currentNode->token.text = currentTokenText;
                //std::cout << colors::AMBER << symbolNames.at(currentTokenType) << " " << currentTokenText << "\n";
                //std::cout << colors::AMBER << symbolNames.at(currentNode->token.type) << " " << currentNode->token.text << "\n";
                //std::cout << "\n";
            }

            // Backtrack up the tree until we're one node below the node whose children we have not fully traversed
            while (IsLastChild()) {
                currentNode = currentNode->parent;
            }

            currentNode = &(currentNode->parent->children.at(IndexOfCurrentNode()+1));

            // If top of stack is epsilon
            if (stack.top() == NONE) {
                stack.pop();
                return false;
            }

            // If top of stack matches the current terminal
            if (stack.top() == currentTokenType) {
                stack.pop();
                currentTokenIndex++;
                return false;
            }

            // If top of stack  does not match the current terminal
            errors::AddError(errors::EXPECTED_TERMINAL,
                colors::AMBER + "[line " + std::to_string(currentLine) + "]" +
                colors::RED + " Expected Terminal: got " + colors::CYAN + symbolNames.at(currentTokenType) +
                colors::RED + " but expected " + colors::CYAN + symbolNames.at(stack.top()) +
                colors::RED + " with stack " + colors::CYAN + FormatStack(stack) + colors::WHITE);
            return true;
        }

        auto HandleNonTerminal(const int currentTokenType) -> bool {
            // Returns 'true' if an error occurred
            const ProductionRight tableProduction = table::GetTable().at(stack.top()).at(currentTokenType).second;
            if (tableProduction.empty()) {
                return true;
            }

            for (const int symbol : tableProduction) {
                Token newToken = Token{symbol, "", currentLine};
                const TreeNode newNode = TreeNode{currentNode, newToken, vector<TreeNode>()};
                currentNode->children.push_back(newNode);
            }

            currentNode = &(currentNode->children.at(0));

            stack.pop();

            for (int i = int(tableProduction.size()) - 1; i >= 0; i--) {
                stack.push(tableProduction.at(i));
            }

            return false;
        }
    }

    auto Reset() -> void {
        while (!stack.empty()) {
            stack.pop();
        }
        currentLine = 1;
        currentTokenIndex = 0;
        root = TreeNode();
        currentNode = &root;
    }

    auto Parse(vector<Token> tokens) -> TreeNode {
        stack.push(Program);
        root.token = Token{Program, ""};

        while ((!stack.empty()) && (currentTokenIndex != tokens.size())) {
            const string currentTokenText = tokens[currentTokenIndex].text;
            const int currentTokenType = tokens[currentTokenIndex].type;

            //std::cout << FormatStack(stack) << "\n";

            if (currentTokenType == NEWLINE) {
                HandleNewline();
                continue;
            }

            if (setUtil::IsSymbolTerminal(stack.top())) {
                if (HandleTerminal(currentTokenType, currentTokenText)) {
                    return root;
                }
                continue;
            }

            // Top of stack is a non-terminal
            if (!HandleNonTerminal(currentTokenType)) {
                continue;
            }

            // If there is no applicable rule at the corresponding slot in the table
            errors::AddError(errors::INVALID_SYNTAX,
                colors::AMBER + "[line " + std::to_string(currentLine) + "]" +
                colors::RED + " Invalid Syntax: invalid symbol '"+ colors::CYAN + currentTokenText +
                colors::RED + "' of type " + colors::CYAN + symbolNames.at(currentTokenType) +
                colors::RED + " with stack: " + colors::CYAN + FormatStack(stack));
            return root;
        }

        return root;
    }
}