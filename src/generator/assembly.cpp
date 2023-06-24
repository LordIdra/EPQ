#include "generator/dataValue.hpp"
#include "generator/memory.hpp"
#include <cstdlib>
#include <generator/assembly.hpp>

#include <iostream>
#include <string>
#include <unordered_map>
#include <util/types.hpp>
#include <util/util.hpp>

#include <generator/dataValues.hpp>
#include <generator/generator.hpp>


namespace assembly {
    namespace {
        unordered_map<string, int> labels;
        unordered_map<string, int> labelCount;
        unordered_map<int, string> comments;
        vector<string> program;

        const char COMMENT_CHARACTER = ';';

        auto ResolveDataValue(const int id) -> string {
            const int register_ = dataValues::Load(id);
            return FormatValue(register_);
        }
    }

    auto Reset() -> void {
        labels.clear();
        labelCount.clear();
        comments.clear();
        program.clear();
    }

    auto GenerateLabel(const string &identifier) -> string {
        // Ensures there are no duplicate labels by adding a number suffix to each label
        labelCount[identifier]++;
        return identifier + "_" + std::to_string(labelCount.at(identifier));
    }

    auto Label(const string &label) -> void {
        labels.insert(std::make_pair(label, program.size()));
    }

    auto ResolveLabels() -> void {
        for (string &instruction : program) {

            const int i1 = instruction.find('@');
            const int i2 = instruction.find('%');
            if (i1 != string::npos) {

                const int valueType = std::stoi(instruction.substr(i1+1, 1));
                const string identifier = instruction.substr(i1+2, i2-i1-2);
                const int address = labels.at(identifier) - 1;
                int value = 0;

                if (valueType == 3) {
                    value = div(address, 256).quot;
                } else if (valueType == 2) {
                    value = div(div(address, 256).rem, 16).quot;
                } else if (valueType == 1) {
                    value = div(div(address, 256).rem, 16).rem;
                }

                instruction = instruction.substr(0, i1)
                            + FormatValue(value)
                            + instruction.substr(i2+1, instruction.size()-i2-1);
            }
        }
    }

    auto GetProgram() -> const vector<string>& {
        return program;
    }

    auto GetComments() -> const unordered_map<int, string>& {
        return comments;
    }

    auto NOP() -> void {
        program.emplace_back("NOP");
    }

    auto LDA(const int r1, const int r2, const int r3) -> void {
        program.push_back("LDA " + ResolveDataValue(r1) + " " + ResolveDataValue(r2) + " " + ResolveDataValue(r3));
    }

    auto LDA(const Register r1, const Register r2, const Register r3) -> void {
        program.push_back("LDA " + FormatValue(r1) + " " + FormatValue(r2) + " " + FormatValue(r3));
    }

    auto LDA(const string &identifier) -> void {
        const int address = generator::GetIdentifierAddress(identifier);

        const int a1 = div(address, 256).quot;
        const int a2 = div(div(address, 256).rem, 16).quot;
        const int a3 = div(div(address, 256).rem, 16).rem;

        assembly::Comment("load " + identifier);
        SET(a1, MDR_1);
        SET(a2, MDR_2);
        SET(a3, MER_1);
        LDA(MDR_1, MDR_2, MER_1);
    }

    auto STA(const int r1, const int r2, const int r3) -> void {
        program.push_back("STA " + ResolveDataValue(r1) + " " + ResolveDataValue(r2) + " " + ResolveDataValue(r3));
    }

    auto STA(const Register r1, const int r2, const int r3) -> void {
        program.push_back("STA " + FormatValue(r1) + " " + ResolveDataValue(r2) + " " + ResolveDataValue(r3));
    }

    auto STA(const string &identifier) -> void {
        const int address = generator::GetIdentifierAddress(identifier);

        const int a1 = div(address, 256).quot;
        const int a2 = div(div(address, 256).rem, 16).quot;
        const int a3 = div(div(address, 256).rem, 16).rem;

        const int r1 = dataValues::Allocate();
        const int r2 = dataValues::Allocate();

        assembly::Comment("store " + identifier);
        SET(a1, MER_1);
        SET(a2, r1);
        SET(a3, r2);
        STA(MER_1, r1, r2);

        dataValues::Free(r1);
        dataValues::Free(r2);
    }

    auto ADD(const int r1, const int r2, const int r3) -> void {
        program.push_back("ADD " + ResolveDataValue(r1) + " " + ResolveDataValue(r2) + " " + ResolveDataValue(r3));
    }

