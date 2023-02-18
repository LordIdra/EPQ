#pragma once



namespace registers {
    const int TRUE = 0;
    const int FALSE = 8;

    const int MDR1 = 0;
    const int MDR2 = 1;
    
    auto Reset() -> void;
    auto Allocate() -> int;
    auto Free(const int r) -> void;
}