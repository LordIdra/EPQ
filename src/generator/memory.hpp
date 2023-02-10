#pragma once

#include <util/types.hpp>



namespace memory {
    auto Reset() -> void;
    auto Allocate(const string &identifier) -> void;
    auto Free() -> void;
    auto LocationOf(const string &identifier) -> int;
}