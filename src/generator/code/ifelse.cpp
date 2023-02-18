#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"
#include "generator/registers.hpp"



namespace generator {
    namespace If {
        auto First() -> void {
            branchEndStack.push(assembly::GenerateLabel("branchEnd"));
            branchFinalStack.push(assembly::GenerateLabel("branchFinal"));
        }

        auto Last() -> void {
            const int r2 = registers::Allocate();
            const int r3 = registers::Allocate();
            const int r4 = registers::Allocate();

            const int r1 = PopRegister();

            // If the IF statement is followed by an ELSE or ELSE IF statement
            // const auto ifBlock = node->parent->parent;
            // const bool hasElseIf = ifBlock->children.at(1).children.at(0).token.type != NONE;
            // const bool hasElse = ifBlock->children.at(2).children.at(0).token.type != NONE;

            // if (hasElse || hasElseIf) {
            //     assembly::Comment("branch to " + branchEndStack.top());
            //     assembly::SET("1" + branchEndStack.top(), r2);
            //     assembly::SET("2" + branchEndStack.top(), r3);
            //     assembly::SET("3" + branchEndStack.top(), r4);
            //     assembly::BRP(r2, r3, r4, r1);
            // }

            assembly::NOT(r1, r1);
            assembly::Comment("branch to " + branchEndStack.top());
            assembly::SET("1" + branchEndStack.top(), r2);
            assembly::SET("2" + branchEndStack.top(), r3);
            assembly::SET("3" + branchEndStack.top(), r4);
            assembly::BRP(r2, r3, r4, r1);

            registers::Free(r2);
            registers::Free(r3);
            registers::Free(r4);
        }
    }

    //{ElseIf, ElseIf::First},
    //{Else, Else::First},

    namespace N_If {
        auto Last() -> void {
            const int r2 = registers::Allocate();
            const int r3 = registers::Allocate();
            const int r4 = registers::Allocate();

            assembly::Comment("branch to " + branchFinalStack.top());
            assembly::SET("1" + branchFinalStack.top(), r2);
            assembly::SET("2" + branchFinalStack.top(), r3);
            assembly::SET("3" + branchFinalStack.top(), r4);
            assembly::BRA(r2, r3, r4);

            assembly::NOP();
            assembly::LabelLatestInstruction(branchEndStack.top());

            branchEndStack.pop();

            registers::Free(r2);
            registers::Free(r3);
            registers::Free(r4);
        }
    }

    namespace ElseIf {
        auto First() -> void {
            branchEndStack.push(assembly::GenerateLabel("branchEnd"));
        }

        auto Last() -> void {
            If::Last();
        }
    }

    namespace N_ElseIf {
        auto Last() -> void {
            N_If::Last();
        }
    }

    namespace N_IfBlock {
        auto Last() -> void {
            assembly::NOP();
            assembly::LabelLatestInstruction(branchFinalStack.top());
            branchFinalStack.pop();
        }
    }

    namespace L_If {
        auto Last() -> void {
            N_If::Last();
        }
    }

    namespace L_ElseIf {
        auto Last() -> void {
            N_ElseIf::Last();
        }
    }

    namespace L_IfBlock {
        auto Last() -> void {
            N_IfBlock::Last();
        }
    }
}