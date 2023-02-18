#include "util/errors.hpp"
#include <generator/memory.hpp>
#include <unordered_map>



namespace memory {

    namespace {
        set<int> freeMemory;
        unordered_map<string, int> allocatedMemory;
    }

    auto Reset() -> void {
        freeMemory.clear();
        allocatedMemory.clear();
    }

    auto Allocate(const string &identifier) -> void {
        if (freeMemory.empty()) {
            errors::AddError(errors::OUT_OF_MEMORY, colors::RED + "No remaining free memory addresses");
            return;
        }

        const int newMemoryLocation = *(freeMemory.begin());
        freeMemory.erase(freeMemory.begin());
        allocatedMemory.insert(std::make_pair(identifier, newMemoryLocation));
    }

    auto Free(const string &identifier) -> void {
        if (allocatedMemory.count(identifier) == 0) {
            errors::AddError(errors::FREE_UNALLOCATED_MEMORY, colors::RED + "Attempt to free unallocated memory");
            return;
        }

        freeMemory.insert(LocationOf(identifier));
        allocatedMemory.erase(allocatedMemory.find(identifier));
    }

    auto LocationOf(const string &identifier) -> int {
        return allocatedMemory.at(identifier);
    }

}