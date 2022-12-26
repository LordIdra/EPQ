#include "grammar/productions.hpp"
#include "grammar/symbolNames.hpp"
#include "grammar/terminals.hpp"
#include <iostream>
#include <parser/sets/setUtil.hpp>

#include <grammar/nonTerminals.hpp>
#include <parser/sets/first.hpp>
#include <parser/sets/follow.hpp>

#include <unordered_map>
#include <util/util.hpp>



namespace setUtil {

    auto AddEmptyNonTerminalEntries(unordered_map<int, set<int>> &addTo) -> void {
        // Generate a set which consists of all non terminals mapped to empty vectors
        for (int i = FIRST_NON_TERMINAL; i <= LAST_NON_TERMINAL; i++) {
            addTo.insert(std::pair<int, set<int>> (i, set<int>()));
        }
    }

    auto AddEmptyTerminalEntries(unordered_map<int, set<int>> &addTo) -> void {
        for (int i = FIRST_TERMINAL; i <= LAST_TERMINAL; i++) {
            addTo.insert(std::pair<int, set<int>> (i, set<int>()));
        }
    }

    auto AddEmptyNonTerminalEntries(unordered_map<int, vector<set<int>>> &addTo) -> void {
        // Generate a set which consists of all non terminals mapped to empty vectors
        for (int i = FIRST_NON_TERMINAL; i <= LAST_NON_TERMINAL; i++) {
            addTo.insert(std::pair<int, vector<set<int>>> (i, vector<set<int>>()));
        }
    }

    auto AddEmptyTerminalEntries(unordered_map<int, vector<set<int>>> &addTo) -> void {
        for (int i = FIRST_TERMINAL; i <= LAST_TERMINAL; i++) {
            addTo.insert(std::pair<int, vector<set<int>>> (i, vector<set<int>>()));
        }
    }

    auto PopulateFirstTerminalEntries(unordered_map<int, vector<set<int>>> &addTo) -> void {
        for (int i = FIRST_TERMINAL; i <= LAST_TERMINAL; i++) {
            set<int> terminalSet = {i};
            addTo[i].push_back(terminalSet);
        }
    }

    auto AddFirstSet(set<int> &addTo, const int nonTerminal) -> void {
        vector<set<int>> firstSetForNonTerminal = first::GetFirstSet().at(nonTerminal);
        for (const set<int> &x : firstSetForNonTerminal) {
            for (const int y : x) {
                if (addTo.count(y) == 0) {
                    if (y != 0) {
                        addTo.insert(y);
                    }
                }
            }
        }
    }
    auto AddFollowSet(set<int> &addTo, const int terminalOrNonTerminal) -> void {
        set<int> followSetForNonTerminal = follow::GetFollowSet().at(terminalOrNonTerminal);
        for (const int x : followSetForNonTerminal) {
            if ((addTo.count(x)) == 0) {
                if (x != 0) {
                    addTo.insert(x);
                }
            }
        }
    }

    auto IsSymbolTerminal(const int symbol) -> bool {
        return (symbol >= FIRST_TERMINAL) && (symbol <= LAST_TERMINAL);
    }

    auto FirstSetContainsEpsilon(const int symbol) -> bool {
        for (const set<int> &firstSetOfProduction : first::GetFirstSet().at(symbol)) {
            if (firstSetOfProduction.count(NONE) != 0) {
                return true;
            }
        }
        return false;
    }

    auto RulesEqual(const ProductionPair &v1, const ProductionPair &v2) -> bool {
        if (v1.first != v2.first) {
            return false;
        }
        
        if (v1.second.size() != v2.second.size()) {
            return false;
        }

        for (int i = 0; i < v1.second.size(); i++) {
            if (v1.second[i] != v2.second[i]) {
                return false;
            }
        }

        return true;
    }

    auto RemoveEpsilon(const vector<int> &vec) -> vector<int> {
        vector<int> newVec;
        for (int x : vec) {
            if (x != NONE) {
                newVec.push_back(x);
            }
        }
        return newVec;
    }
}