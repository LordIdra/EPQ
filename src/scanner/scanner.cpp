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

        auto ResetStateMachine() -> void {
            state = 0;
            stateText = "";
            lastFinalState = -1;
            lastFinalStateText = "";
        }

        auto TransitionPossible(const char character) -> bool {
            unordered_map<char, int> possibleTransitions = transitions.at(state);
            return possibleTransitions.count(character) == 1;
        }

        auto MakeTransition(const char character) -> void {
            unordered_map<char, int> possibleTransitions = transitions.at(state);
            state = possibleTransitions.at(character);
            stateText += character;
        }

        auto IsCommentState() -> bool {
            return state == 108;
        }

        auto IsDeadEndState() -> bool {
            return finalStates.at(state) != NONE;
        }

        auto PopCharacter(string &str) -> void {
            str = str.substr(1, str.size()-1);
        }

        auto ShouldAddToken() -> bool {
            return finalStates.at(state) != NO_TOKEN;
        }

        auto AddToken() -> void {
            tokens.push_back(Token{finalStates.at(state), stateText});
        }

        auto LastFinalStateExists() -> bool {
            return lastFinalState != -1;
        }

        auto AddFinalStateToken() -> void {
            tokens.push_back(Token{finalStates.at(lastFinalState), lastFinalStateText});
        }

        auto IsCurrentStateFinal() -> bool {
            return finalStates.at(state) != NONE;
        }

        auto UpdateLastFinalState(const string &remainingText) -> void {
            lastFinalState = state;
            lastFinalStateText = stateText;
            lastFinalStateRemainingText = remainingText;
        }

        auto StepToNextState(string remainingText) -> void {
            const char character = remainingText.at(0);
            if (TransitionPossible(character)) {
                MakeTransition(character);
                if (IsCommentState()) {
                    ResetStateMachine();
                    return;
                }
                PopCharacter(remainingText);
            } else {
                if (IsDeadEndState()) {
                    if (ShouldAddToken()) {
                       AddToken();
                    }
                    ResetStateMachine();
                } else {
                    if (LastFinalStateExists()) {
                        AddFinalStateToken();
                        remainingText = lastFinalStateRemainingText;
                    } else {
                        error = true;
                        return;
                    }
                }
            }
            if (IsCurrentStateFinal()) {
                UpdateLastFinalState(remainingText);
            }
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