    auto ADC(const int r1, const int r2, const int r3) -> void {
        program.push_back("ADC " + ResolveDataValue(r1) + " " + ResolveDataValue(r2) + " " + ResolveDataValue(r3));
    }

    auto SUB(const int r1, const int r2, const int r3) -> void {
        program.push_back("SUB " + ResolveDataValue(r1) + " " + ResolveDataValue(r2) + " " + ResolveDataValue(r3));
    }

    auto SBC(const int r1, const int r2, const int r3) -> void {
        program.push_back("SBC " + ResolveDataValue(r1) + " " + ResolveDataValue(r2) + " " + ResolveDataValue(r3));
    }

    auto AND(const int r1, const int r2, const int r3) -> void {
        program.push_back("AND " + ResolveDataValue(r1) + " " + ResolveDataValue(r2) + " " + ResolveDataValue(r3));
    }

    auto ORR(const int r1, const int r2, const int r3) -> void {
        program.push_back("ORR " + ResolveDataValue(r1) + " " + ResolveDataValue(r2) + " " + ResolveDataValue(r3));
    }

    auto XOR(const int r1, const int r2, const int r3) -> void {
        program.push_back("XOR " + ResolveDataValue(r1) + " " + ResolveDataValue(r2) + " " + ResolveDataValue(r3));
    }

    auto NOT(const int r1, const int r2) -> void {
        program.push_back("NOT " + ResolveDataValue(r1) + " " + ResolveDataValue(r2));
    }

    auto PSH() -> void {
        program.emplace_back("PSH");
    }

    auto POP() -> void {
        program.emplace_back("POP");
    }

    auto PPC(const int v1) -> void {
        program.push_back("PPC " + FormatValue(v1));
    }

    auto RET() -> void {
        program.emplace_back("RET");
    }

    auto BRA(const int r1, const int r2, const int r3) -> void {
        program.push_back("BRA " + ResolveDataValue(r1) + " " + ResolveDataValue(r2) + " " + ResolveDataValue(r3));
    }

    auto BRA(const Register r1, const Register r2, const Register r3) -> void {
        program.push_back("BRA " + FormatValue(r1) + " " + FormatValue(r2) + " " + FormatValue(r3));
    }

    auto BRA(const string &label) -> void {
        assembly::Comment("branch to " + label);
        SET("1" + label, MDR_1);
        SET("2" + label, MDR_2);
        SET("3" + label, MER_1);
        BRA(MDR_1, MDR_2, MER_1);
    }

    auto BRP(const int r1, const int r2, const int r3, const int r4) -> void {
        program.push_back("BRP " + ResolveDataValue(r1) + " " + ResolveDataValue(r2) + " " + ResolveDataValue(r3) + " " + ResolveDataValue(r4));
    }

    auto BRP(const Register r1, const Register r2, const Register r3, const int r4) -> void {
        program.push_back("BRP " + FormatValue(r1) + " " + FormatValue(r2) + " " + FormatValue(r3) + " " + ResolveDataValue(r4));
    }

    auto BRP(const string &label, const int r1) -> void {
        assembly::Comment("branch to " + label);
        SET("1" + label, MDR_1);
        SET("2" + label, MDR_2);
        SET("3" + label, MER_1);
        BRP(MDR_1, MDR_2, MER_1, r1);
    }

    auto MOV(const int r1, const int r2) -> void {
        program.push_back("MOV " + ResolveDataValue(r1) + " " + ResolveDataValue(r2));
    }

    auto MOV(const int r1, const Register r2) -> void {
        program.push_back("MOV " + ResolveDataValue(r1) + " " + FormatValue(r2));
    }

    auto MOV(const Register r1, const int r2) -> void {
        program.push_back("MOV " + FormatValue(r1) + " " + ResolveDataValue(r2));
    }

    auto MOV(const Register r1, const Register r2) -> void {
        program.push_back("MOV " + FormatValue(r1) + " " + FormatValue(r2));
    }

    auto SET(const int v1, const int r1) -> void {
        program.push_back("SET " + FormatValue(v1) + " " + ResolveDataValue(r1));
    }

    auto SET(const int v1, const Register r1) -> void {
        program.push_back("SET " + FormatValue(v1) + " " + FormatValue(r1));
    }

    auto SET(const string &label, const int r1) -> void {
        program.push_back("SET " "@" + label + "%" + " " + ResolveDataValue(r1));
    }

    auto SET(const string &label, const Register r1) -> void {
        program.push_back("SET " "@" + label + "%" + " " + FormatValue(r1));
    }

    auto Comment(const string &comment) -> void {
        comments.insert(std::make_pair(program.size(),  ";" + comment));
    }
}