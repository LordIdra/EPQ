#include "util/errors.hpp"
#include <iostream>
#include <simulator/simulator.hpp>
#include <stack>
#include <unordered_map>
#include <util/util.hpp>


namespace simulator {
    namespace {
        const int REGISTER_SIZE = 10;
        const int MEMORY_SIZE = 2048;
        const int STACK_SIZE = 2048;
        const int FINAL_INSTRUCTION = 2047;
        const int INFINITE_LOOP_HEURISTIC_LIMIT = 100;

        int programCounter = 0;
        int carry1 = 0;
        int carry2 = 0;

        int infiniteLoopHeuristic = 0;
        int cycles = 0;

        array<int, REGISTER_SIZE> registers;
        array<pair<int, int>, MEMORY_SIZE> memory;
        stack<pair<int, int>> stack;

        auto GetArg1(const string &instruction) -> int {
            return std::stoi(instruction.substr(4, 2));
        }

        auto GetArg2(const string &instruction) -> int {
            return std::stoi(instruction.substr(7, 2));
        }

        auto GetArg3(const string &instruction) -> int {
            return std::stoi(instruction.substr(10, 2));
        }

        auto GetArg4(const string &instruction) -> int {
            return std::stoi(instruction.substr(13, 2));
        }

        auto Assign(const int r, int v) -> void {
            while (v >= 16) { v -= 16; }
            while (v < 0) { v += 16; }
            registers.at(r) = v;
        }

        auto NOP(const string &instruction) -> void {}

        auto LDA(const string &instruction) -> void {
            const int a1 = GetArg1(instruction);
            const int a2 = GetArg2(instruction);
            const int a3 = GetArg3(instruction);
            const int address = (registers.at(a1) * 256) + (registers.at(a2) * 16) + (registers.at(a3) * 1);
            registers.at(0) = memory.at(address).first;
            registers.at(1) = memory.at(address).second;
        }

        auto STA(const string &instruction) -> void {
            const int a1 = GetArg1(instruction);
            const int a2 = GetArg2(instruction);
            const int a3 = GetArg3(instruction);
            const int address = (registers.at(a1) * 256) + (registers.at(a2) * 16) + (registers.at(a3) * 1);
            memory.at(address).first  = registers.at(0);
            memory.at(address).second = registers.at(1);
        }
        
        auto ADD(const string &instruction) -> void {
            const int r1 = GetArg1(instruction);
            const int r2 = GetArg2(instruction);
            const int r3 = GetArg3(instruction);
            const int result = registers.at(r1) + registers.at(r2);
            carry1 = result >= 16;
            Assign(r3, result);
        }

        auto ADC(const string &instruction) -> void {
            const int r1 = GetArg1(instruction);
            const int r2 = GetArg2(instruction);
            const int r3 = GetArg3(instruction);
            const int result = registers.at(r1) + registers.at(r2) + carry1;
            carry1 = result >= 16;
            Assign(r3, registers.at(r1) + registers.at(r2));
        }

        auto SUB(const string &instruction) -> void {
            const int r1 = GetArg1(instruction);
            const int r2 = GetArg2(instruction);
            const int r3 = GetArg3(instruction);
            const int result = registers.at(r1) - registers.at(r2);
            carry1 = result < 0;
            Assign(r3, result);
        }

        auto SBC(const string &instruction) -> void {
            const int r1 = GetArg1(instruction);
            const int r2 = GetArg2(instruction);
            const int r3 = GetArg3(instruction);
            const int result = registers.at(r1) - registers.at(r2) - carry1;
            carry1 = result < 0;
            Assign(r3, registers.at(r1) + registers.at(r2));
        }

        auto AND(const string &instruction) -> void {
            const int r1 = GetArg1(instruction);
            const int r2 = GetArg2(instruction);
            const int r3 = GetArg3(instruction);
            Assign(r3, registers.at(r1) & registers.at(r2));
        }

        auto ORR(const string &instruction) -> void {
            const int r1 = GetArg1(instruction);
            const int r2 = GetArg2(instruction);
            const int r3 = GetArg3(instruction);
            Assign(r3, registers.at(r1) | registers.at(r2));
        }

        auto XOR(const string &instruction) -> void {
            const int r1 = GetArg1(instruction);
            const int r2 = GetArg2(instruction);
            const int r3 = GetArg3(instruction);
            Assign(r3, registers.at(r1) ^ registers.at(r2));
        }

        auto NOT(const string &instruction) -> void {
            const int r1 = GetArg1(instruction);
            const int r2 = GetArg2(instruction);
            Assign(r2, 15 - registers.at(r1));
        }

        auto PSH() -> void {
            stack.push(make_pair(registers.at(0), registers.at(1)));
        }

        auto POP() -> void {
            Assign(0, stack.top().first);
            Assign(1, stack.top().second);
            stack.pop();
        }
        
