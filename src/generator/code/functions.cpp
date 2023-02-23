#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"
#include "generator/dataValue.hpp"
#include "generator/dataValues.hpp"
#include "semanticAnalyser/scopes/scopeManager.hpp"



namespace generator {
    namespace Parameter {
        auto First() -> void {
            const string identifier = node->children.at(1).token.text;

            const int address = scopeTraverser.LocalLookup(identifier).address;

            const int a1 = div(address, 256).quot;
            const int a2 = div(div(address, 256).rem, 16).quot;
            const int a3 = div(div(address, 256).rem, 16).rem;

            const int r1 = dataValues::Allocate();
            const int r2 = dataValues::Allocate();
            const int r3 = dataValues::Allocate();

            assembly::POP();

            assembly::Comment("store " + identifier);
            assembly::SET(a1, r1);
            assembly::SET(a2, r2);
            assembly::SET(a3, r3);
            assembly::STA(r1, r2, r3);

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r3);
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
            const int r1 = PopValue();

            assembly::MOV(r1, MDR_1);
            assembly::PSH();

            dataValues::Free(r1);
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
            const int r1 = dataValues::Allocate();
            const int r2 = dataValues::Allocate();
            const int r3 = dataValues::Allocate();

            assembly::SET("1" + identifier, r1);
            assembly::SET("2" + identifier, r2);
            assembly::SET("3" + identifier, r3);
            assembly::BRA(r1, r2, r3);

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r3);

            // Return code
            const SymbolType returnType = ScopeManager::GetFunctionSymbol(identifier).returnType;

            if (returnType != TYPE_VOID) {
                const int returnRegister = dataValues::Allocate();

                assembly::POP();
                assembly::MOV(MDR_1, returnRegister);

                PushValue(returnRegister);
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
            const int returnAddress1b = dataValues::Allocate();
            const int returnAddress2a = dataValues::Allocate();
            const int returnAddress2b = dataValues::Allocate();

            assembly::POP();
            assembly::MOV(MDR_2, returnAddress1b);
            assembly::POP();
            assembly::MOV(MDR_1, returnAddress2a);
            assembly::MOV(MDR_2, returnAddress2b);

            // Push return value to stack
            const int returnValue = PopValue();
            assembly::MOV(returnValue, MDR_1);
            assembly::PSH();

            // Push return address back onto stack
            assembly::MOV(returnAddress2b, MDR_2);
            assembly::MOV(returnAddress2a, MDR_1);
            assembly::PSH();
            assembly::MOV(returnAddress1b, MDR_2);
            assembly::PSH();

            dataValues::Free(returnAddress1b);
            dataValues::Free(returnAddress2a);
            dataValues::Free(returnAddress2b);
            dataValues::Free(returnValue);
        }
    }
}