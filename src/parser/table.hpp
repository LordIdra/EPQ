#pragma once

#include "grammar/productions.hpp"
#include <util/types.hpp>



namespace table {   
    auto GenerateTable() -> void;
    auto GetTable() -> const unordered_map<int, unordered_map<int, ProductionPair>>&;
}