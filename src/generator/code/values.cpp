#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"
#include "generator/dataValues.hpp"



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
                    const int r4 = dataValues::Allocate();

                    assembly::LDA(identifier);
                    assembly::MOV(MDR_1, r4);

                    PushValue(r4);
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
            const int r1 = dataValues::Allocate();

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
                assembly::SET(dataValues::TRUE, r1);
            }

            // Literal -> FALSE
            if (GetChildType(0) == FALSE) {
                assembly::SET(dataValues::FALSE, r1);
            }

            PushValue(r1);
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