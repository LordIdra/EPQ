#pragma once

#include "generator/dataValue.hpp"
#include <util/types.hpp>



namespace dataValues {
    const int TRUE = 0;
    const int FALSE = 8;
    
    auto Reset() -> void;
    auto Allocate() -> int;
    auto Free(const int r) -> void;
    auto Load(const int id) -> int;
}