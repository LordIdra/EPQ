#include "stateGenerator.hpp"
#include "grammar/symbolNames.hpp"
#include "grammar/terminalRules.hpp"
#include "grammar/terminals.hpp"
#include <iostream>
#include <utility>



namespace stateGenerator {
    namespace {
        const vector<char> IDENTIFIER_CHARACTERS = {
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
            'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            '_'};

        const vector<char> NUMBER_CHARACTERS = {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

        unordered_map<int, unordered_map<char, int>> transitions = {{0, {}}};
        unordered_map<int, Terminal> finalStates = {};
        int nextState = 0;

        auto GenerateRuleStates() -> void {
            for (const auto &rule : terminalRules) {
                int state = 0;

                for (const char character : rule.second) {

                    // Figure out where we can go from our current state
                    unordered_map<char, int> &transitionsFromCurrentState = transitions.at(state);

                    // If we can go to another state using our current character, go to that state
                    if (transitionsFromCurrentState.count(character) != 0) {
                        state = transitionsFromCurrentState.at(character);
                    }

                    // Otherwise. create a new state and move to it
                    else {
                        nextState++;
                        transitions.insert(make_pair(nextState, unordered_map<char, int>()));
                        transitionsFromCurrentState.insert(make_pair(character, nextState));
                        state = nextState;
                    }
                }

                // The last state that we landed on will be a final state
                finalStates.insert(make_pair(state, rule.first));
            }
        }

        auto GenerateNumberStates() -> void {
            unordered_map<char, int> numberTransitions;
            const int minusState = transitions.at(0).at('-');


            for (const char number : NUMBER_CHARACTERS) {
                // Transitions from the initial state to the NUMBER state
                transitions.at(0).insert(make_pair(number, nextState));

                // Transition from the NUMBER state to the NUMBER state
                numberTransitions.insert(make_pair(number, nextState));

                // Transition from the - state to the NUMBER state via any number (as NUMBER can start with a -)
                transitions.at(minusState).insert(make_pair(number, nextState));
            }

            // Add new state and mark it as the final state for NUMBER
            transitions.insert(make_pair(nextState, numberTransitions));
            finalStates.insert(make_pair(nextState, NUMBER));
        }

        auto RecursiveAddTransitions(const int state) -> void {
            // If the state is not 0 (ie, at least one letter precedes the current state)
            if (state != 0) {
                // Every number from there on can transition to identifier
                for (const char number : NUMBER_CHARACTERS) {
                    // If the letter does not already have a transition from the current state
                    if (transitions.at(state).count(number) == 0) {
                        // Add a transition to IDENTIFIER via that letter
                        transitions.at(state).insert(make_pair(number, nextState));
                    }
                }
            }

            // If the state doesn't have a final state yet, set its final state to be an identifier
            if (finalStates.count(state) == 0) {
                finalStates.insert(make_pair(state, IDENTIFIER));
            }

            for (const char letter : IDENTIFIER_CHARACTERS) {
                // If the letter does not already have a transition from the current state
                if (transitions.at(state).count(letter) == 0) {
                    // Add a transition to IDENTIFIER via that letter
                    transitions.at(state).insert(make_pair(letter, nextState));
                }

                // Recurse to the state if it can be reached through a transition via the letter from the current state
                else {
                    RecursiveAddTransitions(transitions.at(state).at(letter));
                }
            }
        }

        auto GenerateLetterStates() -> void {
            unordered_map<char, int> letterTransitions;

            for (const char letter : IDENTIFIER_CHARACTERS) {
                // Transitions from the IDENTIFIER state to the IDENTIFIER state
                letterTransitions.insert(make_pair(letter, nextState));
            }

            for (const char number : NUMBER_CHARACTERS) {
                // Transitions from the IDENTIFIER state to the IDENTIFIER state
                letterTransitions.insert(make_pair(number, nextState));
            }

            // Add new state and mark it as the final state for IDENTIFIER
            transitions.insert(make_pair(nextState, letterTransitions));
            finalStates.insert(make_pair(nextState, IDENTIFIER));

            RecursiveAddTransitions(0);
        }

        auto MakeAllRemainingFinalStatesNone() -> void {
            for (int state = 0; state < nextState; state++) {
                if (finalStates.count(state) == 0) {
                    finalStates.insert(make_pair(state, NONE));
                }
            }
        }
    }

    auto GenerateStates() -> void {
        // Reset
        transitions = {{0, {}}};
        finalStates = {};
        nextState = 0;

        GenerateRuleStates();
        nextState++;
        GenerateNumberStates();
        nextState++;
        GenerateLetterStates();
        finalStates.at(0) = NONE;
        MakeAllRemainingFinalStatesNone();
    }

    auto Transitions() -> const unordered_map<int, unordered_map<char, int>>& {
        return transitions;
    }

    auto FinalStates() -> const unordered_map<int, Terminal>& {
        return finalStates;
    }
}