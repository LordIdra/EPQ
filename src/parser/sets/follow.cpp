#include <parser/sets/follow.hpp>

#include <grammar/productions.hpp>
#include <parser/sets/first.hpp>
#include <parser/sets/setUtil.hpp>

#include <iostream>



namespace follow {
    namespace {
        bool followSetComputed = false;

        unordered_map<int, set<int>> followSet;

        namespace AddUnexpandedSymbols {
            auto IterateProduction(const ProductionRight &productionRight, const int startingPosition) -> int {
                // Iterates over the production until either the end of the production is reached, or the next
                // terminal/non-terminal's FIRST set does not contain NONE
                int j = startingPosition;
                for (int symbol : productionRight) {
                    if (setUtil::FirstSetContainsEpsilon(symbol)) {
                        break;
                    }
                    j++;
                }
                return j;
            }

            auto HandleEpsilon(const ProductionLeft productionLeft, const ProductionRight &productionRight, int i) -> void {
                const int originalSymbol = productionRight.at(i);

                // Move to next token so we don't start by checking if the original symbol's FIRST set contains epsilon
                i++;

                // If said next token does not contain epsilon, do not continue
                if (!setUtil::FirstSetContainsEpsilon(productionRight[i])) {
                    return;
                }

                // Iterate while FIRST of the symbol at i contains epsilon
                // If said symbol's FIRST set does contain epsilon, the FIRST set of the next symbol should be added
                while ((i+1 != productionRight.size()) && setUtil::FirstSetContainsEpsilon(productionRight[i])) {
                    const int nextSymbol = productionRight.at(i+1);
                    setUtil::AddFirstSet(followSet.at(originalSymbol), nextSymbol);
                    i++;
                }
                
                if (i+1 == productionRight.size()) {
                    // All subsequent symbol FIRST sets contain epsilon
                    followSet.at(originalSymbol).insert(productionLeft);
                }
            }

            auto ProcessPoduction_NextSymbolExists(const ProductionLeft productionLeft, const ProductionRight &productionRight, int i) -> void {
                // Add FIRST(b) to FOLLOW(a)
                const int originalSymbol = productionRight.at(i);
                const int nextSymbol = productionRight.at(i+1);
                setUtil::AddFirstSet(followSet.at(originalSymbol), nextSymbol);

                // Handle any subsequent symbols with epsilon in their FIRST set
                HandleEpsilon(productionLeft, productionRight, i);
            }

            auto ProcessProduction_EndOfNonTerminal(const ProductionLeft productionLeft, const int currentSymbol) -> void {
                // Add FOLLOW(A)
                followSet.at(currentSymbol).insert(productionLeft);
            }

            auto ProcessProduction(const ProductionLeft productionLeft, const ProductionRight &productionRight) -> void {
                for (int i = 0; i < productionRight.size(); i++) {

                    // If x -> ...ab...
                    if (i+1 < productionRight.size()) {
                        ProcessPoduction_NextSymbolExists(productionLeft, productionRight, i);
                    }

                    // If A -> ...a
                    else if (i+1 == productionRight.size()) {
                        const int currentTerminalOrNonTerminal = productionRight.at(i);
                        ProcessProduction_EndOfNonTerminal(productionLeft, currentTerminalOrNonTerminal);
                    }
                }
            }

            auto AddUnexpandedSymbols() -> void {
                for (const auto &pair : productions) {
                    for (const ProductionRight &production : pair.second) {
                        ProcessProduction(pair.first, production);
                    }
                }
            }
        }

        namespace ExpandFollowSets { 
            auto ContainsFollowSetOfNonTerminal(set<int> &symbols, const int nonTerminal) -> bool {
                // Check if terminalsAndNonTerminals contains all characters in FOLLOW(nonTerminal)
                for (const int newSymbol : followSet.at(nonTerminal)) {
                    if (symbols.count(newSymbol) == 0) {
                        return false;
                    }
                }
                return true;
            }

            auto ExpandFollowSetsOneIteration(bool &setChanged, set<int> &symbols) -> void {
                // Expand all the nested follow sets in the given non-terminal set
                set<int> newSymbols;
                for (const int symbol : symbols) {

                    // If current symbol is a terminal
                    if (setUtil::IsSymbolTerminal(symbol)) {
                        newSymbols.insert(symbol);

                    // If current symbol is a non-terminal
                    } else {

                        // Add the follow set corresponding to the non-terminal
                        if (!ContainsFollowSetOfNonTerminal(symbols, symbol)) {
                            setUtil::AddFollowSet(newSymbols, symbol);
                            setChanged = true;
                        }
                    }
                }
                symbols = newSymbols;
            }

            auto ExpandFollowSets() -> void {
                bool setChanged = true;
                while (setChanged) {
                    setChanged = false;
                    for (auto &pair : followSet) {
                        ExpandFollowSetsOneIteration(setChanged, pair.second);
                    }
                }
            }
        }
    }

    auto ComputeFollowSet() -> void {
        if (followSetComputed) {
            return;
        }
        setUtil::AddEmptyTerminalEntries(followSet);
        setUtil::AddEmptyNonTerminalEntries(followSet);
        AddUnexpandedSymbols::AddUnexpandedSymbols();
        ExpandFollowSets::ExpandFollowSets();
        followSetComputed = true;
    }

    auto GetFollowSet() -> unordered_map<int, set<int>>& {
        return followSet;
    }
}