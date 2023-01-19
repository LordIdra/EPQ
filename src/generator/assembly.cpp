#include "generator/memory.hpp"
#include "semanticAnalyser/symbolTable.hpp"
#include <cstdlib>
#include <generator/assembly.hpp>

#include <iostream>
#include <string>
#include <unordered_map>
#include <util/types.hpp>

#include <generator/registers.hpp>


namespace assembly {
    namespace {
        std::unordered_map<string, int> labels;
        vector<string> program;

        std::unordered_map<string, int> labelCount;

        auto SplitAddress(const int address, int &a1, int &a2, int &a3) -> void {
            a1 = div(address, 256).quot;
            a2 = div(address - (a1*256), 16).quot;
            a3 = address - (a1*256) - (a2*16);
        }

        auto FormatRegister(const int x) -> string {
            if (x <= 9) {
                return "0" + std::to_string(x);
            }
            return std::to_string(x);
        }
    }

    auto GenerateLabel(const string &identifier) -> string {
        // Ensures there are no duplicate labels by adding a number suffix to each label
        labelCount[identifier]++;
        return identifier + "_" + std::to_string(labelCount.at(identifier));
    }

    auto LabelLatestInstruction(const string &label) -> void {
        labels.insert(std::make_pair(label, program.size() - 1));
    }

    auto ResolveLabels() -> void {
        for (string &instruction : program) {

            const int i1 = instruction.find('@');
            const int i2 = instruction.find('%');
            if (i1 != string::npos) {


                const int valueType = std::stoi(instruction.substr(i1+1, 1));
                const string identifier = instruction.substr(i1+2, i2-i1-2);
                const int address = labels.at(identifier);
                int value = 0;

                if (valueType == 3) {
                    value = div(address, 256).quot;
                } else if (valueType == 2) {
                    value = div(div(address, 256).rem, 16).quot;
                } else if (valueType == 1) {
                    value = div(div(address, 256).rem, 16).rem;
                }

                std::cout << valueType << " " << address << "\n";
                
                instruction = instruction.substr(0, i1)
                            + FormatRegister(value)
                            + instruction.substr(i2+1, instruction.size()-i2-1);
            }
        }
    }

    auto GetProgram() -> vector<string> {
        return program;
    }
    
    auto NOP() -> void {
        program.emplace_back("NOP");
    }

    auto LDA(const int r1, const int r2, const int r3) -> void {
        program.push_back("LDA " + FormatRegister(r1) + " " + FormatRegister(r2) + " " + FormatRegister(r3));
    }

    auto STA(const int r1, const int r2, const int r3) -> void {
        program.push_back("STA " + FormatRegister(r1) + " " + FormatRegister(r2) + " " + FormatRegister(r3));
    }

    auto ADD(const int r1, const int r2, const int r3) -> void {
        program.push_back("ADD " + FormatRegister(r1) + " " + FormatRegister(r2) + " " + FormatRegister(r3));
    }

    auto ADC(const int r1, const int r2, const int r3) -> void {
        program.push_back("ADC " + FormatRegister(r1) + " " + FormatRegister(r2) + " " + FormatRegister(r3));
    }

    auto SUB(const int r1, const int r2, const int r3) -> void {
        program.push_back("SUB " + FormatRegister(r1) + " " + FormatRegister(r2) + " " + FormatRegister(r3));
    }

    auto SBC(const int r1, const int r2, const int r3) -> void {
        program.push_back("SBC " + FormatRegister(r1) + " " + FormatRegister(r2) + " " + FormatRegister(r3));
    }

    auto AND(const int r1, const int r2, const int r3) -> void {
        program.push_back("AND " + FormatRegister(r1) + " " + FormatRegister(r2) + " " + FormatRegister(r3));
    }

    auto ORR(const int r1, const int r2, const int r3) -> void {
        program.push_back("ORR " + FormatRegister(r1) + " " + FormatRegister(r2) + " " + FormatRegister(r3));
    }

    auto XOR(const int r1, const int r2, const int r3) -> void {
        program.push_back("XOR " + FormatRegister(r1) + " " + FormatRegister(r2) + " " + FormatRegister(r3));
    }

    auto NOT(const int r1, const int r2) -> void {
        program.push_back("NOT " + FormatRegister(r1) + " " + FormatRegister(r2));
    }

    auto PSH() -> void {
        program.emplace_back("PSH");
    }

    auto POP() -> void {
        program.emplace_back("POP");
    }

    auto PPC(const int v1) -> void {
        program.push_back("PPC " + FormatRegister(v1));
    }

    auto RET() -> void {
        program.emplace_back("RET");
    }

    auto BRA(const int r1, const int r2, const int r3) -> void {
        program.push_back("BRA " + FormatRegister(r1) + " " + FormatRegister(r2) + " " + FormatRegister(r3));
    }

    auto BRP(const int r1, const int r2, const int r3, const int r4) -> void {
        program.push_back("BRP " + FormatRegister(r1) + " " + FormatRegister(r2) + " " + FormatRegister(r3) + " " + FormatRegister(r4));
    }

    auto MOV(const int r1, const int r2) -> void {
        program.push_back("MOV " + FormatRegister(r1) + " " + FormatRegister(r2));
    }

    auto SET(const int v1, const int r1) -> void {
        program.push_back("SET " + FormatRegister(v1) + " " + FormatRegister(r1));
    }

    auto SET(const string &identifier, const int r1) -> void {
        program.push_back("SET " "@" + identifier + "%" + " " + FormatRegister(r1));
    }

    auto Comment(const string &comment) -> void {
        if (!program.empty()) {
            if (program.at(program.size() - 1).at(0) == ';') {
                program.pop_back();
            }
        }
        program.push_back(";" + comment);
    }
}