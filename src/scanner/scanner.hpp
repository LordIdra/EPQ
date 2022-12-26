#pragma once

#include <grammar/terminals.hpp>



namespace scanner {
    auto Scan(const vector<string> &lines) -> vector<Token>;
    auto Reset() -> void;
}