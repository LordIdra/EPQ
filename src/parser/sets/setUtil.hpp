#pragma once

#include <grammar/productions.hpp>

#include <util/types.hpp>



namespace setUtil {
    auto AddEmptyNonTerminalEntries(unordered_map<int, set<int>> &addTo) -> void;
    auto AddEmptyTerminalEntries(unordered_map<int, set<int>> &addTo) -> void;

    auto AddEmptyNonTerminalEntries(unordered_map<int, vector<set<int>>> &addTo) -> void;
    auto AddEmptyTerminalEntries(unordered_map<int, vector<set<int>>> &addTo) -> void;

    auto PopulateFirstTerminalEntries(unordered_map<int, vector<set<int>>> &addTo) -> void;

    auto AddFirstSet(set<int> &addTo, const int terminalOrNonTerminal) -> void;
    auto AddFollowSet(set<int> &addTo, const int terminalOrNonTerminal) -> void;

    auto IsSymbolTerminal(const int symbol) -> bool;

    auto FirstSetContainsEpsilon(const int symbol) -> bool;

    auto RulesEqual(const ProductionPair &v1, const ProductionPair &v2) -> bool;

    auto RemoveEpsilon(const vector<int> &vec) -> vector<int>;
}