#pragma once

#include <grammar/productions.hpp>

#include <util/util.hpp>


namespace first {
    auto ComputeFirstSet() -> void;
    auto GetFirstSet() -> unordered_map<int, vector<set<int>>>&;
}