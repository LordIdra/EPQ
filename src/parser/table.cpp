#include "grammar/nonTerminals.hpp"
#include "grammar/productions.hpp"
#include "grammar/symbolNames.hpp"
#include "parser/sets/first.hpp"
#include "parser/sets/follow.hpp"
#include "util/errors.hpp"
#include <parser/sets/setUtil.hpp>
#include <iostream>
#include <parser/table.hpp>
#include <unordered_map>



namespace table {

    namespace {
        bool tableGenerated = false;

        unordered_map<int, unordered_map<int, ProductionPair>> table;

        ProductionPair emptyProduction = ProductionPair{-1, ProductionRight{vector<int>()}};

        auto FillTableWithBlanks() -> void {
            for (int nonTerminal = FIRST_NON_TERMINAL; nonTerminal <= LAST_NON_TERMINAL; nonTerminal++) {
                unordered_map<int, ProductionPair> nonTerminalMap;
                for (int terminal = FIRST_TERMINAL; terminal <= LAST_TERMINAL; terminal++) {
                    nonTerminalMap.insert(std::make_pair(terminal, emptyProduction));
                }
                table.insert(std::make_pair(nonTerminal, nonTerminalMap));
            }
        }

        auto LogError(const int nonTerminal, const int terminal, const ProductionPair &newRule) -> void {
            ProductionPair previousRule = table.at(nonTerminal).at(terminal);

            string errorMessage = colors::RED + "Rule already exists in table at [";
            errorMessage += colors::CYAN + symbolNames.at(nonTerminal) + 
                            colors::RED  + "|" +
                            colors::CYAN + symbolNames.at(terminal) + 
                            colors::RED + "]";

            errorMessage += " with previous rule " + colors::CYAN + symbolNames.at(previousRule.first) + " -> ";
            for (const int x : previousRule.second) {
                errorMessage += symbolNames.at(x) + " ";
            }

            errorMessage += colors::RED + "and new rule " + colors::CYAN + symbolNames.at(newRule.first) + " -> ";
            for (const int x : newRule.second) {
                errorMessage += symbolNames.at(x) + " ";
            }

            errorMessage += colors::WHITE;

            errors::AddError(errors::OVERWRITING_TABLE_ENTRY, errorMessage);
        }

        auto OverwritingRule(const int nonTerminal, const int terminal, const ProductionPair &productionPair) -> bool {
            return (table.at(nonTerminal).at(terminal).first != -1) && 
                   (!setUtil::RulesEqual(table.at(nonTerminal).at(terminal), productionPair));
        }

        auto AddRule(const int nonTerminal, const int terminal, const ProductionPair &productionPair) -> void {
            if (OverwritingRule(nonTerminal, terminal, productionPair)) {
                LogError(nonTerminal, terminal, productionPair);
            }
            table.at(nonTerminal).at(terminal) = productionPair;
        }

        auto IterateFirst(const int nonTerminal, const vector<int> &productionRight, const int productionRightIndex) -> void {
            ProductionPair pair = ProductionPair {nonTerminal, productionRight};
            for (int terminal : first::GetFirstSet().at(nonTerminal).at(productionRightIndex)) {
                AddRule(nonTerminal, terminal, pair);
            }
        }

        auto IterateFollow(const int nonTerminal, const vector<int> &productionRight) -> void {
            ProductionPair pair = ProductionPair {nonTerminal, productionRight};
            for (int terminal : follow::GetFollowSet().at(nonTerminal)) {
                AddRule(nonTerminal, terminal, pair);
            }
        }
    }

    auto GenerateTable() -> void {
        if (tableGenerated) {
            return;
        }

        FillTableWithBlanks();

        // Iterate every production
        for (const auto &production : productions) {
            ProductionLeft nonTerminal = production.first;

            int productionRightIndex = 0;
            for (const ProductionRight &productionRight : production.second) {

                IterateFirst(nonTerminal, productionRight, productionRightIndex);
                if (first::GetFirstSet().at(nonTerminal).at(productionRightIndex).count(NONE) != 0) {
                    IterateFollow(nonTerminal, productionRight);
                }
                productionRightIndex++;
            }
        }

        tableGenerated = true;
    }

    auto GetTable() -> const unordered_map<int, unordered_map<int, ProductionPair>>& {
        return table;
    }
}