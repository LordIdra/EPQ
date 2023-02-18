#include "readfile.hpp"
#include "util/errors.hpp"
#include "util/util.hpp"

#include <fstream>
#include <iostream>

using std::ifstream;



namespace readfile {
    auto Read(const string &path) -> vector<string> {
        if (!FileExists(path)) {
            errors::AddError(errors::FILE_NOT_FOUND, colors::RED + "File not found: " + colors::CYAN + path + "\n");
            return vector<string>();
        }
        vector<string> lines;
        ifstream file(path);
        string currentLine;
        while (std::getline(file, currentLine)) {
            lines.push_back(currentLine);
        }
        return lines;
    }
}