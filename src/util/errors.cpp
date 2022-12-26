#include <util/errors.hpp>

#include <iostream>



namespace errors {

    namespace {
        int errorCode = NONE;
        bool stopCompilation = false;
        vector<string> errors;
    }

    auto Reset() -> void {
        errorCode = NONE;
        stopCompilation = false;
        errors.clear();
    }

    auto StopCompilation() -> void {
        stopCompilation = true;
    }

    auto ShouldCompilationBeStopped() -> bool {
        return stopCompilation;
    }

    auto GetErrorCode() -> int {
        return errorCode;
    }

    auto AddError(const ErrorCode code, const string &error) -> void {
        errors.push_back(error);
        errorCode = code;
    }

    auto OutputErrors() -> void {
        if (errors.empty()) {
            std::cout << colors::BOLD_GREEN << "0 compiler errors" << colors::WHITE << "\n";
            return;
        }
        
        std::cout << colors::BOLD_RED << std::to_string(errors.size()) << " compiler errors" << colors::WHITE << "\n";
        for (const string &error : errors) {
            std::cout << error << "\n";
        }
    }
}