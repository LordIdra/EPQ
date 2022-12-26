#pragma once

#include <util/types.hpp>



namespace follow {
    auto ComputeFollowSet() -> void;
    auto GetFollowSet() -> unordered_map<int, set<int>>&;
}