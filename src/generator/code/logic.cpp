#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"
#include "generator/dataValues.hpp"



namespace generator {
    namespace Term_AND {
        auto Last() -> void {
            // Term_AND -> NONE
            if (node->children.size() == 1) { return; }

            // Term_AND -> AND Term_1 Term_AND
            const int r3 = dataValues::Allocate();
            const int r1 = PopValue();
            const int r2 = PopValue();

            assembly::AND(r1, r2, r3);

            dataValues::Free(r1);
            dataValues::Free(r2);
            PushValue(r3);
        }
    }

    namespace Term_OR {
        auto Last() -> void {
            // Term_OR -> NONE
            if (node->children.size() == 1) { return; }

            // Term_OR -> OR Term_2 Term_OR
            const int r3 = dataValues::Allocate();
            const int r1 = PopValue();
            const int r2 = PopValue();

            assembly::ORR(r1, r2, r3);

            dataValues::Free(r1);
            dataValues::Free(r2);
            PushValue(r3);
        }
    }
}