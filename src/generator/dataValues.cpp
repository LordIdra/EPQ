#include "generator/assembly.hpp"
#include "generator/dataValue.hpp"
#include "util/errors.hpp"
#include <generator/dataValues.hpp>

#include <memory>
#include <unordered_map>
#include <util/util.hpp>



namespace dataValues {
    namespace {
        const int MAX_ADDRESS = 2048;
        const int MIN_CACHE_ADDRESS = 2000;
        int cacheAddress = MIN_CACHE_ADDRESS;

        enum RegisterStatus {
            ALLOCATED, 
            FREE
        };

        const unordered_map<Register, RegisterStatus> DEFAULT_REGISTERS = {
            {REG_1, FREE},
            {REG_2, FREE},
            {REG_3, FREE},
            {REG_4, FREE},
            {REG_5, FREE},
            {REG_6, FREE},
            {REG_7, FREE}};
        unordered_map<Register, RegisterStatus> registers = DEFAULT_REGISTERS;
        unordered_map<int, DataValue> values;
        int nextValueID = 0;

        auto FreeRegisterExists() -> bool {
            for (auto pair : registers) {
                if (pair.second == FREE) {
                    return true;
                }
            }
            return false;
        }

        auto GetFreeRegister() -> Register {
            for (auto &pair : registers) {
                if (pair.second) {
                    pair.second = ALLOCATED;
                    return pair.first;
                }
            }
            throw;
        }

        auto PrepareRegisterForCaching(const ByteHalf half) -> Register {
            int toCache_id = 0;

            for (auto &value : values) {
                if (!values.at(value.first).isCached) {
                    toCache_id = value.first;
                    break;
                }
            }

            //std::cout << toCache_id << "\n";

            values.at(toCache_id).address = NibbleAddress{cacheAddress, half};
            values.at(toCache_id).isCached = true;
            registers.at(values.at(toCache_id).register_) = FREE;

            if (half == FIRST) {
                assembly::MOV(values.at(toCache_id).register_, MDR_1);
            } else if (half == SECOND) {
                assembly::MOV(values.at(toCache_id).register_, MDR_2);
            }

            return values.at(toCache_id).register_;
        }

        auto CacheDataValuePair() -> void {
            if (cacheAddress > MAX_ADDRESS) {
                errors::AddError(errors::OUT_OF_MEMORY, "Out of memory for register cache.");
                throw;
            }

            assembly::Comment("prepare to cache first half");
            const Register r1 = PrepareRegisterForCaching(FIRST);

            assembly::Comment("prepare to cache second half");
            const Register r2 = PrepareRegisterForCaching(SECOND);

            cacheAddress++;

            const int a1 = div(cacheAddress, 256).quot;
            const int a2 = div(cacheAddress - (a1*256), 16).quot;
            const int a3 = cacheAddress - (a1*256) - (a2*16);

            assembly::Comment("cache register pair");
            assembly::SET(a1, MER_1);
            assembly::SET(a2, r1);
            assembly::SET(a3, r2);
            assembly::STA(MER_1, r1, r2);
        }

        auto UncacheDataValue(const int id) -> void {
            DataValue &toUncache = values.at(id);
            
            const int a1 = div(toUncache.address.address, 256).quot;
            const int a2 = div(toUncache.address.address - (a1*256), 16).quot;
            const int a3 = toUncache.address.address - (a1*256) - (a2*16);

            assembly::Comment("uncache register pair");
            assembly::SET(a1, MDR_1);
            assembly::SET(a2, MDR_2);
            assembly::SET(a3, MER_1);
            assembly::LDA(MDR_1, MDR_2, MER_1);

            const Register newRegister = GetFreeRegister();
            toUncache.isCached = false;
            toUncache.register_ = newRegister;

            if (toUncache.address.half == FIRST) {
                assembly::Comment("restore first half ");
                assembly::MOV(MDR_1, newRegister);
            } else if (toUncache.address.half == SECOND) {
                assembly::Comment("restore second half ");
                assembly::MOV(MDR_2, newRegister);
            }
        }
    }

    auto Reset() -> void {
        registers = DEFAULT_REGISTERS;
        values.clear();
    }

    auto Allocate() -> int {
        if (!FreeRegisterExists()) {
            CacheDataValuePair();
        }

        const Register newRegister = GetFreeRegister();
        const int id = nextValueID;
        nextValueID++;

        values.insert(std::make_pair(id, 
            DataValue{false, newRegister, -1, FIRST}));
        return id;
    }

    auto Free(const int id) -> void {
        DataValue &value = values.at(id);
        if (value.isCached) {
            // todo free memory

        } else {
            registers.at(value.register_) = FREE;
        }
        values.erase(id);
    }

    auto Load(const int id) -> int {
        DataValue &value = values.at(id);
        if (value.isCached) {
            if (!FreeRegisterExists()) {
                CacheDataValuePair();
            }
            UncacheDataValue(id);
        }
        return values.at(id).register_;
    }
}