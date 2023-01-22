#pragma once

#include <util/types.hpp>



auto Pow(int x, const int y) -> int;
auto DenaryToBinary(int x, const int bits) -> string;
auto FileExists(const string &path) -> bool;
auto Contains(const vector<int> container, const int target) -> bool;

auto FormatValue(int x) -> string;
auto FormatAddress(int x) -> string;