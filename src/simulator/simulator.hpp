#pragma once

#include <util/types.hpp>




namespace simulator {
    auto Reset() -> void;
    auto Run(vector<string> instructions, const bool debugMode) -> void;
}