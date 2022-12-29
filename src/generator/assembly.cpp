#include <cstdlib>
#include <generator/assembly.hpp>

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
    }

    auto GenerateLabel(const string &identifier) -> string {
        // Ensures there are no duplicate labels by adding a number suffix to each label
        labelCount[identifier]++;
        return identifier + "_" + std::to_string(labelCount.at(identifier));
    }

    auto LabelLatestInstruction(const string &label) -> void {
        labels.insert(std::make_pair(GenerateLabel(label), program.size() - 1));
    }

    auto GetProgram() -> vector<string> {
        return program;
    }
    
    auto NOP() -> void {
        program.push_back("NOP");
    }

    auto LDA(const string &identifier) -> void {
        program.push_back("LDA @" + identifier);
    }

    auto STA(const string &identifier) -> void {
        program.push_back("STA @" + identifier);
    }

    auto ADD(const int r1, const int r2, const int r3) -> void {
        program.push_back("ADD $" + std::to_string(r1) + " $" + std::to_string(r2) + " $" + std::to_string(r3));
    }

    auto ADC(const int r1, const int r2, const int r3) -> void {
        program.push_back("ADC $" + std::to_string(r1) + " $" + std::to_string(r2) + " $" + std::to_string(r3));
    }

    auto SUB(const int r1, const int r2, const int r3) -> void {
        program.push_back("SUB $" + std::to_string(r1) + " $" + std::to_string(r2) + " $" + std::to_string(r3));
    }

    auto SBC(const int r1, const int r2, const int r3) -> void {
        program.push_back("SBC $" + std::to_string(r1) + " $" + std::to_string(r2) + " $" + std::to_string(r3));
    }

    auto AND(const int r1, const int r2, const int r3) -> void {
        program.push_back("AND $" + std::to_string(r1) + " $" + std::to_string(r2) + " $" + std::to_string(r3));
    }

    auto ORR(const int r1, const int r2, const int r3) -> void {
        program.push_back("ORR $" + std::to_string(r1) + " $" + std::to_string(r2) + " $" + std::to_string(r3));
    }

    auto XOR(const int r1, const int r2, const int r3) -> void {
        program.push_back("XOR $" + std::to_string(r1) + " $" + std::to_string(r2) + " $" + std::to_string(r3));
    }

    auto NOT(const int r1, const int r2) -> void {
        program.push_back("NOT $" + std::to_string(r1) + " $" + std::to_string(r2));
    }

    auto PSH() -> void {
        program.push_back("PSH");
    }

    auto POP() -> void {
        program.push_back("POP");
    }

    auto PPC(const int v1) -> void {
        program.push_back("PPC #" + std::to_string(v1));
    }

    auto RET() -> void {
        program.push_back("RET");
    }

    auto BRA(const string &label) -> void {
        program.push_back("BRA @" + label);
    }

    auto BRP(const string &label, const int r1) -> void {
        program.push_back("BRP @" + label + " #" + std::to_string(r1));
    }

    auto MOV(const int r1, const int r2) -> void {
        program.push_back("MOV $" + std::to_string(r1) + " $" + std::to_string(r2));
    }

    auto SET(const int v1, const int r1) -> void {
        program.push_back("SET #" + std::to_string(v1) + " $" + std::to_string(r1));
    }
}