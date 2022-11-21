#pragma once

#include <scanner/tokens.hpp>



namespace scanner {
    auto Scan(const vector<string> &lines) -> vector<Token>;
}