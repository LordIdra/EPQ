#include "scanner.hpp"



namespace scanner {

    namespace {
        vector<Token> tokens;

        auto ScanNextToken(const string &line) -> unsigned int {
            bool tokenEnd = false;
            int index = 0;
            Token token = {""};
            while (!tokenEnd) {
                const char character = line[index];
            }
            return index;
        }

        auto ParseLine(const string &line) -> void {
            for (const char c : line) {
                ScanNextToken(line);
            }
        }
    }

    auto Scan(const vector<string> &lines) -> vector<Token> {
        for (const string &line : lines) {
            ParseLine(line);
        }
        return tokens;
    }
}