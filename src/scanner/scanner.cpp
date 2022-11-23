#include "scanner.hpp"
#include "scanner/tokens.hpp"

#include <iostream>
#include <scanner/States.hpp>



namespace scanner {

    namespace {
        bool error = false;;
        int state = 0;
        int lastFinalState = -1;
        string stateText;
        string lastFinalStateText;
        string lastFinalStateRemainingText;
        vector<Token> tokens;

        auto StepToNextState(string remainingText) -> void {
            unordered_map<char, int> possibleTransitions = transitions.at(state);

            // Get next character
            const char nextCharacter = remainingText.at(0);
            
            // Move state variable to next state if possible
            if (possibleTransitions.count(nextCharacter) == 1) {
                state = possibleTransitions.at(nextCharacter);
                stateText += nextCharacter;

                // If the new state is a comment state, we can discard the rest of the line
                if (state == 108) {
                    state = 0;
                    stateText = "";
                    lastFinalState = -1;
                    lastFinalStateText = "";
                    return;
                }

                // Pop character off the text, since we've moved to the next state
                remainingText = remainingText.substr(1, remainingText.size()-1);
            
            // Otherwise, try to complete the token
            } else {

                // If the token can be completed at the current state, complete the token and add it to the token vector
                if (finalStates.at(state) != NONE) {

                    // If the token type is NO_TOKEN, don't add a token
                    if (finalStates.at(state) != NO_TOKEN) {
                        tokens.push_back(Token{finalStates.at(state), stateText});
                    }

                    // Reset state machine for next cycle
                    state = 0;
                    stateText = "";
                    lastFinalState = -1;
                    lastFinalStateText = "";
                }

                // If not, backtrack to the previous final state and create the token with that
                else {
                    if (lastFinalState != -1) {
                        tokens.push_back(Token{finalStates.at(lastFinalState), lastFinalStateText});
                        remainingText = lastFinalStateRemainingText;

                    // If there's no last final state, we have an error
                    } else {
                        error = true;
                        return;
                    }
                }
            }

            // Check if we're in a potential final state
            if (finalStates.at(state) != NONE) {
                lastFinalState = state;
                lastFinalStateText = stateText;
                lastFinalStateRemainingText = remainingText;
            }

            // Recursive call if we still have text to go through
            if (!remainingText.empty()) {
                StepToNextState(remainingText);
            }
        }
    }


    auto Scan(const vector<string> &lines) -> vector<Token> {
        for (const string &line : lines) {
            StepToNextState(line);
        }
        return tokens;
    }

    auto Reset() -> void {
        error = false;
        state = 0;
        lastFinalState = -1;
        stateText = "";
        lastFinalStateText = "";
        lastFinalStateRemainingText = "";
        tokens = vector<Token> ();
    }

    auto GetError() -> bool {
        return error;
    }
}