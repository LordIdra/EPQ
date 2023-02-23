#include <util/errors.hpp>

#include <iostream>



namespace errors {

    namespace {
        int errorCode = NONE;
        vector<string> errors;
    }

    auto Reset() -> void {
        errorCode = NONE;
        errors.clear();
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
            cout << colors::BOLD_GREEN << "0 compiler errors" << colors::WHITE << "\n";
            return;
        }
        
        cout << colors::BOLD_RED << std::to_string(errors.size()) << " compiler errors" << colors::WHITE << "\n";
        for (const string &error : errors) {
            cout << error << colors::WHITE << "\n";
        }
    }
}