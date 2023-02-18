#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"
#include "generator/registers.hpp"



namespace generator {
    namespace Reference {
        // TODO (12-bit datatype)
        auto First() -> void {}
        auto Last() -> void {}
    }

    namespace Dereference {
        // TODO (12-bit datatype)
        auto First() -> void {}
        auto Last() -> void {}
    }

    namespace ListIndex {
        // TODO (lists)
        auto First() -> void {}
        auto Last() -> void {}
    }

    namespace Listindex {
        // TODO (12-bit datatype)
        auto First() -> void {}
        auto Last() -> void {}
    }

    // Value
    namespace Variable {
        auto First() -> void {
            // Variable -> Dereference
            if (node->children.size() == 1) {}

            // Variable -> IDENTIFIER IdentifierSuffix
            else {
                // IdentifierSuffix -> NONE
                // Variable -> IDENTIFIER
                if (node->children.at(1).children.at(0).token.type == NONE) {
                    const string identifier = GetChildText(0);

                    const int address = scopeTraverser.LocalLookup(identifier).address;

                    const int a1 = div(address, 256).quot;
                    const int a2 = div(div(address, 256).rem, 16).quot;
                    const int a3 = div(div(address, 256).rem, 16).rem;

                    const int r1 = registers::Allocate();
                    const int r2 = registers::Allocate();
                    const int r3 = registers::Allocate();

                    const int r4 = registers::Allocate();

                    assembly::Comment("load " + identifier);
                    assembly::SET(a1, r1);
                    assembly::SET(a2, r2);
                    assembly::SET(a3, r3);
                    assembly::LDA(r1, r2, r3);

                    assembly::MOV(registers::MDR1, r4);

                    PushRegister(r4);

                    registers::Free(r1);
                    registers::Free(r2);
                    registers::Free(r3);
                }

                // IdentifierSuffix -> ListIndex
                // Variable -> IDENTIFIER ListIndex
                else {
                    // TODO (12-bit datatype)
                }
            }
        }
    }

    namespace IdentifierSuffix {
        // TODO (lists)
        auto First() -> void {}
        auto Last() -> void {}
    }

    namespace Literal {
        auto First() -> void {
            const int r1 = registers::Allocate();

            // Literal -> NUMBER
            if (GetChildType(0) == NUMBER) {
                int v1 = std::atoi(GetChildText(0).c_str());
                if (v1 < 0) {
                    v1 = 16 + v1;
                }
                assembly::SET(v1, r1);
            }

            // Literal -> TRUE
            if (GetChildType(0) == TRUE) {
                assembly::SET(registers::TRUE, r1);
            }

            // Literal -> FALSE
            if (GetChildType(0) == FALSE) {
                assembly::SET(registers::FALSE, r1);
            }

            PushRegister(r1);
        }
    }

    namespace InputTerm {
        auto First() -> void {

            // Literal -> INPUT
            if (GetChildType(0) == INPUT) {
                // TODO (new CPU section)
            }
        }
    }
}