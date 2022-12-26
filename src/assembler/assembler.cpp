#include "assembler.hpp"
#include "util/errors.hpp"

#include <util/util.hpp>

#include <algorithm>
#include <iostream>



namespace assembler {

    namespace Detail {
        auto IsValidInstructionSize(const int size) -> bool {
            return (size == LENGTH_WITH_0_OPERANDS) || (size == LENGTH_WITH_1_OPERANDS) || (size == LENGTH_WITH_2_OPERANDS) || 
                   (size == LENGTH_WITH_3_OPERANDS) || (size == LENGTH_WITH_4_OPERANDS);
        }

        auto IsValidOpcode(const string &opcode_string) -> bool {
            return opcodes.find(opcode_string) != opcodes.end();
        }

        auto IsValidOperand(const int &operand, OperandType type) -> bool {
            if (operand < 0) {
                return false;
            }

            if (type == REGISTER) {
                return operand <= MAX_OPERAND_REGISTER;
            }

            if (type == VALUE) {
                return operand <= MAX_OPERAND_VALUE;
            }

            // Should never be triggered
            return false;
        }

        auto IsArithmeticInstruction(const string &opcode_string) -> bool {
            return (opcode_string == "ADD") || (opcode_string == "SUB") || (opcode_string == "ADC") || (opcode_string == "SBC");
        }

        auto GetFourthOperand(const string &opcode_string) -> string {
            if (opcode_string == "ADD") { return "0000"; }
            if (opcode_string == "ADC") { return "0001"; }
            if (opcode_string == "SUB") { return "0010"; }
            if (opcode_string == "SBC") { return "0011"; }

            // Should never be triggered
            return "ERROR";
        }

        auto HandleOperand(vector<string> &operand_strings, const string &instruction, const int length) -> void {
            if (instruction.size() >= length) { operand_strings.push_back(instruction.substr(length-2, 2)); }
        }

        auto GetOperandStrings(const string &instruction) -> vector<string> {
            vector<string> operand_strings;
            HandleOperand(operand_strings, instruction, LENGTH_WITH_1_OPERANDS);
            HandleOperand(operand_strings, instruction, LENGTH_WITH_2_OPERANDS);
            HandleOperand(operand_strings, instruction, LENGTH_WITH_3_OPERANDS);
            HandleOperand(operand_strings, instruction, LENGTH_WITH_4_OPERANDS);
            return operand_strings;
        }

        auto OperandCountValid(const string &opcode_string, const vector<string> &operand_strings) -> bool {
            return operand_strings.size() == operand_types.at(opcode_string).size();
        }

        auto OperandsAsIntegers(const vector<string> &operand_strings) -> vector<int> {
            vector<int> operand_ints;
            operand_ints.reserve(operand_strings.size());
            for (const string &operand_string : operand_strings) {
                operand_ints.push_back(std::stoi(operand_string));
            }
            return operand_ints;
        }

        auto OperandsAsBinary(const vector<int> &operand_ints) -> vector<string> {
            vector<string> operand_binary;
            operand_binary.reserve(operand_ints.size());
            for (const int &operand_int : operand_ints) {
                operand_binary.push_back(DenaryToBinary(operand_int, WORD_SIZE));
            }
            return operand_binary;
        }

        auto OperandsValid(const string &opcode, const vector<int> &operand_ints) -> bool {
            for (int i = 0; i < operand_ints.size(); i++) {
                if (!IsValidOperand(operand_ints.at(i), operand_types.at(opcode).at(i))) {
                    return false;
                }
            }
            return true;
        }

        auto BuildMachineCode(const string &opcode_string, const vector<string> &operand_binary) -> string {
            // Convert opcode to binary, this will be the start of the code
            int opcode_int = opcodes.at(opcode_string);
            string opcode_binary = DenaryToBinary(opcode_int, WORD_SIZE);
            string machine_code = opcode_binary;

            // For every operand
            for (int i = 0; i < MACHINE_CODE_OPERAND_COUNT; i++) {
                machine_code += " ";
                int position = operand_positions.at(opcode_string).at(i);

                // If the operand we're supposed to get is -1, that just means no operand
                // If it's not -1 however, we need to add the value of the relevant operand
                if (position != -1) {
                    machine_code += operand_binary.at(position);
                    continue;
                }

                // The 4th operand (3rd index) may need to be different for the arithmetic instructions
                if (i == 3) {
                    if (IsArithmeticInstruction(opcode_string)) {
                        machine_code += GetFourthOperand(opcode_string);
                        continue;
                    }
                }

                machine_code += "0000";
            }

            return machine_code;
        }
    }

    auto Assemble(const string &instruction) -> string {
        // Check if the instruciton is a valid length          
        if (!Detail::IsValidInstructionSize(instruction.size())) {
            errors::AddError(errors::INVALID_INSTRUCTION_SIZE, 
                colors::RED + "Invalid instruction size: " + colors::CYAN + instruction + colors::WHITE);
            return "";
        }
        
        // Get opcode string and check opcode is valid
        string opcode_string = instruction.substr(0, 3);
        if (!Detail::IsValidOpcode(opcode_string)) {
            errors::AddError(errors::INVALID_OPCODE, 
                colors::RED + "Invalid opcode: " + colors::CYAN + instruction + colors::WHITE);
            return "";
        }

        // Get operand strings and check there's a valid number of them
        vector<string> operand_strings = Detail::GetOperandStrings(instruction);
        if (!Detail::OperandCountValid(opcode_string, operand_strings)) {
            errors::AddError(errors::INVALID_OPERAND_COUNT, 
                colors::RED + "Invalid operand count: " + colors::CYAN + instruction + colors::WHITE);
            return "";
        }

        // Convert all those operand strings into integers and check they're valid
        vector<int> operand_ints = Detail::OperandsAsIntegers(operand_strings);
        if (!Detail::OperandsValid(opcode_string, operand_ints)) {
            errors::AddError(errors::INVALID_OPERAND, 
                colors::RED + "Invalid operand: " + colors::CYAN + instruction + colors::WHITE);
            return "";
        }

        // Convert integers into binary
        vector<string> operand_binary = Detail::OperandsAsBinary(operand_ints);
        
        return Detail::BuildMachineCode(opcode_string, operand_binary);
    }
    
}