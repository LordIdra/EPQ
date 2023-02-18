#include "util/errors.hpp"
#include <catch2/catch_test_macros.hpp>

#include <assembler/assembler.hpp>
#include <util/util.hpp>



struct ErrorTest   { string input; int errorCode; };
struct SuccessTest { string input; string output; };

TEST_CASE("[2|ASM] Invalid size") {
    vector<ErrorTest> tests {
        {"BRP 03 090 09", errors::INVALID_INSTRUCTION_SIZE},
        {"mr bayeh is the GOAT", errors::INVALID_INSTRUCTION_SIZE},
        {"", errors::INVALID_INSTRUCTION_SIZE}};

    errors::Reset();

    for (const ErrorTest &t : tests) {
        CAPTURE(t.input, t.errorCode);
        REQUIRE(assembler::Assemble(t.input) == "");
        REQUIRE(errors::GetErrorCode() == t.errorCode);
    }
}

TEST_CASE("[2|ASM] Invalid opcode") {
    vector<ErrorTest> tests {
        {"LOL BAYEH", errors::INVALID_OPCODE},
        {"BYA 02 05", errors::INVALID_OPCODE}};

    errors::Reset();

    for (const ErrorTest &t : tests) {
        REQUIRE(assembler::Assemble(t.input).empty());
        REQUIRE(errors::GetErrorCode() == t.errorCode);
    }
}

TEST_CASE("[2|ASM] Invalid operand count") {
    vector<ErrorTest> tests {
        {"ADD 03 05 03 07", errors::INVALID_OPERAND_COUNT},
        {"NOT 05", errors::INVALID_OPERAND_COUNT}};

    errors::Reset();

    for (const ErrorTest &t : tests) {
        REQUIRE(assembler::Assemble(t.input).empty());
        REQUIRE(errors::GetErrorCode() == t.errorCode);
    }
}

TEST_CASE("[2|ASM] Invalid operand value") {
    vector<ErrorTest> tests {
        {"PPC 16", errors::INVALID_OPERAND},
        {"SET -1 18", errors::INVALID_OPERAND},
        {"SET 15 -5", errors::INVALID_OPERAND}, // who in their right mind..?
        {"ADD 02 05 10", errors::INVALID_OPERAND}};

    errors::Reset();

    for (ErrorTest t : tests) {
        REQUIRE(assembler::Assemble(t.input).empty());
        REQUIRE(errors::GetErrorCode() == t.errorCode);
    }
}

TEST_CASE("[2|ASM] No arguments") {
    vector<SuccessTest> tests {
        {"NOP", "0000 0000 0000 0000 0000"}};

    errors::Reset();

    for (SuccessTest t : tests) {
        CAPTURE(t.input, t.output);
        REQUIRE(assembler::Assemble(t.input) == t.output);
        REQUIRE(errors::GetErrorCode() == errors::NONE);
    }
}

TEST_CASE("[2|ASM] One argument") {
    vector<SuccessTest> tests {
        {"PPC 08", "1010 0000 0000 1000 0000"}};

    errors::Reset();

    for (SuccessTest t : tests) {
        CAPTURE(t.input, t.output);
        REQUIRE(assembler::Assemble(t.input) == t.output);
        REQUIRE(errors::GetErrorCode() == errors::NONE);
    }
}

TEST_CASE("[2|ASM] Two argument") {
    vector<SuccessTest> tests {
        {"NOT 03 08", "0111 0000 0011 1000 0000"},
        {"MOV 00 09", "1110 0000 0000 1001 0000"},
        {"SET 07 02", "1111 0111 0000 0010 0000"}};

    errors::Reset();

    for (SuccessTest t : tests) {
        CAPTURE(t.input, t.output);
        REQUIRE(assembler::Assemble(t.input) == t.output);
        REQUIRE(errors::GetErrorCode() == errors::NONE);
    }
}

TEST_CASE("[2|ASM] Three arguments") {
    vector<SuccessTest> tests {
        {"LDA 09 02 04", "0001 1001 0010 0100 0000"},
        {"STA 07 08 03", "0010 0111 1000 0011 0000"},
        {"ADD 06 02 01", "0011 0110 0010 0001 0000"},
        {"ADC 05 09 00", "0011 0101 1001 0000 0001"},
        {"SUB 02 06 05", "0011 0010 0110 0101 0010"},
        {"SBC 06 02 01", "0011 0110 0010 0001 0011"},
        {"AND 01 07 06", "0100 0001 0111 0110 0000"},
        {"ORR 02 00 07", "0101 0010 0000 0111 0000"},
        {"XOR 08 04 06", "0110 1000 0100 0110 0000"},
        {"BRA 03 03 04", "1100 0011 0011 0100 0000"}};
    
    errors::Reset();

    for (SuccessTest t : tests) {
        CAPTURE(t.input, t.output);
        REQUIRE(assembler::Assemble(t.input) == t.output);
        REQUIRE(errors::GetErrorCode() == errors::NONE);
    }
}

TEST_CASE("[2|ASM] Four arguments") {
    vector<SuccessTest> tests {
        {"BRP 03 09 04 09", "1101 0011 1001 0100 1001"}};

    errors::Reset();
    
    for (SuccessTest t : tests) {
        CAPTURE(t.input, t.output);
        REQUIRE(assembler::Assemble(t.input) == t.output);
        REQUIRE(errors::GetErrorCode() == errors::NONE);
    }
}