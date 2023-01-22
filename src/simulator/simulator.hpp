#pragma once

#include <util/types.hpp>




namespace simulator {
    auto Reset() -> void;
    auto Run(std::pair<vector<string>, std::unordered_map<int, string>> input, const bool debugMode) -> void;
    auto GetData(const int x) -> std::pair<int, int>;
}