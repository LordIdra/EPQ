#pragma once



namespace registers {
    const int TRUE = 0;
    const int FALSE = 8;

    const int MDR1 = 0;
    const int MDR2 = 1;
    
    auto Allocate() -> int;
    auto AllocateMDR1() -> int;
    auto AllocateMDR2() -> int;
    auto Free(const int r) -> void;
}