#pragma once

#include "code.cpp"
#include "generator/assembly.hpp"
#include "generator/dataValues.hpp"



namespace generator {
    namespace Term_EQUALS {
        auto Last() -> void {
            // Term_EQUALS -> NONE
            if (node->children.size() == 1) { return; }

            // Term_EQUALS -> EQUALS Term_3 Term_EQUALS

            const int r3 = dataValues::Allocate();
            const int r4 = dataValues::Allocate();

            const int r1 = PopValue();
            const int r2 = PopValue();

            const string labelMSBTheSame = assembly::GenerateLabel("MSBTheSame");
            const string labelNotEqual   = assembly::GenerateLabel("NotEqual");
            const string labelEqual      = assembly::GenerateLabel("Equal");
            const string labelEnd        = assembly::GenerateLabel("End");

            // If the MSB of both r2 and r3 are the same (result of XOR is positive), we can just focus on the 3 other bits
            assembly::XOR(r1, r2, r3);
            assembly::BRP(labelMSBTheSame, r3);
            assembly::BRA(labelNotEqual);

            // MSBs are the same
            // First, subtract r2 from r1
            assembly::Label(labelMSBTheSame);
            assembly::SUB(r1, r2, r3);

            // We know that if they are equal, they will either be 1000 or 0000
            // Let's first set the MSB to 0 by running and AND operation with 0111
            assembly::SET(7, r4);
            assembly::AND(r3, r4, r3);
            assembly::SET(1, r4);

            // Now, if we subtract 0001, the result will only be negative if we started with 0000
            assembly::SUB(r3, r4, r3);

            // If the result is positive, they are not equal
            assembly::BRP(labelNotEqual, r3);

            // We haven't branched, so they are equal
            assembly::SET(dataValues::TRUE, r3);
            assembly::BRA(labelEnd);

            // This is where we branch to if they are not equal
            assembly::Label(labelNotEqual);
            assembly::SET(dataValues::FALSE, r3);

            // And this is the end of the whole thing
            assembly::Label(labelEnd);
            assembly::NOP();

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r4);
            PushValue(r3);
        }

    }

    namespace Term_NOT_EQUALS {
        auto Last() -> void {
            // Term_NOT_EQUALS -> NONE
            if (node->children.size() == 1) { return; }

            // Term_NOT_EQUALS -> NOT_EQUALS Term_4 Term_NOT_EQUALS
            const int r3 = dataValues::Allocate();
            const int r4 = dataValues::Allocate();

            const int r1 = PopValue();
            const int r2 = PopValue();

            const string labelMSBTheSame = assembly::GenerateLabel("MSBTheSame");
            const string labelNotEqual   = assembly::GenerateLabel("NotEqual");
            const string labelEqual      = assembly::GenerateLabel("Equal");
            const string labelEnd        = assembly::GenerateLabel("End");

            // If the MSB of both r2 and r3 are the same (result of XOR is positive), we can just focus on the 3 other bits
            assembly::XOR(r1, r2, r3);
            assembly::BRP(labelMSBTheSame, r3);
            assembly::BRA(labelNotEqual);

            // MSBs are the same
            // First, subtract r2 from r1
            assembly::Label(labelMSBTheSame);
            assembly::SUB(r1, r2, r3);

            // We know that if they are equal, they will either be 1000 or 0000
            // Let's first set the MSB to 0 by running and AND operation with 0111
            assembly::SET(7, r4);
            assembly::AND(r3, r4, r3);

            // Now, if we subtract 0001, the result will only be negative if we started with 0000
            assembly::SET(1, r4);
            assembly::SUB(r3, r4, r3);

            // If the result is positive, they are not equal
            assembly::BRP(labelNotEqual, r3);

            // We haven't branched, so they are equal
            assembly::SET(dataValues::FALSE, r3);
            assembly::BRA(labelEnd);

            // This is where we branch to if they are not equal
            assembly::Label(labelNotEqual);
            assembly::SET(dataValues::TRUE, r3);

            // And this is the end of the whole thing
            assembly::Label(labelEnd);

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r4);
            PushValue(r3);
        }
    }

    namespace Term_GREATER {
        auto Last() -> void {
            // Term_GREATER -> NONE
            if (node->children.size() == 1) { return; }

            const int r3 = dataValues::Allocate();
            const int r4 = dataValues::Allocate();

            // We're checking if r1 > r2 but the order is inverted when we pop
            const int r2 = PopValue();
            const int r1 = PopValue();

            const string labelMSBTheSame      = assembly::GenerateLabel("MSBTheSame");
            const string labelR1Greater       = assembly::GenerateLabel("R1Greater");
            const string labelR1Lower         = assembly::GenerateLabel("R1Lower");
            const string labelEnd             = assembly::GenerateLabel("End");

            // If the MSB of both r2 and r3 are the same (result of XOR is positive), we can just focus on the 3 other bits
            assembly::XOR(r1, r2, r3);
            assembly::BRP(labelMSBTheSame, r3);

            // We haven't branched, so MSBs are different
            // if r1 is positive, r1 > r2
            assembly::BRP(labelR1Greater, r1);
            assembly::BRA(labelR1Lower);

            // MSBs are the same; let's compare the three remaining bits
            // Let's first set the MSB of both bits to 0 by running and AND operation with 0111
            assembly::Label(labelMSBTheSame);
            assembly::SET(7, r4);
            assembly::AND(r1, r4, r1);
            assembly::AND(r2, r4, r2);

            // Now let's subtract r2 from r1
            assembly::SUB(r1, r2, r1);

            // Subtract a further 1 so that the result is negative (false) if the two numbers are equal
            assembly::SET(1, r4);
            assembly::SUB(r1, r4, r1);

            // If the result is positive, r1 > r2
            assembly::BRP(labelR1Greater, r1);
            assembly::BRA(labelR1Lower);

            // r1 > r2
            assembly::Label(labelR1Greater);
            assembly::SET(dataValues::TRUE, r3);
            assembly::BRA(labelEnd);

            // r1 < r2
            assembly::Label(labelR1Lower);
            assembly::SET(dataValues::FALSE, r3);

            // end
            assembly::Label(labelEnd);

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r4);
            PushValue(r3);
        }
    }

    namespace Term_GREATER_OR_EQUAL {
        auto Last() -> void {
            // Term_GREATER_OR_EQUAL -> NONE
            if (node->children.size() == 1) { return; }

            const int r3 = dataValues::Allocate();
            const int r4 = dataValues::Allocate();

            // We're checking if r1 > r2 but the order is inverted when we pop
            const int r2 = PopValue();
            const int r1 = PopValue();

            const string labelMSBTheSame      = assembly::GenerateLabel("MSBTheSame");
            const string labelR1Greater       = assembly::GenerateLabel("R1Greater");
            const string labelR1Lower         = assembly::GenerateLabel("R1Lower");
            const string labelEnd             = assembly::GenerateLabel("End");

            // If the MSB of both r2 and r3 are the same (result of XOR is positive), we can just focus on the 3 other bits
            assembly::XOR(r1, r2, r3);
            assembly::BRP(labelMSBTheSame, r3);

            // We haven't branched, so MSBs are different
            // if r1 is positive, r1 > r2
            assembly::BRP(labelR1Greater, r1);
            assembly::BRA(labelR1Lower);

            // MSBs are the same; let's compare the three remaining bits
            // Let's first set the MSB of both bits to 0 by running and AND operation with 0111
            assembly::Label(labelMSBTheSame);
            assembly::SET(7, r4);
            assembly::AND(r1, r4, r1);
            assembly::AND(r2, r4, r2);

            // Now let's subtract r2 from r1
            assembly::SUB(r1, r2, r1);

            // If the result is positive, r1 > r2
            assembly::BRP(labelR1Greater, r1);
            assembly::BRA(labelR1Lower);

            // r1 > r2
            assembly::Label(labelR1Greater);
            assembly::SET(dataValues::TRUE, r3);
            assembly::BRA(labelEnd);

            // r1 < r2
            assembly::Label(labelR1Lower);
            assembly::SET(dataValues::FALSE, r3);

            // end
            assembly::Label(labelEnd);

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r4);
            PushValue(r3);
        }
    }

    namespace Term_LESS {
        auto Last() -> void {
            // Term_LESS -> NONE
            if (node->children.size() == 1) { return; }

            const int r3 = dataValues::Allocate();
            const int r4 = dataValues::Allocate();

            // We're checking if r1 > r2 but the order is inverted when we pop
            const int r2 = PopValue();
            const int r1 = PopValue();

            const string labelMSBTheSame      = assembly::GenerateLabel("MSBTheSame");
            const string labelR1Greater       = assembly::GenerateLabel("R1Greater");
            const string labelR1Lower         = assembly::GenerateLabel("R1Lower");
            const string labelEnd             = assembly::GenerateLabel("End");

            // If the MSB of both r2 and r3 are the same (result of XOR is positive), we can just focus on the 3 other bits
            assembly::XOR(r1, r2, r3);
            assembly::Comment("branch to " + labelMSBTheSame);
            assembly::BRP(labelMSBTheSame, r3);

            // We haven't branched, so MSBs are different
            // if r1 is positive, r1 > r2
            assembly::BRP(labelR1Greater, r1);
            assembly::BRA(labelR1Lower);

            // MSBs are the same; let's compare the three remaining bits
            // Let's first set the MSB of both bits to 0 by running and AND operation with 0111
            assembly::Label(labelMSBTheSame);
            assembly::SET(7, r4);
            assembly::AND(r1, r4, r1);
            assembly::AND(r2, r4, r2);

            // Now let's subtract r2 from r1
            assembly::SUB(r1, r2, r1);

            // If the result is positive, r1 > r2
            assembly::BRP(labelR1Greater, r1);
            assembly::BRA(labelR1Lower);

            // r1 > r2
            assembly::Label(labelR1Greater);
            assembly::SET(dataValues::FALSE, r3);
            assembly::BRA(labelEnd);

            // r1 < r2
            assembly::Label(labelR1Lower);
            assembly::SET(dataValues::TRUE, r3);

            // end
            assembly::Label(labelEnd);

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r4);
            PushValue(r3);
        }
    }

    namespace Term_LESS_OR_EQUAL {
        auto Last() -> void {
            // Term_LESS_OR_EQUAL -> NONE
            if (node->children.size() == 1) { return; }

            const int r3 = dataValues::Allocate();
            const int r4 = dataValues::Allocate();

            // We're checking if r1 > r2 but the order is inverted when we pop
            const int r2 = PopValue();
            const int r1 = PopValue();

            const string labelMSBTheSame      = assembly::GenerateLabel("MSBTheSame");
            const string labelR1Greater       = assembly::GenerateLabel("R1Greater");
            const string labelR1Lower         = assembly::GenerateLabel("R1Lower");
            const string labelEnd             = assembly::GenerateLabel("End");

            // If the MSB of both r2 and r3 are the same (result of XOR is positive), we can just focus on the 3 other bits
            assembly::XOR(r1, r2, r3);
            assembly::BRP(labelMSBTheSame, r3);

            // We haven't branched, so MSBs are different
            // if r1 is positive, r1 > r2
            assembly::BRP(labelR1Greater, r1);
            assembly::BRA(labelR1Lower);

            // MSBs are the same; let's compare the three remaining bits
            // Let's first set the MSB of both bits to 0 by running and AND operation with 0111
            assembly::Label(labelMSBTheSame);
            assembly::SET(7, r4);
            assembly::AND(r1, r4, r1);
            assembly::AND(r2, r4, r2);

            // Now let's subtract r2 from r1
            assembly::SUB(r1, r2, r1);

            // Subtract a further 1 so that the result is negative if the two numbers are equal
            assembly::SET(1, r4);
            assembly::SUB(r1, r4, r1);

            // If the result is positive, r1 > r2
            assembly::BRP(labelR1Greater, r1);
            assembly::BRA(labelR1Lower);

            // r1 > r2
            assembly::Label(labelR1Greater);
            assembly::SET(dataValues::FALSE, r3);
            assembly::BRA(labelEnd);

            // r1 < r2
            assembly::Label(labelR1Lower);
            assembly::SET(dataValues::TRUE, r3);

            // end
            assembly::Label(labelEnd);

            dataValues::Free(r1);
            dataValues::Free(r2);
            dataValues::Free(r4);
            PushValue(r3);
        }
    }
}