#include "generator/dataValue.hpp"
#include "util/errors.hpp"
#include <generator/registers.hpp>

#include <util/util.hpp>



namespace registers {
    namespace {

        set<int> freeRegisters = {2, 3, 4, 5, 6, 7, 8, 9};
        set<int> allocatedRegisters;

        vector<DataValue> values;
    }

    auto Reset() -> void {
        freeRegisters = {2, 3, 4, 5, 6, 7, 8, 9};
        allocatedRegisters.clear();
        values.clear();
    }

    auto Allocate() -> DataValue& {
        // If no register is available we'll need to cache one register
        if (freeRegisters.empty()) {
            values.at(0).Cache();
        }

        const int newRegister = *(freeRegisters.begin());
        freeRegisters.erase(freeRegisters.begin());
        allocatedRegisters.insert(newRegister);

        values.push_back(DataValue(newRegister));
        return *values.end();
    }

    auto Free(const int r) -> void {
        if (allocatedRegisters.count(r) == 0) {
            errors::AddError(errors::FREE_UNALLOCATED_REGISTER, colors::RED + "Attempt to free an unallocated register");
            return;
        }
        allocatedRegisters.erase(allocatedRegisters.find(r));
        freeRegisters.insert(r);
    }
}