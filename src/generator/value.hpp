#pragma once



class Value {
private:
    bool isCached;
    int registerIndex;
    int memoryLocation;

public:
    Value();

    auto Cache() -> void;
    auto Load() -> void;
};