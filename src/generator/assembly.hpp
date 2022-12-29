#pragma once

#include <util/types.hpp>



namespace assembly {
    auto GenerateLabel(const string &identifier) -> string;
    auto LabelLatestInstruction(const string &label) -> void;
    auto GetProgram() -> vector<string>;

    auto NOP() -> void;

    auto LDA(const string label) -> void;
    auto STA(const string label) -> void;

    auto ADD(const int r1, const int r2, const int r3) -> void;
    auto ADC(const int r1, const int r2, const int r3) -> void;
    auto SUB(const int r1, const int r2, const int r3) -> void;
    auto SBC(const int r1, const int r2, const int r3) -> void;

    auto AND(const int r1, const int r2, const int r3) -> void;
    auto ORR(const int r1, const int r2, const int r3) -> void;
    auto XOR(const int r1, const int r2, const int r3) -> void;
    auto NOT(const int r1, const int r2) -> void;

    auto PSH() -> void;
    auto POP() -> void;

    auto PPC(const int v1) -> void;
    auto RET() -> void;

    auto BRA(const string label) -> void;
    auto BRP(const string label, const int r1) -> void;

    auto MOV(const int r1, const int r2) -> void;
    auto SET(const int v1, const int r1) -> void;
}