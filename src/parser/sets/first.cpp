#include "grammar/productions.hpp"
#include <parser/sets/first.hpp>

#include <parser/sets/setUtil.hpp>

#include <iostream>
#include <unordered_map>



namespace first {

    namespace {
        bool firstSetComputed = false;

        unordered_map<int, vector<set<int>>> firstSet;

        namespace AddUnexpandedSymbols {


            auto AddProductionRightToFirstSetRight(const ProductionRight &productionRight, set<int> &firstSetRight) -> void {
                // Does not add duplicate entries
                if (firstSetRight.count(productionRight[0]) == 0) {
                    firstSetRight.insert(productionRight[0]);
                }
            }

            auto AddUnexpandedSymbols() -> void {
                for (const auto &pair : productions) {
                    const ProductionRightList &productionRightList = pair.second;
                    vector<set<int>> &symbolVector = firstSet[pair.first];
                    for (const ProductionRight &productionRight : productionRightList) {
                        set<int> newSymbols;
                        AddProductionRightToFirstSetRight(productionRight, newSymbols);
                        symbolVector.push_back(newSymbols);
                    }
                }
            }
        }

        namespace ExpandFirstSets { 
            auto ContainsFirstSetOfNonTerminal(set<int> &symbols, const int nonTerminal) -> bool {
                // Check if terminalsAndNonTerminals contains all characters in FIRST(nonTerminal)
                for (const set<int> &newSymbols : firstSet.at(nonTerminal)) {
                    for (const int newSymbol : newSymbols) {
                        if (symbols.count(newSymbol) == 0) {
                            return false;
                        }
                    }
                }
                return true;
            }
            
            auto ExpandFirstSetsOneIteration(bool &setChanged, set<int> &symbols) -> void {
                // Expand all the nested first sets in the given non-terminal set
                set<int> newSymbols;
                for (const int symbol : symbols) {

                    // If current symbol is a terminal
                    if (setUtil::IsSymbolTerminal(symbol)) {
                        newSymbols.insert(symbol);

                    // If current symbol is a non-terminal
                    } else {
                        
                        // Add the first set corresponding to the non-terminal
                        if (!ContainsFirstSetOfNonTerminal(symbols, symbol)) {
                            setUtil::AddFirstSet(newSymbols, symbol);
                            setChanged = true;
                        }
                    }
                }
                symbols = newSymbols;
            }

            auto ExpandFirstSets() -> void {
                bool setChanged = true;
                while (setChanged) {
                    setChanged = false;
                    for (auto &pair : firstSet) {
                        for (set<int> &x: pair.second) {
                            ExpandFirstSetsOneIteration(setChanged, x);
                        }
                    }
                }
            }
        }
    }

    auto ComputeFirstSet() -> void {
        if (firstSetComputed) {
            return;
        }
        setUtil::AddEmptyNonTerminalEntries(firstSet);
        setUtil::AddEmptyTerminalEntries(firstSet);
        AddUnexpandedSymbols::AddUnexpandedSymbols();
        ExpandFirstSets::ExpandFirstSets();
        setUtil::PopulateFirstTerminalEntries(firstSet);
        firstSetComputed = true;
    }

    auto GetFirstSet() -> unordered_map<int, vector<set<int>>>& {
        return firstSet;
    }
}