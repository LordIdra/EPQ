#include "dataValue.hpp"
#include "generator/registers.hpp"



DataValue::DataValue(const int registerIndex) 
    : registerIndex(), isCached(false), memoryLocation(0) {}

DataValue::~DataValue() {
    if (!isCached) {
        registers::Free(registerIndex);
    }
}

auto DataValue::Load() -> void {
    if (registers::)
}

auto DataValue::Cache() -> void {
    
}