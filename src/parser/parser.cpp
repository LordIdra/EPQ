#include "grammar/nonTerminals.hpp"
#include "grammar/productions.hpp"
#include "grammar/symbolNames.hpp"
#include "grammar/terminals.hpp"
#include "parser/sets/first.hpp"
#include "parser/sets/follow.hpp"
#include "parser/sets/setUtil.hpp"
#include "parser/table.hpp"
#include "util/errors.hpp"
#include <iostream>
#include <parser/parser.hpp>

#include <stack>



namespace parser {
    namespace {
        stack<int> stack_;
        TreeNode root;
        TreeNode *currentNode;
        int currentLine = 1;
        int currentTokenIndex = 0;

        auto FormatStack(stack<int> stack_) -> string {
            string str;
            while (!stack_.empty()) {
                str += symbolNames.at(stack_.top()) + " ";
                stack_.pop();
            }
            return str;
        }

        auto ExpectedTerminal(const int currentTokenType) -> void {
            errors::AddError(errors::EXPECTED_TERMINAL,
                colors::AMBER + "[line " + std::to_string(currentLine) + "]" +
                colors::RED + " Expected " + colors::CYAN + symbolNames.at(stack_.top()) +
                colors::RED + " but got " + colors::CYAN + symbolNames.at(currentTokenType));
        }

        auto InvalidSymbol(const int currentTokenType) -> void {
            errors::AddError(errors::INVALID_SYNTAX,
                colors::AMBER + "[line " + std::to_string(currentLine) + "]" +
                colors::RED + " Invalid symbol '"+ colors::CYAN + symbolNames.at(currentTokenType) + colors::RED + "'");
        }

        auto UnexpectedEndOfProgram() -> void {
            errors::AddError(errors::EXPECTED_TERMINAL,
                colors::AMBER + "[line " + std::to_string(currentLine) + "]" +
                colors::RED + " Unexpected end of program ");
        }

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

        auto HandleNewline() -> void {
            currentLine++;
            currentTokenIndex++;
        }

        auto HandleTerminal(const int currentTokenType, const string &currentTokenText) -> bool {
            // Returns 'true' if an error occurred

            // Set current tree node (representing the current terminal) to have currentTokenText
            if (setUtil::IsSymbolTerminal(stack_.top())) {
                currentNode->token.text = currentTokenText;
            }

            // Backtrack up the tree until we're one node below the node whose children we have not fully traversed
            while (IsLastChild()) {
                currentNode = currentNode->parent;
            }

            currentNode = &(currentNode->parent->children.at(IndexOfCurrentNode()+1));

            // If top of stack is epsilon
            if (stack_.top() == NONE) {
                stack_.pop();
                return false;
            }

            // If top of stack does not match the current terminal
            if (stack_.top() != currentTokenType) {
                return true;
            }

            // Top of stack matches the current terminal
            stack_.pop();
            currentTokenIndex++;
            return false;
        }

        auto HandleNonTerminal(const int currentTokenType) -> bool {
            // Returns 'true' if an error occurred
            const ProductionRight tableProduction = table::GetTable().at(stack_.top()).at(currentTokenType).second;
            if (tableProduction.empty()) {
                return true;
            }

            for (const int symbol : tableProduction) {
                Token newToken = Token{symbol, "", currentLine};
                const TreeNode newNode = TreeNode{currentNode, newToken, vector<TreeNode>()};
                currentNode->children.push_back(newNode);
            }

            currentNode = &(currentNode->children.at(0));

            stack_.pop();

            for (int i = int(tableProduction.size()) - 1; i >= 0; i--) {
                stack_.push(tableProduction.at(i));
            }

            return false;
        }
    }

    auto Reset() -> void {
        while (!stack_.empty()) {
            stack_.pop();
        }
        currentLine = 1;
        currentTokenIndex = 0;
        root = TreeNode();
        currentNode = &root;
    }

    auto Parse(vector<Token> tokens) -> TreeNode {
        stack_.push(Program);
        root.token = Token{Program, ""};

        while ((!stack_.empty()) && (currentTokenIndex != tokens.size())) {
            const string currentTokenText = tokens[currentTokenIndex].text;
            const int currentTokenType = tokens[currentTokenIndex].type;

            if (currentTokenType == NEWLINE) {
                HandleNewline();
                continue;
            }

            if (setUtil::IsSymbolTerminal(stack_.top())) {
                if (HandleTerminal(currentTokenType, currentTokenText)) {
                    ExpectedTerminal(currentTokenType);
                    return root;
                }
            }

            // Top of stack is a non-terminal
            else if (HandleNonTerminal(currentTokenType)) {
                // There is no applicable rule at the corresponding slot in the table
                InvalidSymbol(currentTokenType);
                return root;
            }
        }

        if (stack_.size() != 1) {
            UnexpectedEndOfProgram();
        }

        return root;
    }
}