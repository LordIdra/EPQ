#pragma once

#include <chrono>
#include <util/types.hpp>




namespace integrationTest {
    namespace {
        string name;

        int size;
        int capacity;
        int cycles;

        std::chrono::microseconds time;
    }

    auto SetName(const string &name_) -> void {
        name = name_;
    }

    auto SetProgram(const vector<string> &program) -> void {

    }
};