#include "util/errors.hpp"
#include <generator/registers.hpp>

#include <util/util.hpp>



namespace registers {
    namespace {

        set<int> freeRegisters = {2, 3, 4, 5, 6, 7, 8, 9};
        set<int> allocatedRegisters;
    }

    auto Reset() -> void {
        freeRegisters = {2, 3, 4, 5, 6, 7, 8, 9};
        allocatedRegisters.clear();
    }

    auto Allocate() -> int {
        if (freeRegisters.empty()){
            errors::AddError(errors::NO_FREE_REGISTERS, colors::RED + "No free registers remaining");
            return -1;
        }

        const int newRegister = *(freeRegisters.begin());
        freeRegisters.erase(freeRegisters.begin());
        allocatedRegisters.insert(newRegister);
        return newRegister;
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