#pragma once

#include <unordered_map>
#include <util/types.hpp>



namespace assembler {
    const int WORD_SIZE = 4;
    const int LENGTH_WITH_0_OPERANDS = 3;
    const int LENGTH_WITH_1_OPERANDS = 6;
    const int LENGTH_WITH_2_OPERANDS = 9;
    const int LENGTH_WITH_3_OPERANDS = 12;
    const int LENGTH_WITH_4_OPERANDS = 15;

    const int MAX_OPERAND_REGISTER = 9;
    const int MAX_OPERAND_VALUE = 15;

    const int MACHINE_CODE_OPERAND_COUNT = 4;

    enum OperandType {
        REGISTER,   // 00 to 09 inclusive
        VALUE       // 00 to 15 inclusive
    };

    const unordered_map<string, int> opcodes = {
        {"NOP", 0}, 
        {"LDA", 1}, 
        {"STA", 2}, 
        {"ADD", 3}, 
        {"ADC", 3}, 
        {"SUB", 3},
        {"SBC", 3},
        {"AND", 4}, 
        {"ORR", 5}, 
        {"XOR", 6}, 
        {"NOT", 7}, 
        {"PSH", 8}, 
        {"POP", 9}, 
        {"PPC", 10}, 
        {"RET", 11},
        {"BRA", 12},
        {"BRP", 13}, 
        {"MOV", 14}, 
        {"SET", 15}
    };

    const unordered_map<string, vector<OperandType>> operand_types = {
        {"NOP", {}},
        {"LDA", {REGISTER, REGISTER, REGISTER}},
        {"STA", {REGISTER, REGISTER, REGISTER}},
        {"ADD", {REGISTER, REGISTER, REGISTER}},
        {"SUB", {REGISTER, REGISTER, REGISTER}},
        {"ADC", {REGISTER, REGISTER, REGISTER}},
        {"SBC", {REGISTER, REGISTER, REGISTER}},
        {"AND", {REGISTER, REGISTER, REGISTER}},
        {"ORR", {REGISTER, REGISTER, REGISTER}},
        {"XOR", {REGISTER, REGISTER, REGISTER}},
        {"NOT", {REGISTER, REGISTER}},
        {"PSH", {}},
        {"POP", {}},
        {"PPC", {VALUE}},
        {"RET", {}},
        {"BRA", {REGISTER, REGISTER, REGISTER}},
        {"BRP", {REGISTER, REGISTER, REGISTER, REGISTER}},
        {"MOV", {REGISTER, REGISTER}},
        {"SET", {VALUE, REGISTER}},
    };
    
    // Each element of the vector maps the index of that part of the machine code to the index of the operand
    // -1 means no operand associated (0000)
    const unordered_map<string, array<int, 4>> operand_positions= {
        {"NOP", {-1, -1, -1, -1}},
        {"LDA", {0, 1, 2, -1}},
        {"STA", {0, 1, 2, -1}},
        {"ADD", {0, 1, 2, -1}},
        {"SUB", {0, 1, 2, -1}},
        {"ADC", {0, 1, 2, -1}},
        {"SBC", {0, 1, 2, -1}},
        {"AND", {0, 1, 2, -1}},
        {"ORR", {0, 1, 2, -1}},
        {"XOR", {0, 1, 2, -1}},
        {"NOT", {-1, 0, 1, -1}},
        {"PSH", {-1, -1, -1, -1}},
        {"POP", {-1, -1, -1, -1}},
        {"PPC", {-1, -1, 0, -1}},
        {"RET", {-1, -1, -1, -1}},
        {"BRA", {0, 1, 2, -1}},
        {"BRP", {0, 1, 2, 3}},
        {"MOV", {0, -1, 1, -1}},
        {"SET", {0, -1, 1, -1}},
    };

    auto Assemble(const string &instruction) -> string;
}