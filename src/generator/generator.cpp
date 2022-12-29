#include "generator/memory.hpp"
#include "generator/programs.hpp"
#include "generator/registers.hpp"
#include "grammar/nonTerminals.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"
#include <generator/generator.hpp>

#include <generator/assembly.hpp>
#include <stack>
#include <util/types.hpp>



namespace generator {

    namespace {
        parser::TreeNode node;
        std::stack<int> registerStack;

        auto PopRegister() -> int {
            const int r = registerStack.top();
            registerStack.pop();
            registers::Free(r);
            return r;
        }

        auto PushRegister() -> int {
            const int r = registers::Allocate();
            registerStack.push(r);
            return r;
        }

        namespace generate {
            auto Term_AND() -> void {
                // Term_AND -> NONE
                if (node.children.size() == 1) { return; }

                // Term_AND -> AND Term_1 Term_AND
                const int r1 = PopRegister();
                const int r2 = PopRegister();
                const int r3 = PushRegister();
                assembly::AND(r1, r2, r3);
            }

            auto Term_OR() -> void {
                // Term_OR -> NONE
                if (node.children.size() == 1) { return; }

                // Term_OR -> OR Term_1 Term_OR
                const int r1 = PopRegister();
                const int r2 = PopRegister();
                const int r3 = PushRegister();
                assembly::ORR(r1, r2, r3);
            }

            auto Term_EQUALS() -> void {
                // Term_AND -> NONE
                if (node.children.size() == 1) { return; }

                // Term_AND -> AND Term_1 Term_AND
                const int r1 = PopRegister();
                const int r2 = PopRegister();
                const int r3 = programs::IsEqual(r1, r2);
            }

            auto Term_NOT_EQUALS() -> void {
                // Term_AND -> NONE
                if (node.children.size() == 1) { return; }

                // Term_AND -> AND Term_1 Term_AND
                const int r1 = PopRegister();
                const int r2 = PopRegister();
                const int r3 = programs::IsEqual(r1, r2);
                assembly::NOT(r3, r3);
            }

            auto Term_GREATER() -> void {
                // Term_AND -> NONE
                if (node.children.size() == 1) { return; }

                // TODO implement this
            }

            auto Term_GREATER_OR_EQUAL() -> void {
                // Term_AND -> NONE
                if (node.children.size() == 1) { return; }

                // TODO implement this
            }

            auto Term_LESS() -> void {
                // Term_AND -> NONE
                if (node.children.size() == 1) { return; }

                // TODO implement this
            }

            auto Term_LESS_OR_EQUAL() -> void {
                // Term_AND -> NONE
                if (node.children.size() == 1) { return; }

                // TODO implement this
            }

            auto Term_SUB() -> void {
                // Term_ADD -> NONE
                if (node.children.size() == 1) { return; }

                // Term_ADD -> ADD Term_10 Term_ADD
                const int r1 = PopRegister();
                const int r2 = PopRegister();
                const int r3 = PushRegister();
                assembly::SUB(r1, r2, r3);
            }

            auto Term_ADD() -> void {
                // Term_ADD -> NONE
                if (node.children.size() == 1) { return; }

                // Term_ADD -> ADD Term_10 Term_ADD
                const int r1 = PopRegister();
                const int r2 = PopRegister();
                const int r3 = PushRegister();
                assembly::ADD(r1, r2, r3);
            }

            auto Term_MULTIPLY() -> void {
                // Term_ADD -> NONE
                if (node.children.size() == 1) { return; }

                // Term_ADD -> ADD Term_10 Term_ADD
                const int r1 = PopRegister();
                const int r2 = PopRegister();
                const int r3 = PushRegister();
                assembly::ADD(r1, r2, r3);
            }

            auto Variable() -> void {
                // Variable -> Dereference
                if (node.children.size() == 1) { return; }

                // Variable -> IDENTIFIER IdentifierOperation
                const string identifier = node.children[0].token.text;
                const int r1 = PushRegister();
                assembly::LDA(identifier);
            }
        }

        unordered_map<int, void(*)()> generatorFunctions = {
            {Term_ADD, generate::Term_ADD}
        };
    }

    auto Generate(const parser::TreeNode _node) -> vector<string> {
        node = _node;
        (generatorFunctions.at(node.token.type))();
        return assembly::GetProgram();
    }
}