#pragma once

#include <util/types.hpp>



// https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
// https://en.wikipedia.org/wiki/ANSI_escape_code
namespace colors {
    const string WHITE = "\033[0;37m";
    const string CYAN  = "\033[0;36m";

    const string AMBER = "\033[0;33m";
    const string RED   = "\033[0;31m";

    const string BOLD_GREEN = "\033[1;32m";
    const string BOLD_RED   = "\033[1;31m";
}

namespace errors {
    enum ErrorCode {
        NONE,

        EXPECTED_TERMINAL,

        OVERWRITING_TABLE_ENTRY,

        UNRECOGNIZED_SYMBOL,
        INVALID_SYNTAX,

        UNKNOWN_IDENTIFIER,
        MISMATCHED_TYPE,
        
        INVALID_INSTRUCTION_SIZE,
        INVALID_OPCODE,
        INVALID_OPERAND_COUNT,
        INVALID_OPERAND,

        NO_FREE_REGISTERS,
        FREE_UNALLOCATED_REGISTER,
        OUT_OF_MEMORY,
        FREE_UNALLOCATED_MEMORY,

        INCORRECT_NUMBER_OF_ARGUMENTS
    };
    
    auto Reset() -> void;
    auto StopCompilation() -> void;
    auto ShouldCompilationBeStopped() -> bool;
    auto GetErrorCode() -> int;
    auto AddError(const ErrorCode code, const string &error) -> void;
    auto OutputErrors() -> void;
}