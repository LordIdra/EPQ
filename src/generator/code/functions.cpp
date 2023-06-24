#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"
#include "generator/dataValue.hpp"
#include "generator/dataValues.hpp"
#include "semanticAnalyser/scopes/scopeManager.hpp"



namespace generator {

    namespace {
        vector<int> argumentList;
    }

    namespace Parameter {
        auto First() -> void {
            const string identifier = node->children.at(1).token.text;

            assembly::POP();
            assembly::STA(identifier);
        }
    }

    namespace FunctionDeclaration {

        auto First() -> void {
            // FunctionDeclaration -> VoidableDatatype IDENTIFIER ParameterList_1 N_Block FunctionDeclaration

            if (node->children.size() == 5) {
                const string identifier = node->children.at(1).token.text;
                assembly::Label(identifier);
            }

            // RET is added in RecursiveGenerateFunctionDeclaration
        }
    }

    namespace Argument {
        auto First() -> void {
            const int r1 = PopValue();

            argumentList.push_back(r1);
        }
    }

    namespace FunctionCall {
        auto First() -> void {
            const string identifier = node->children[1].token.text;
            argumentList.clear();
        }

        auto Last() -> void {
            const string identifier = node->children.at(1).token.text;
            const int programCounterIncrement = 4 + (2*argumentList.size());

            for (const int argument : argumentList) {
                // Make sure all arguments are stored in registers
                // This makes sure that each argument only occupies 2 instructions
                dataValues::Load(argument);
            }

            assembly::Comment("call " + identifier);
            assembly::PPC(programCounterIncrement);

            for (const int argument : argumentList) {
                assembly::MOV(argument, MDR_1);
                dataValues::Free(argument);
                assembly::PSH();
            }

            // Call code
            assembly::BRA(identifier);

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