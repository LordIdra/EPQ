#pragma once

#include "generator/dataValue.hpp"
#include <util/types.hpp>



namespace assembly {
    auto Reset() -> void;
    
    auto GenerateLabel(const string &identifier) -> string;
    auto Label(const string &label) -> void;
    auto ResolveLabels() -> void;
    auto GetProgram() -> const vector<string>&;
    auto GetComments() -> const unordered_map<int, string>&;

    auto NOP() -> void;

    auto LDA(const int r1, const int r2, const int r3) -> void;
    auto LDA(const Register r1, const Register r2, const Register r3) -> void;
    auto LDA(const string &identifier) -> void;

    auto STA(const int r1, const int r2, const int r3) -> void;
    auto STA(const Register r1, const int r2, const int r3) -> void;
    auto STA(const string &identifier) -> void;

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

    auto BRA(const int r1, const int r2, const int r3) -> void;
    auto BRA(const Register r1, const Register r2, const Register r3) -> void;
    auto BRA(const string &label) -> void;

    auto BRP(const int r1, const int r2, const int r3, const int r4) -> void;
    auto BRP(const Register r1, const Register r2, const Register r3, const Register r4) -> void;
    auto BRP(const string &label, const int r1) -> void;

    auto MOV(const int r1, const int r2) -> void;
    auto MOV(const int r1, const Register r2) -> void;
    auto MOV(const Register r1, const int r2) -> void;
    auto MOV(const Register r1, const Register r2) -> void;

    auto SET(const int v1, const int r1) -> void;
    auto SET(const int v1, const Register r1) -> void;
    auto SET(const string &identifier, const int r1) -> void;
    auto SET(const string &identifier, const Register r1) -> void;

    auto Comment(const string &comment) -> void;
}