        auto PPC(const string &instruction) -> void {
            const int r1 = GetArg1(instruction);
            const int newprogramCounter = programCounter + r1;

            const int v1 = div(programCounter, 256).quot;
            const int v2 = div(div(newprogramCounter, 256).rem, 16).quot;
            const int v3 = div(div(newprogramCounter, 256).rem, 16).rem;

            stack.push(make_pair(v2, v1));
            stack.push(make_pair(0, v3));
        }

        auto RET() -> void {
            const int v3 = stack.top().second;
            stack.pop();
            const int v2 = stack.top().first;
            const int v1 = stack.top().second;
            stack.pop();
            programCounter = v1*256 + v2*16 + v3;
        }

        auto BRA(const string &instruction) -> void {
            const int r1 = GetArg1(instruction);
            const int r2 = GetArg2(instruction);
            const int r3 = GetArg3(instruction);
            programCounter = registers.at(r3)*256 + registers.at(r2)*16 + registers.at(r1);
        }

        auto BRP(const string &instruction) -> void {
            const int r1 = GetArg1(instruction);
            const int r2 = GetArg2(instruction);
            const int r3 = GetArg3(instruction);
            const int r4 = GetArg4(instruction);
            if (registers.at(r4) < 8) {
                programCounter = registers.at(r3)*256 + registers.at(r2)*16 + registers.at(r1);
            }
        }

        auto MOV(const string &instruction) -> void {
            const int r1 = GetArg1(instruction);
            const int r2 = GetArg2(instruction);
            registers.at(r2) = registers.at(r1);
        }

        auto SET(const string &instruction) -> void {
            const int v1 = GetArg1(instruction);
            const int r1 = GetArg2(instruction);
            registers.at(r1) = v1;
        }

        auto FormatInstruction(string instruction) -> string {
            while (instruction.size() < 17) {
                instruction += " ";
            }
            return instruction;
        }

        auto DebugCycle(const unordered_map<int, string> &comments, const string &instruction) -> void {
            if (comments.find(programCounter) != comments.end()) {
                cout << colors::WHITE << comments.at(programCounter) << "\n";
            }
            cout << colors::WHITE << FormatInstruction(instruction);
            for (int i = 0; i < registers.size(); i++) {
                cout << colors::CYAN << "R" << i << " " << colors::AMBER << FormatValue(registers.at(i));
                cout << colors::WHITE << "|";
            }
            cout << 
                colors::CYAN  << "SP " << colors::AMBER << FormatAddress(stack.size()) <<
                colors::WHITE << "|"  << 
                colors::CYAN  << "PC " << colors::AMBER << FormatAddress(programCounter);
            cout << "\n";
        }
    }

    auto Reset() -> void {
        for (int i = 0; i < REGISTER_SIZE; i++) {
            registers.at(i) = 0;
        }

        for (int i = 0; i < MEMORY_SIZE; i++) {
            memory.at(i) = make_pair(0, 0);
        }

        while (!stack.empty()) {
            stack.pop();
        }

        programCounter = 0;
        cycles = 0;
        carry1 = 0;
        carry2 = 0;
    }

    auto Run(const vector<string> &program, const unordered_map<int, string> &comments, const bool debugMode) -> void {

        while (programCounter != FINAL_INSTRUCTION) {
            const string instruction = program.at(programCounter);
            const string opcode = instruction.substr(0, 3);

            if (debugMode) {
                DebugCycle(comments, instruction);
            }

            if (opcode == "BRA") {
                infiniteLoopHeuristic++;
            } else if (opcode == "BRP") {
                infiniteLoopHeuristic = 0;
            }

            if (infiniteLoopHeuristic > INFINITE_LOOP_HEURISTIC_LIMIT) {
                std::cerr << colors::RED << "Infinite loop heuristic triggered" << colors::WHITE << "\n";
                break;
            }

            if      (opcode == "NOP") { NOP(instruction); }

            else if (opcode == "LDA") { LDA(instruction); }
            else if (opcode == "STA") { STA(instruction); }

            else if (opcode == "ADD") { ADD(instruction); }
            else if (opcode == "ADC") { ADC(instruction); }
            else if (opcode == "SUB") { SUB(instruction); }
            else if (opcode == "SBC") { SBC(instruction); }

            else if (opcode == "AND") { AND(instruction); }
            else if (opcode == "ORR") { ORR(instruction); }
            else if (opcode == "XOR") { XOR(instruction); }
            else if (opcode == "NOT") { NOT(instruction); }

            else if (opcode == "PSH") { PSH(); }
            else if (opcode == "POP") { POP(); }
            else if (opcode == "PPC") { PPC(instruction); }
            else if (opcode == "RET") { RET(); }

            else if (opcode == "BRA") { BRA(instruction); }
            else if (opcode == "BRP") { BRP(instruction); }

            else if (opcode == "MOV") { MOV(instruction); }
            else if (opcode == "SET") { SET(instruction); }
            
            carry2 = carry1;
            carry1 = 0;
            programCounter++;
            cycles++;
        }

    }

    auto GetRegister(const int x) -> int {
        return registers.at(x);
    }

    auto GetData(const int x) -> pair<int, int> {
        return memory.at(x);
    }

    auto GetCycles() -> int {
        return cycles;
    }
}