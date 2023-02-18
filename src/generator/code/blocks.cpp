#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"
#include "generator/registers.hpp"



namespace generator {
    namespace SimpleStatement {
        auto First() -> void {
            // SimpleStatement -> OUTPUT Term SEMICOLON
            if (GetChildType(0) != OUTPUT) {
                // TODO (new CPU section)
            }
        }
    }

    namespace N_Block_0 {
        auto First() -> void {}
        auto Last() -> void {}
    }

    namespace L_Block_0 {
        auto First() -> void {}
        auto Last() -> void {}
    }

    namespace Program {
        auto First() -> void {
            const int r1 = registers::Allocate();
            const int r2 = registers::Allocate();
            const int r3 = registers::Allocate();

            const int IHaveNoIdea = 4;

            assembly::Comment("branch to main");
            assembly::PPC(IHaveNoIdea);
            assembly::SET("1" "main", r1);
            assembly::SET("2" "main", r2);
            assembly::SET("3" "main", r3);
            assembly::BRA(r1, r2, r3);

            assembly::Comment("branch to end of program");
            assembly::SET(14, r1);
            assembly::SET(15, r2);
            assembly::SET(7, r3);
            assembly::BRA(r1, r2, r3);

            registers::Free(r1);
            registers::Free(r2);
            registers::Free(r3);
        }
    }
}