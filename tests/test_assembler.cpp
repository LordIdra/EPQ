#include <catch2/catch_test_macros.hpp>

#include <assembler/assembler.hpp>
#include <util/util.hpp>



struct Test { string input; string output; };

TEST_CASE("Invalid size") {
    vector<Test> tests {
        {"BRP 03 090 09", assembler::INVALID_INSTRUCTION_SIZE},
        {"mr bayeh is the GOAT", assembler::INVALID_INSTRUCTION_SIZE},
        {"", assembler::INVALID_INSTRUCTION_SIZE}};
    for (Test t : tests) {
        CAPTURE(t.input, t.output);
        REQUIRE(assembler::Assemble(t.input) == t.output);
    }
}

TEST_CASE("Invalid opcode") {
    vector<Test> tests {
        {"LOL BAYEH", assembler::INVALID_OPCODE},
        {"BYA 02 05", assembler::INVALID_OPCODE}};
    for (Test t : tests) {
        CAPTURE(t.input, t.output);
        REQUIRE(assembler::Assemble(t.input) == t.output);
    }
}

TEST_CASE("Invalid operand count") {
    vector<Test> tests {
        {"ADD 03 05 03 07", assembler::INVALID_OPERAND_COUNT},
        {"NOT 05", assembler::INVALID_OPERAND_COUNT}};
    for (Test t : tests) {
        CAPTURE(t.input, t.output);
        REQUIRE(assembler::Assemble(t.input) == t.output);
    }
}

TEST_CASE("Invalid operand value") {
    vector<Test> tests {
        {"PPC 16", assembler::INVALID_OPERAND},
        {"SET -1 18", assembler::INVALID_OPERAND},
        {"SET 15 -5", assembler::INVALID_OPERAND}, // who in their right mind..?
        {"ADD 02 05 10", assembler::INVALID_OPERAND}};
    for (Test t : tests) {
        CAPTURE(t.input, t.output);
        REQUIRE(assembler::Assemble(t.input) == t.output);
    }
}

TEST_CASE("No arguments") {
    vector<Test> tests {
        {"NOP", "0000 0000 0000 0000 0000"}};
    for (Test t : tests) {
        CAPTURE(t.input, t.output);
        REQUIRE(assembler::Assemble(t.input) == t.output);
    }
}

TEST_CASE("One argument") {
    vector<Test> tests {
        {"PPC 08", "1010 0000 0000 1000 0000"}};
    for (Test t : tests) {
        CAPTURE(t.input, t.output);
        REQUIRE(assembler::Assemble(t.input) == t.output);
    }
}

TEST_CASE("Two argument") {
    vector<Test> tests {
        {"NOT 03 08", "0111 0000 0011 1000 0000"},
        {"MOV 00 09", "1110 0000 0000 1001 0000"},
        {"SET 07 02", "1111 0111 0000 0010 0000"}};
    for (Test t : tests) {
        CAPTURE(t.input, t.output);
        REQUIRE(assembler::Assemble(t.input) == t.output);
    }
}

TEST_CASE("Three arguments") {
    vector<Test> tests {
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
    for (Test t : tests) {
        CAPTURE(t.input, t.output);
        REQUIRE(assembler::Assemble(t.input) == t.output);
    }
}

TEST_CASE("Four arguments") {
    vector<Test> tests {
        {"BRP 03 09 04 09", "1101 0011 1001 0100 1001"}};
    for (Test t : tests) {
        CAPTURE(t.input, t.output);
        REQUIRE(assembler::Assemble(t.input) == t.output);
    }
}