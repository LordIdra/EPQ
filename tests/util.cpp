#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "util.hpp"
#include "util/errors.hpp"



auto CheckErrorCode(const errors::ErrorCode code) -> void {
    if (errors::GetErrorCode() != code) {
        std::cout << colors::CYAN + "Expected error code " << colors::BOLD_RED << code << 
                     colors::CYAN + " but got " << colors::BOLD_RED << errors::GetErrorCode() << "\n";
        errors::OutputErrors();
    }
    REQUIRE(errors::GetErrorCode() == code);
}