#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"
#include "generator/dataValues.hpp"



namespace generator {
    namespace LoopCondition {
        auto First() -> void {
            loopStartStack.push(assembly::GenerateLabel("loopStart"));
            loopEndStack.push(assembly::GenerateLabel("loopEnd"));

            assembly::NOP();
            assembly::LabelLatestInstruction(loopStartStack.top());
        }

        auto Last() -> void {

            const int r2 = dataValues::Allocate();
            const int r3 = dataValues::Allocate();
            const int r4 = dataValues::Allocate();

            const int r1 = PopValue();

            assembly::NOT(r1, r1);
            assembly::Comment("branch to " + loopEndStack.top());
            assembly::SET("1" + loopEndStack.top(), r2);
            assembly::SET("2" + loopEndStack.top(), r3);
            assembly::SET("3" + loopEndStack.top(), r4);
            assembly::BRP(r2, r3, r4, r1);

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r3);
            dataValues::Free(r4);
        }
    }

    namespace For {
        auto Last() -> void {
            const int r1 = dataValues::Allocate();
            const int r2 = dataValues::Allocate();
            const int r3 = dataValues::Allocate();

            assembly::SET("1" + loopStartStack.top(), r1);
            assembly::SET("2" + loopStartStack.top(), r2);
            assembly::SET("3" + loopStartStack.top(), r3);
            assembly::Comment("branch to " + loopStartStack.top());
            assembly::BRA(r1, r2, r3);
            assembly::NOP();
            assembly::LabelLatestInstruction(loopEndStack.top());

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r3);

            loopStartStack.pop();
            loopEndStack.pop();
        }
    }

    namespace While {
        auto Last() -> void {
            const int r1 = dataValues::Allocate();
            const int r2 = dataValues::Allocate();
            const int r3 = dataValues::Allocate();

            assembly::Comment("branch to " + loopStartStack.top());
            assembly::SET("1" + loopStartStack.top(), r1);
            assembly::SET("2" + loopStartStack.top(), r2);
            assembly::SET("3" + loopStartStack.top(), r3);
            assembly::BRA(r1, r2, r3);
            assembly::NOP();
            assembly::LabelLatestInstruction(loopEndStack.top());

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r3);

            loopStartStack.pop();
            loopEndStack.pop();
        }
    }

    namespace Break {
        auto First() -> void {
            const int r1 = dataValues::Allocate();
            const int r2 = dataValues::Allocate();
            const int r3 = dataValues::Allocate();

            assembly::Comment("branch to " + loopEndStack.top());
            assembly::SET("1" + loopEndStack.top(), r1);
            assembly::SET("2" + loopEndStack.top(), r2);
            assembly::SET("3" + loopEndStack.top(), r3);
            assembly::BRA(r1, r2, r3);

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r3);
        }
    }

    namespace Continue {
        auto First() -> void {
            const int r1 = dataValues::Allocate();
            const int r2 = dataValues::Allocate();
            const int r3 = dataValues::Allocate();

            assembly::Comment("branch to " + loopAssignmentStack.top());
            assembly::SET("1" + loopAssignmentStack.top(), r1);
            assembly::SET("2" + loopAssignmentStack.top(), r2);
            assembly::SET("3" + loopAssignmentStack.top(), r3);
            assembly::BRA(r1, r2, r3);

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r3);
        }
    }
}