#pragma once



class DataValue {
private:
    bool isCached;
    int registerIndex;
    int memoryLocation;

public:
    DataValue(const int registerIndex);
    ~DataValue();

    auto Cache() -> void;
    auto Load() -> void;
};