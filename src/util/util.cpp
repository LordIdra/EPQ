#include "util.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>

using std::ifstream;



auto Pow(const int x, const int y) -> int {
    // x^0 = 1
    if (y == 0) {
        return 1;
    }

    // Loop through y-1 times and multiply the current value by x
    int result = x;
    for (int i = 0; i < y-1; i++) {
        result *= x;
    }

    return result;
}

auto DenaryToBinary(int x, const int bits) -> string {
    string binary_string;

    // If x is greater than the maximum value for the given number of bits, that's an overflow
    // For the algorithm to work with this we'll need x >= 0 and x < max value
    int max_value = Pow(2, bits);
    if (x >= max_value) {
        x -= max_value;
    }

    // We'll start at the MSB, so find the value of said MSB
    int bit_value = max_value / 2;

    // Loop through every bit
    for (int bit = 0; bit < bits; bit++) {

        // Find if the current bit should be 1 or 0, add either 0 or 1 to the result string based on this
        // If the bit should be 1, subtract the bit value - this will allow us to use the same
        // technique on the next iteration
        if ((x - bit_value) >= 0) {
            binary_string += "1";
            x -= bit_value;
        } else {
            binary_string += "0";
        }

        // Next bit value will be half the previous one
        bit_value /= 2;
    }

    return binary_string;
}

auto FileExists(const string &path) -> bool {
    // Open a file as an ifstream
    ifstream file(path);
    
    // Check if the opened file contains any data - if so, the file exists
    return bool(file);
}

auto Contains(const vector<int> container, const int target) -> bool {
    return std::count(container.begin(), container.end(), target) != 0;
}