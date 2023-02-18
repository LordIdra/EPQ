#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"
#include "generator/registers.hpp"
#include "semanticAnalyser/scopes/scopeManager.hpp"



namespace generator {
    namespace Parameter {
        auto First() -> void {
            const string identifier = node->children.at(1).token.text;

            const int address = scopeTraverser.LocalLookup(identifier).address;

            const int a1 = div(address, 256).quot;
            const int a2 = div(div(address, 256).rem, 16).quot;
            const int a3 = div(div(address, 256).rem, 16).rem;

            const int r1 = registers::Allocate();
            const int r2 = registers::Allocate();
            const int r3 = registers::Allocate();

            assembly::POP();

            assembly::Comment("store " + identifier);
            assembly::SET(a1, r1);
            assembly::SET(a2, r2);
            assembly::SET(a3, r3);
            assembly::STA(r1, r2, r3);

            registers::Free(r1);
            registers::Free(r2);
            registers::Free(r3);
        }
    }

    namespace FunctionDeclaration {

        auto First() -> void {
            // FunctionDeclaration -> VoidableDatatype IDENTIFIER ParameterList_1 N_Block FunctionDeclaration

            if (node->children.size() == 5) {
                const string identifier = node->children.at(1).token.text;
                assembly::NOP();
                assembly::LabelLatestInstruction(identifier);
            }

            // RET is added in RecursiveGenerateFunctionDeclaration
        }

        auto Last() -> void {

        }
    }

    namespace Argument {
        auto First() -> void {
            const int r1 = PopRegister();
            assembly::MOV(r1, registers::MDR1);
            assembly::PSH();
        }
    }

    namespace FunctionCall {
        auto First() -> void {
            const string identifier = node->children[1].token.text;
            const int IHaveNoIdea = 4;

            assembly::Comment("call " + identifier);
            assembly::PPC(IHaveNoIdea);
        }

        auto Last() -> void {
            const string identifier = node->children.at(1).token.text;

            // Call code
            const int r1 = registers::Allocate();
            const int r2 = registers::Allocate();
            const int r3 = registers::Allocate();

            assembly::SET("1" + identifier, r1);
            assembly::SET("2" + identifier, r2);
            assembly::SET("3" + identifier, r3);
            assembly::BRA(r1, r2, r3);

            registers::Free(r1);
            registers::Free(r2);
            registers::Free(r3);

            // Return code
            const SymbolType returnType = ScopeManager::GetFunctionSymbol(identifier).returnType;

            if (returnType != TYPE_VOID) {
                const int returnRegister = registers::Allocate();

                assembly::POP();
                assembly::MOV(registers::MDR1, returnRegister);

                PushRegister(returnRegister);
            }
        }
    }

    namespace ReturnContents {
        auto Last() -> void {

            // ReturnContents -> NONE
            if (node->children.at(0).token.type == NONE) {
                return;
            }

            // ReturnContents -> Term
            // Pop return address off stack
            const int returnAddress1b = registers::Allocate();
            const int returnAddress2a = registers::Allocate();
            const int returnAddress2b = registers::Allocate();

            assembly::POP();
            assembly::MOV(registers::MDR2, returnAddress1b);
            assembly::POP();
            assembly::MOV(registers::MDR1, returnAddress2a);
            assembly::MOV(registers::MDR2, returnAddress2b);

            // Push return value to stack
            const int returnValue = PopRegister();
            assembly::MOV(returnValue, registers::MDR1);
            assembly::PSH();

            // Push return address back onto stack
            assembly::MOV(returnAddress2b, registers::MDR2);
            assembly::MOV(returnAddress2a, registers::MDR1);
            assembly::PSH();
            assembly::MOV(returnAddress1b, registers::MDR2);
            assembly::PSH();

            registers::Free(returnAddress1b);
            registers::Free(returnAddress2a);
            registers::Free(returnAddress2b);
        }
    }
}