#pragma once

#include <util/types.hpp>




namespace simulator {
    auto Reset() -> void;
    auto Run(const vector<string> &program, const unordered_map<int, string> &comments, const bool debugMode) -> void;
    auto GetData(const int x) -> pair<int, int>;
    auto GetCycles() -> int;
}