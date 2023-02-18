#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"
#include "generator/registers.hpp"
#include "arithmetic.cpp"



namespace generator {
    namespace Declaration {
        auto Last() -> void {
            // Declaration -> Datatype Declaration_0
            {
                // Declaration_0 -> OPEN_SQUARE_BRACKET NUMBER CLOSE_SQUARE_BRACKET IDENTIFIER
                // Declaration -> Datatype OPEN_SQUARE_BRACKET NUMBER CLOSE_SQUARE_BRACKET IDENTIFIER
                if (node->children.at(1).children.at(0).token.type == OPEN_SQUARE_BRACKET) {
                    // TODO (lists)
                }

                // Declaration_0 -> IDENTIFIER ASSIGN InputTerm
                // Declaration -> Datatype IDENTIFIER ASSIGN InputTerm
                if (node->children.at(1).children.at(0).token.type == IDENTIFIER) {
                    const int r1 = registers::Allocate();
                    const int r2 = registers::Allocate();
                    const int r3 = registers::Allocate();

                    const int r4 = PopRegister();

                    const string identifier = node->children.at(1).children.at(0).token.text;

                    const int address = scopeTraverser.LocalLookup(identifier).address;

                    const int a1 = div(address, 256).quot;
                    const int a2 = div(div(address, 256).rem, 16).quot;
                    const int a3 = div(div(address, 256).rem, 16).rem;

                    assembly::Comment("store " + identifier);
                    assembly::SET(a1, r1);
                    assembly::SET(a2, r2);
                    assembly::SET(a3, r3);

                    assembly::MOV(r4, registers::MDR1);
                    assembly::STA(r1, r2, r3);

                    registers::Free(r1);
                    registers::Free(r2);
                    registers::Free(r3);
                }
            }
        }

    }

    namespace AssignmentOperation {
        auto Last() -> void {
            // AssignmentOperation -> ASSIGN InputTerm
            if (GetChildType(0) == ASSIGN) {
                const int assignFrom = PopRegister();
                const int assignTo = registerStack.top();
                assembly::MOV(assignFrom, assignTo);
                return;
            }

            // AssignmentOperation -> ADD_ASSIGN Term
            if (GetChildType(0) == ADD_ASSIGN) {
                const int assignFrom = PopRegister();
                const int assignTo = registerStack.top();
                assembly::ADD(assignTo, assignFrom, assignTo);
                return;
            }

            // AssignmentOperation -> SUBTRACT_ASSIGN Term
            if (GetChildType(0) == SUBTRACT_ASSIGN) {
                const int assignFrom = PopRegister();
                const int assignTo = registerStack.top();
                assembly::SUB(assignTo, assignFrom, assignTo);
                return;
            }

            // AssignmentOperation -> MULTIPLY_ASSIGN Term
            if (GetChildType(0) == MULTIPLY_ASSIGN) {
                Term_MULTIPLY::Last();
                return;
            }

            // AssignmentOperation -> DIVIDE_ASSIGN Term
            if (GetChildType(0) == DIVIDE_ASSIGN) {
                Term_DIVIDE::Last();
                return;
            }

            // AssignmentOperation -> MODULUS_ASSIGN Term
            if (GetChildType(0) == MODULUS_ASSIGN) {
                Term_MODULUS::Last();
                return;
            }

            // AssignmentOperation -> INCREMENT
            if (GetChildType(0) == INCREMENT) {
                const int r3 = registers::Allocate();
                const int r2 = registers::Allocate();
                const int r1 = PopRegister();

                assembly::SET(1, r2);
                assembly::ADD(r1, r2, r3);

                PushRegister(r3);
                registers::Free(r2);
                return;
            }

            // AssignmentOperation -> DECREMENT
            if (GetChildType(0) == DECREMENT) {
                const int r3 = registers::Allocate();
                const int r2 = registers::Allocate();
                const int r1 = PopRegister();

                assembly::SET(1, r2);
                assembly::SUB(r1, r2, r3);

                PushRegister(r3);
                registers::Free(r2);
                return;
            }
        }
    }

    namespace Assignment {
        auto Last() -> void {
            // Assignment -> Variable AssignmentOperation
            {
                // Variable -> IDENTIFIER IdentifierSuffix
                // Assignment -> IDENTTIFIER IdentifierSuffix AssignmentOperation
                if (node->children.at(0).children.at(0).token.type == IDENTIFIER) {
                    const int r1 = registers::Allocate();
                    const int r2 = registers::Allocate();
                    const int r3 = registers::Allocate();

                    const int r4 = PopRegister();

                    const string identifier = node->children.at(0).children.at(0).token.text;

                    const int address = scopeTraverser.LocalLookup(identifier).address;

                    const int a1 = div(address, 256).quot;
                    const int a2 = div(div(address, 256).rem, 16).quot;
                    const int a3 = div(div(address, 256).rem, 16).rem;

                    assembly::Comment("store " + identifier);
                    assembly::SET(a1, r1);
                    assembly::SET(a2, r2);
                    assembly::SET(a3, r3);

                    assembly::MOV(r4, registers::MDR1);
                    assembly::STA(r1, r2, r3);

                    registers::Free(r1);
                    registers::Free(r2);
                    registers::Free(r3);
                }

                // Variable -> Dereference
                // Assignment -> Dereference AssignmentOperation
                else {
                    // TODO (12-bit datatype)
                }
            }
        }
    }
}