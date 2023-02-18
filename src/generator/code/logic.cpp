#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"
#include "generator/registers.hpp"



namespace generator {
    namespace Term_AND {
        auto Last() -> void {
            // Term_AND -> NONE
            if (node->children.size() == 1) { return; }

            // Term_AND -> AND Term_1 Term_AND
            const int r3 = registers::Allocate();

            const int r1 = PopRegister();
            const int r2 = PopRegister();

            assembly::AND(r1, r2, r3);

            PushRegister(r3);
        }
    }

    namespace Term_OR {
        auto Last() -> void {
            // Term_OR -> NONE
            if (node->children.size() == 1) { return; }

            // Term_OR -> OR Term_2 Term_OR
            const int r3 = registers::Allocate();

            const int r1 = PopRegister();
            const int r2 = PopRegister();

            assembly::ORR(r1, r2, r3);

            PushRegister(r3);
        }
    }
}