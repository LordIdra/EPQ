#pragma once

#include <grammar/terminals.hpp>



namespace scanner {
    auto Scan(const vector<string> &lines) -> const vector<Token>&;
    auto Reset() -> void;
}