#include "util/errors.hpp"
#include <generator/registers.hpp>

#include <util/util.hpp>



namespace registers {
    namespace {
        bool MDR1reserved = false;
        bool MDR2reserved = false;

        set<int> freeRegisters = {2, 3, 4, 5, 6, 7, 8, 9};
        set<int> allocatedRegisters;
    }

    auto Reset() -> void {
        MDR1reserved = false;
        MDR2reserved = false;
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

    auto AllocateMDR1() -> int {
        if (MDR1reserved) {
            errors::AddError(errors::FREE_UNALLOCATED_REGISTER, colors::RED + "Attempt to reserve allocated MDR1");
        }
        MDR1reserved = true;
        return MDR1;
    }

    auto AllocateMDR2() -> int {
        if (MDR2reserved) {
            errors::AddError(errors::FREE_UNALLOCATED_REGISTER, colors::RED + "Attempt to reserve allocated MDR2");
        }
        MDR2reserved = true;
        return MDR2;
    }

    auto Free(const int r) -> void {
        if (r == MDR1) {
            if (!MDR1reserved) {
                errors::AddError(errors::FREE_UNALLOCATED_REGISTER, colors::RED + "Attempt to free unallocated MDR1");
                return;
            }
            MDR1reserved = false;
            return;
        }

        if (r == MDR2) {
            if (!MDR2reserved) {
                errors::AddError(errors::FREE_UNALLOCATED_REGISTER, colors::RED + "Attempt to free unallocated MDR2");
                return;
            }
            MDR2reserved = false;
            return;
        }

        if (allocatedRegisters.count(r) == 0) {
            errors::AddError(errors::FREE_UNALLOCATED_REGISTER, colors::RED + "Attempt to free an unallocated register");
            return;
        }
        allocatedRegisters.erase(allocatedRegisters.find(r));
        freeRegisters.insert(r);
    }
}