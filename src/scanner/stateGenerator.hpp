#pragma once

#include "grammar/terminals.hpp"
#include <util/types.hpp>



namespace stateGenerator {
    auto GenerateStates() -> void;
    auto Transitions() -> const unordered_map<int, unordered_map<char, int>>&;
    auto FinalStates() -> const unordered_map<int, Terminal>&;
}