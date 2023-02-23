#pragma once

#include <generator/dataValues.hpp>



enum Register {
    MDR_1,
    MDR_2,
    MER_1,
    REG_1,
    REG_2,
    REG_3,
    REG_4,
    REG_5,
    REG_6,
    REG_7
};

enum ByteHalf {
    FIRST,
    SECOND
};

struct NibbleAddress {
    int address;
    ByteHalf half;
};

struct DataValue {
    bool isCached;
    Register register_;
    NibbleAddress address;
};