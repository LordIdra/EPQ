#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"



namespace generator {
    namespace Term_SUB {
        auto Last() -> void {
            // Term_SUB -> NONE
            if (node->children.size() == 1) { return; }

            // Term_SUB -> SUB Term_9 Term_SUB
            const int out = dataValues::Allocate();
            const int in_2 = PopValue();
            const int in_1 = PopValue();

            assembly::SUB(in_1, in_2, out);

            dataValues::Free(in_1);
            dataValues::Free(in_2);
            PushValue(out);
        }
    }

    namespace Term_ADD {
        auto Last() -> void {
            // Term_ADD -> NONE
            if (node->children.size() == 1) { return; }

            // Term_ADD -> ADD Term_10 Term_ADD
            const int out = dataValues::Allocate();
            const int in_2 = PopValue();
            const int in_1 = PopValue();

            assembly::ADD(in_1, in_2, out);

            dataValues::Free(in_1);
            dataValues::Free(in_2);
            PushValue(out);
        }
    }

    namespace Term_MULTIPLY {
        auto Last() -> void {
            // Term_ADD -> NONE
            if (node->children.size() == 1) { return; }

            // Term_MULTIPLY -> MULTIPLY Term_11 Term_MULTIPLY
            const string labelMultiplyLoop = assembly::GenerateLabel("multiplyLoop");
            const string labelFirstNumberPositive = assembly::GenerateLabel("firstNumberPositive");
            const string labelSecondNumberPositive = assembly::GenerateLabel("secondNumberPositive");
            const string labelResultPositive = assembly::GenerateLabel("resultPositive");

            const int r3 = dataValues::Allocate();
            const int r4 = dataValues::Allocate();
            const int r5 = dataValues::Allocate();

            const int r6 = dataValues::Allocate();
            const int r7 = dataValues::Allocate();
            const int r8 = dataValues::Allocate();

            const int r1 = PopValue();
            const int r2 = PopValue();

            // Used to determine whether the result is negative later on
            assembly::XOR(r1, r2, r5);

            // Set input registers to both be positive
            assembly::SET(1, r3);

            assembly::Comment("branch to " + labelFirstNumberPositive);
            assembly::SET("1" + labelFirstNumberPositive, r6);
            assembly::SET("2" + labelFirstNumberPositive, r7);
            assembly::SET("3" + labelFirstNumberPositive, r8);
            assembly::BRP(r6, r7, r8, r1);
            assembly::NOT(r1, r1);
            assembly::ADD(r1, r3, r1);

            assembly::LabelLatestInstruction(labelFirstNumberPositive);                    

            assembly::Comment("branch to " + labelSecondNumberPositive);
            assembly::SET("1" + labelSecondNumberPositive, r6);
            assembly::SET("2" + labelSecondNumberPositive, r7);
            assembly::SET("3" + labelSecondNumberPositive, r8);
            assembly::BRP(r6, r7, r8, r2);
            assembly::NOT(r2, r2);
            assembly::ADD(r2, r3, r2);

            assembly::LabelLatestInstruction(labelSecondNumberPositive);

            // Set up registers
            assembly::SET(1, r3);
            assembly::SET(0, r4);
            assembly::SUB(r4, r1, r4);

            // Standard multiplication loop
            assembly::LabelLatestInstruction(labelMultiplyLoop);
            assembly::SUB(r2, r3, r2);
            assembly::ADD(r4, r1, r4);
            assembly::Comment("branch to " + labelMultiplyLoop);
            assembly::SET("1" + labelMultiplyLoop, r6);
            assembly::SET("2" + labelMultiplyLoop, r7);
            assembly::SET("3" + labelMultiplyLoop, r8);
            assembly::BRP(r6, r7, r8, r2);

            // If only one register is negative, the result will be negative
            //assembly::XOR(r4, r5, r4);
            assembly::Comment("branch to " + labelResultPositive);
            assembly::SET("1" + labelResultPositive, r6);
            assembly::SET("2" + labelResultPositive, r7);
            assembly::SET("3" + labelResultPositive, r8);
            assembly::BRP(r6, r7, r8, r5);
            assembly::SET(1, r3);
            assembly::NOT(r4, r4);
            assembly::ADD(r4, r3, r4);

            assembly::LabelLatestInstruction(labelResultPositive);

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r3);
            dataValues::Free(r5);
            dataValues::Free(r6);
            dataValues::Free(r7);
            dataValues::Free(r8);
            PushValue(r4);
        }
    }

    namespace Term_DIVIDE {
        auto Last() -> void {
            // Term_DIVIDE -> NONE
            if (node->children.size() == 1) { return; }

            // Term_DIVIDE -> DIVIDE Term_12 Term_DIVIDE
            const string labelDivideLoop = assembly::GenerateLabel("divideLoop");
            const string labelFirstNumberPositive = assembly::GenerateLabel("firstNumberPositive");
            const string labelSecondNumberPositive = assembly::GenerateLabel("secondNumberPositive");
            const string labelResultPositive = assembly::GenerateLabel("resultPositive");

            const int r3 = dataValues::Allocate();
            const int r4 = dataValues::Allocate();
            const int r5 = dataValues::Allocate();

            const int r6 = dataValues::Allocate();
            const int r7 = dataValues::Allocate();
            const int r8 = dataValues::Allocate();

            const int r2 = PopValue();
            const int r1 = PopValue();

            // Used to determine whether the result is negative later on
            assembly::XOR(r1, r2, r5);

            // Set input registers to both be positive
            assembly::SET(1, r3);

            assembly::Comment("branch to " + labelFirstNumberPositive);
            assembly::SET("1" + labelFirstNumberPositive, r6);
            assembly::SET("2" + labelFirstNumberPositive, r7);
            assembly::SET("3" + labelFirstNumberPositive, r8);
            assembly::BRP(r6, r7, r8, r1);
            assembly::NOT(r1, r1);
            assembly::ADD(r1, r3, r1);

            assembly::LabelLatestInstruction(labelFirstNumberPositive);                    

            assembly::Comment("branch to " + labelSecondNumberPositive);
            assembly::SET("1" + labelSecondNumberPositive, r6);
            assembly::SET("2" + labelSecondNumberPositive, r7);
            assembly::SET("3" + labelSecondNumberPositive, r8);
            assembly::BRP(r6, r7, r8, r2);
            assembly::NOT(r2, r2);
            assembly::ADD(r2, r3, r2);

            assembly::LabelLatestInstruction(labelSecondNumberPositive);

            // Set up registers
            assembly::SET(1, r3);
            assembly::SET(0, r4);
            assembly::SUB(r4, r3, r4);

            // Standard division loop (r1 / r2)
            assembly::LabelLatestInstruction(labelDivideLoop);
            assembly::SUB(r1, r2, r1);
            assembly::ADD(r4, r3, r4);
            assembly::Comment("branch to " + labelDivideLoop);
            assembly::SET("1" + labelDivideLoop, r6);
            assembly::SET("2" + labelDivideLoop, r7);
            assembly::SET("3" + labelDivideLoop, r8);
            assembly::BRP(r6, r7, r8, r1);

            // If only one register is negative, the result will be negative
            //assembly::XOR(r4, r5, r4);
            assembly::Comment("branch to " + labelResultPositive);
            assembly::SET("1" + labelResultPositive, r6);
            assembly::SET("2" + labelResultPositive, r7);
            assembly::SET("3" + labelResultPositive, r8);
            assembly::BRP(r6, r7, r8, r5);
            assembly::SET(1, r3);
            assembly::NOT(r4, r4);
            assembly::ADD(r4, r3, r4);

            assembly::LabelLatestInstruction(labelResultPositive);

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r3);
            dataValues::Free(r5);
            dataValues::Free(r6);
            dataValues::Free(r7);
            dataValues::Free(r8);
            PushValue(r4);
        }
    }

    namespace Term_MODULUS {
        auto Last() -> void {
            // Term_MODULUS -> NONE
            if (node->children.size() == 1) { return; }

            // Term_MODULUS -> MODULUS Term_12 Term_MODULUS
            const string labelModulusLoop = assembly::GenerateLabel("modulusLoop");
            const string labelFirstNumberPositive = assembly::GenerateLabel("firstNumberPositive");
            const string labelSecondNumberPositive = assembly::GenerateLabel("secondNumberPositive");
            const string labelResultPositive = assembly::GenerateLabel("resultPositive");

            const int r3 = dataValues::Allocate();
            const int r4 = dataValues::Allocate();
            const int r5 = dataValues::Allocate();

            const int r6 = dataValues::Allocate();
            const int r7 = dataValues::Allocate();
            const int r8 = dataValues::Allocate();

            const int r2 = PopValue();
            const int r1 = dataStack.top();

            // Used to determine whether the result is negative later on
            assembly::XOR(r1, r2, r5);

            // Set input registers to both be positive
            assembly::SET(1, r3);

            assembly::Comment("branch to " + labelFirstNumberPositive);
            assembly::SET("1" + labelFirstNumberPositive, r6);
            assembly::SET("2" + labelFirstNumberPositive, r7);
            assembly::SET("3" + labelFirstNumberPositive, r8);
            assembly::BRP(r6, r7, r8, r1);
            assembly::NOT(r1, r1);
            assembly::ADD(r1, r3, r1);

            assembly::LabelLatestInstruction(labelFirstNumberPositive);

            assembly::Comment("branch to " + labelSecondNumberPositive);
            assembly::SET("1" + labelSecondNumberPositive, r6);
            assembly::SET("2" + labelSecondNumberPositive, r7);
            assembly::SET("3" + labelSecondNumberPositive, r8);
            assembly::BRP(r6, r7, r8, r2);
            assembly::NOT(r2, r2);
            assembly::ADD(r2, r3, r2);

            assembly::LabelLatestInstruction(labelSecondNumberPositive);

            // Set up registers
            assembly::SET(1, r3);
            assembly::SET(0, r4);
            assembly::SUB(r4, r3, r4);

            // Standard division loop (r1 / r2)
            assembly::LabelLatestInstruction(labelModulusLoop);
            assembly::SUB(r1, r2, r1);
            assembly::ADD(r4, r3, r4);
            assembly::Comment("branch to " + labelModulusLoop);
            assembly::SET("1" + labelModulusLoop, r6);
            assembly::SET("2" + labelModulusLoop, r7);
            assembly::SET("3" + labelModulusLoop, r8);
            assembly::BRP(r6, r7, r8, r1);

            // r1 is now 'r2' below the remainder, let's add r2 to counteract this
            assembly::ADD(r1, r2, r1);

            // If only one register is negative, the result will be negative
            assembly::Comment("branch to " + labelResultPositive);
            assembly::SET("1" + labelResultPositive, r6);
            assembly::SET("2" + labelResultPositive, r7);
            assembly::SET("3" + labelResultPositive, r8);
            assembly::BRP(r6, r7, r8, r5);
            assembly::SET(1, r3);
            assembly::NOT(r1, r1);
            assembly::ADD(r1, r3, r1);
            // Oh, if the result is negative, r1 is NOT r2 below the remainder as stated above
            // Therefore we'll need to undo that addition
            //assembly::SUB(r1, r2, r1);

            assembly::LabelLatestInstruction(labelResultPositive);

            dataValues::Free(r2);
            dataValues::Free(r3);
            dataValues::Free(r4);
            dataValues::Free(r5);
            dataValues::Free(r6);
            dataValues::Free(r7);
            dataValues::Free(r8);
        }

    }
}