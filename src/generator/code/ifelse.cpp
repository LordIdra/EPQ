#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"
#include "generator/dataValues.hpp"



namespace generator {
    namespace If {
        auto First() -> void {
            branchEndStack.push(assembly::GenerateLabel("branchEnd"));
            branchFinalStack.push(assembly::GenerateLabel("branchFinal"));
        }

        auto Last() -> void {
             const int r1 = PopValue();

            assembly::NOT(r1, r1);
            assembly::BRP(branchEndStack.top(), r1);

            dataValues::Free(r1);
        }
    }

    //{ElseIf, ElseIf::First},
    //{Else, Else::First},

    namespace N_If {
        auto Last() -> void {
            assembly::BRA(branchFinalStack.top());
            assembly::Label(branchEndStack.top());

            branchEndStack.pop();
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
            assembly::Label(branchFinalStack.top());
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