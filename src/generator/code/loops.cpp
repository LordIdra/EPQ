#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"
#include "generator/dataValues.hpp"



namespace generator {
    namespace LoopCondition {
        auto First() -> void {
            loopStartStack.push(assembly::GenerateLabel("loopStart"));
            loopEndStack.push(assembly::GenerateLabel("loopEnd"));

            assembly::Label(loopStartStack.top());
        }

        auto Last() -> void {
            const int r1 = PopValue();

            assembly::NOT(r1, r1);
            assembly::BRP(loopEndStack.top(), r1);

            dataValues::Free(r1);
        }
    }

    namespace For {
        auto Last() -> void {
            assembly::BRA(loopStartStack.top());
            assembly::Label(loopEndStack.top());

            loopStartStack.pop();
            loopEndStack.pop();
        }
    }

    namespace While {
        auto Last() -> void {
            assembly::BRA(loopStartStack.top());
            assembly::Label(loopEndStack.top());

            loopStartStack.pop();
            loopEndStack.pop();
        }
    }

    namespace Break {
        auto First() -> void {
            assembly::BRA(loopEndStack.top());
        }
    }

    namespace Continue {
        auto First() -> void {
            assembly::BRA(loopAssignmentStack.top());
        }
    }
}