#include "readfile.hpp"
#include "util/util.hpp"

#include <fstream>
#include <iostream>

using std::ifstream;



namespace readfile {
    auto Read(const string &path) -> vector<string> {
        if (!FileExists(path)) {
            std::cerr << "File not found: " + path + "\n";
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