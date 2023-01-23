#include "generator/memory.hpp"
#include "generator/registers.hpp"
#include "grammar/nonTerminals.hpp"
#include "grammar/symbolNames.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/symbolTable.hpp"
#include "util/errors.hpp"
#include <cstdlib>
#include <generator/generator.hpp>

#include <generator/assembly.hpp>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <util/types.hpp>



namespace generator {

    namespace {
        parser::TreeNode *node;
        std::stack<int> registerStack;
        std::stack<string> loopStartStack;
        std::stack<string> loopEndStack;
        SymbolTable symbolTable;

        bool debugMode;

        auto GetChildType(const int i) -> int {
            return node->children.at(i).token.type;
        }

        auto GetChildText(const int i) -> string {
            return node->children.at(i).token.text;
        }

        auto PopRegister() -> int {
            if (debugMode) {
                std::cout << colors::CYAN << symbolNames.at(node->token.type) << colors::AMBER << " popped" << colors::WHITE << "\n";
            }
            if (registerStack.empty()) {
                std::cout << colors::RED << "Attempt to pop off empty register stack" << colors::WHITE << "\n";
                return -1;
            }
            const int r = registerStack.top();
            registerStack.pop();
            registers::Free(r);
            return r;
        }

        auto PushRegister(const int r) -> void {
            if (debugMode) {
                std::cout << colors::CYAN << symbolNames.at(node->token.type) << colors::AMBER << " pushed" << colors::WHITE << "\n";
            }
            if (registerStack.size() >= 9) {
                std::cout << colors::RED << "Attempt to push to full register stack" << colors::WHITE << "\n";
                return;
            }
            registerStack.push(r);
        }

        namespace generate {
            namespace Term_AND {
                auto Last() -> void {
                    // Term_AND -> NONE
                    if (node->children.size() == 1) { return; }

                    // Term_AND -> AND Term_1 Term_AND
                    const int r3 = registers::Allocate();

                    const int r1 = PopRegister();
                    const int r2 = PopRegister();

                    assembly::AND(r1, r2, r3);
                    
                    PushRegister(r3);
                }
            }

            namespace Term_OR {
                auto Last() -> void {
                    // Term_OR -> NONE
                    if (node->children.size() == 1) { return; }

                    // Term_OR -> OR Term_2 Term_OR
                    const int r3 = registers::Allocate();

                    const int r1 = PopRegister();
                    const int r2 = PopRegister();

                    assembly::ORR(r1, r2, r3);

                    PushRegister(r3);
                }

            }

            namespace Term_EQUALS {
                auto Last() -> void {
                    /* OLD ALGORITHM (doesn't really work) */
                        //assembly::SUB(r1, r2, r3);
                        //assembly::SET("1" + labelNext, r5);
                        //assembly::SET("2" + labelNext, r6);
                        //assembly::SET("3" + labelNext, r7);
                        //assembly::Comment("branch to " + labelNext);
                        //assembly::BRP(r5, r6, r7, r3);           // Branch to labelNext if MSB(r1) > MSB(r2)
                        //assembly::SET(registers::FALSE, r4);

                        //assembly::SUB(r2, r1, r3);
                        //assembly::LabelLatestInstruction(labelNext);
                        //assembly::SET("1" + labelTrue, r5);
                        //assembly::SET("2" + labelTrue, r6);
                        //assembly::SET("3" + labelTrue, r7);
                        //assembly::Comment("branch to " + labelTrue);
                        //assembly::BRP(r5, r6, r7, r3);
                        //assembly::SET(registers::FALSE, r4);
                        //assembly::SET("1" + labelEnd, r5);
                        //assembly::SET("2" + labelEnd, r6);
                        //assembly::SET("3" + labelEnd, r7);
                        //assembly::Comment("branch to " + labelEnd);
                        //assembly::BRA(r5, r6, r7);

                        //assembly::SET(registers::TRUE, r4);
                        //assembly::LabelLatestInstruction(labelTrue);

                        //assembly::NOP();
                        //assembly::LabelLatestInstruction(labelEnd);
                    /* END */

                    // Term_EQUALS -> NONE
                    if (node->children.size() == 1) { return; }

                    // Term_EQUALS -> EQUALS Term_3 Term_EQUALS

                    const int r3 = registers::Allocate();
                    const int r4 = registers::Allocate();

                    const int r5 = registers::Allocate();
                    const int r6 = registers::Allocate();
                    const int r7 = registers::Allocate();

                    const int r1 = PopRegister();
                    const int r2 = PopRegister();
                    
                    const string labelMSBTheSame = assembly::GenerateLabel("MSBTheSame");
                    const string labelNotEqual   = assembly::GenerateLabel("NotEqual");
                    const string labelEqual      = assembly::GenerateLabel("Equal");
                    const string labelEnd        = assembly::GenerateLabel("End");

                    // If the MSB of both r2 and r3 are the same (result of XOR is positive), we can just focus on the 3 other bits
                    assembly::XOR(r1, r2, r3);
                    assembly::Comment("branch to " + labelMSBTheSame);
                    assembly::SET("1" + labelMSBTheSame, r5);
                    assembly::SET("2" + labelMSBTheSame, r6);
                    assembly::SET("3" + labelMSBTheSame, r7);
                    assembly::BRP(r5, r6, r7, r3);

                    // We haven't branched, so MSBs are different, so they are not equal
                    assembly::Comment("branch to " + labelNotEqual);
                    assembly::SET("1" + labelNotEqual, r5);
                    assembly::SET("2" + labelNotEqual, r6);
                    assembly::SET("3" + labelNotEqual, r7);
                    assembly::BRA(r5, r6, r7);

                    // MSBs are the same
                    // First, subtract r2 from r1
                    assembly::LabelLatestInstruction(labelMSBTheSame);
                    assembly::SUB(r1, r2, r3);
                    // We know that if they are equal, they will either be 1000 or 0000
                    // Let's first set the MSB to 0 by running and AND operation with 0111
                    assembly::SET(7, r4);
                    assembly::AND(r3, r4, r3);
                    assembly::SET(1, r4);
                    // Now, if we subtract 0001, the result will only be negative if we started with 0000
                    assembly::SUB(r3, r4, r3);
                    // If the result is positive, they are not equal
                    assembly::Comment("branch to " + labelNotEqual);
                    assembly::SET("1" + labelNotEqual, r5);
                    assembly::SET("2" + labelNotEqual, r6);
                    assembly::SET("3" + labelNotEqual, r7);
                    assembly::BRP(r5, r6, r7, r3);

                    // We haven't branched, so they are equal
                    assembly::SET(registers::TRUE, r3);
                    assembly::Comment("branch to " + labelEnd);
                    assembly::SET("1" + labelEnd, r5);
                    assembly::SET("2" + labelEnd, r6);
                    assembly::SET("3" + labelEnd, r7);
                    assembly::BRA(r5, r6, r7);

                    // This is where we branch to if they are not equal
                    assembly::LabelLatestInstruction(labelNotEqual);
                    assembly::SET(registers::FALSE, r3);

                    // And this is the end of the whole thing
                    assembly::LabelLatestInstruction(labelEnd);
                    assembly::NOP();

                    PushRegister(r3);

                    registers::Free(r4);
                    registers::Free(r5);
                    registers::Free(r6);
                    registers::Free(r7);
                }

            }

            namespace Term_NOT_EQUALS {
                auto Last() -> void {
                    // Term_NOT_EQUALS -> NONE
                    if (node->children.size() == 1) { return; }

                    // Term_NOT_EQUALS -> NOT_EQUALS Term_4 Term_NOT_EQUALS
                    const int r3 = registers::Allocate();
                    const int r4 = registers::Allocate();

                    const int r5 = registers::Allocate();
                    const int r6 = registers::Allocate();
                    const int r7 = registers::Allocate();

                    const int r1 = PopRegister();
                    const int r2 = PopRegister();

                    const string labelMSBTheSame = assembly::GenerateLabel("MSBTheSame");
                    const string labelNotEqual   = assembly::GenerateLabel("NotEqual");
                    const string labelEqual      = assembly::GenerateLabel("Equal");
                    const string labelEnd        = assembly::GenerateLabel("End");

                    // If the MSB of both r2 and r3 are the same (result of XOR is positive), we can just focus on the 3 other bits
                    assembly::XOR(r1, r2, r3);
                    assembly::Comment("branch to " + labelMSBTheSame);
                    assembly::SET("1" + labelMSBTheSame, r5);
                    assembly::SET("2" + labelMSBTheSame, r6);
                    assembly::SET("3" + labelMSBTheSame, r7);
                    assembly::BRP(r5, r6, r7, r3);

                    // We haven't branched, so MSBs are different, so they are not equal
                    assembly::Comment("branch to " + labelNotEqual);
                    assembly::SET("1" + labelNotEqual, r5);
                    assembly::SET("2" + labelNotEqual, r6);
                    assembly::SET("3" + labelNotEqual, r7);
                    assembly::BRA(r5, r6, r7);

                    // MSBs are the same
                    // First, subtract r2 from r1
                    assembly::LabelLatestInstruction(labelMSBTheSame);
                    assembly::SUB(r1, r2, r3);
                    // We know that if they are equal, they will either be 1000 or 0000
                    // Let's first set the MSB to 0 by running and AND operation with 0111
                    assembly::SET(7, r4);
                    assembly::AND(r3, r4, r3);
                    // Now, if we subtract 0001, the result will only be negative if we started with 0000
                    assembly::SET(1, r4);
                    assembly::SUB(r3, r4, r3);
                    // If the result is positive, they are not equal
                    assembly::Comment("branch to " + labelNotEqual);
                    assembly::SET("1" + labelNotEqual, r5);
                    assembly::SET("2" + labelNotEqual, r6);
                    assembly::SET("3" + labelNotEqual, r7);
                    assembly::BRP(r5, r6, r7, r3);

                    // We haven't branched, so they are equal
                    assembly::SET(registers::FALSE, r3);
                    assembly::Comment("branch to " + labelEnd);
                    assembly::SET("1" + labelEnd, r5);
                    assembly::SET("2" + labelEnd, r6);
                    assembly::SET("3" + labelEnd, r7);
                    assembly::BRA(r5, r6, r7);

                    // This is where we branch to if they are not equal
                    assembly::LabelLatestInstruction(labelNotEqual);
                    assembly::SET(registers::TRUE, r3);

                    // And this is the end of the whole thing
                    assembly::LabelLatestInstruction(labelEnd);
                    assembly::NOP();

                    PushRegister(r3);

                    registers::Free(r4);
                    registers::Free(r5);
                    registers::Free(r6);
                    registers::Free(r7);
                }
            }

            namespace Term_GREATER {
                auto Last() -> void {
                    // Term_GREATER -> NONE
                    if (node->children.size() == 1) { return; }

                    const int r3 = registers::Allocate();
                    const int r4 = registers::Allocate();

                    const int r5 = registers::Allocate();
                    const int r6 = registers::Allocate();
                    const int r7 = registers::Allocate();

                    // We're checking if r1 > r2 but the order is inverted when we pop
                    const int r2 = PopRegister();
                    const int r1 = PopRegister();

                    const string labelMSBTheSame      = assembly::GenerateLabel("MSBTheSame");
                    const string labelR1Greater       = assembly::GenerateLabel("R1Greater");
                    const string labelR1Lower         = assembly::GenerateLabel("R1Lower");
                    const string labelEnd             = assembly::GenerateLabel("End");

                    // If the MSB of both r2 and r3 are the same (result of XOR is positive), we can just focus on the 3 other bits
                    assembly::XOR(r1, r2, r3);
                    assembly::Comment("branch to " + labelMSBTheSame);
                    assembly::SET("1" + labelMSBTheSame, r5);
                    assembly::SET("2" + labelMSBTheSame, r6);
                    assembly::SET("3" + labelMSBTheSame, r7);
                    assembly::BRP(r5, r6, r7, r3);

                    // We haven't branched, so MSBs are different
                    // if r1 is positive, r1 > r2
                    assembly::Comment("branch to " + labelR1Greater);
                    assembly::SET("1" + labelR1Greater, r5);
                    assembly::SET("2" + labelR1Greater, r6);
                    assembly::SET("3" + labelR1Greater, r7);
                    assembly::BRP(r5, r6, r7, r1);

                    // We again haven't branched, so r1 is negative, so r1 < r2
                    assembly::Comment("branch to " + labelR1Lower);
                    assembly::SET("1" + labelR1Lower, r5);
                    assembly::SET("2" + labelR1Lower, r6);
                    assembly::SET("3" + labelR1Lower, r7);
                    assembly::BRA(r5, r6, r7);

                    // MSBs are the same; let's compare the three remaining bits
                    // Let's first set the MSB of both bits to 0 by running and AND operation with 0111
                    assembly::LabelLatestInstruction(labelMSBTheSame);
                    assembly::SET(7, r4);
                    assembly::AND(r1, r4, r1);
                    assembly::AND(r2, r4, r2);
                    // Now let's subtract r2 from r1
                    assembly::SUB(r1, r2, r1);
                    // Subtract a further 1 so that the result is negative (false) if the two numbers are equal
                    assembly::SET(1, r4);
                    assembly::SUB(r1, r4, r1);

                    // If the result is positive, r1 > r2
                    assembly::Comment("branch to " + labelR1Greater);
                    assembly::SET("1" + labelR1Greater, r5);
                    assembly::SET("2" + labelR1Greater, r6);
                    assembly::SET("3" + labelR1Greater, r7);
                    assembly::BRP(r5, r6, r7, r1);

                    // If the result is negative, r1 < r2
                    assembly::Comment("branch to " + labelR1Lower);
                    assembly::SET("1" + labelR1Lower, r5);
                    assembly::SET("2" + labelR1Lower, r6);
                    assembly::SET("3" + labelR1Lower, r7);
                    assembly::BRA(r5, r6, r7);

                    // r1 > r2
                    assembly::LabelLatestInstruction(labelR1Greater);
                    assembly::SET(registers::TRUE, r3);
                    assembly::Comment("branch to " + labelEnd);
                    assembly::SET("1" + labelEnd, r5);
                    assembly::SET("2" + labelEnd, r6);
                    assembly::SET("3" + labelEnd, r7);
                    assembly::BRA(r5, r6, r7);

                    // r1 < r2
                    assembly::LabelLatestInstruction(labelR1Lower);
                    assembly::SET(registers::FALSE, r3);
                    
                    // end
                    assembly::LabelLatestInstruction(labelEnd);
                    assembly::NOP();

                    PushRegister(r3);

                    registers::Free(r4);
                    registers::Free(r5);
                    registers::Free(r6);
                    registers::Free(r7);
                }
            }

            namespace Term_GREATER_OR_EQUAL {
                auto Last() -> void {
                    // Term_GREATER_OR_EQUAL -> NONE
                    if (node->children.size() == 1) { return; }

                    const int r3 = registers::Allocate();
                    const int r4 = registers::Allocate();

                    const int r5 = registers::Allocate();
                    const int r6 = registers::Allocate();
                    const int r7 = registers::Allocate();

                    // We're checking if r1 > r2 but the order is inverted when we pop
                    const int r2 = PopRegister();
                    const int r1 = PopRegister();

                    const string labelMSBTheSame      = assembly::GenerateLabel("MSBTheSame");
                    const string labelR1Greater       = assembly::GenerateLabel("R1Greater");
                    const string labelR1Lower         = assembly::GenerateLabel("R1Lower");
                    const string labelEnd             = assembly::GenerateLabel("End");

                    // If the MSB of both r2 and r3 are the same (result of XOR is positive), we can just focus on the 3 other bits
                    assembly::XOR(r1, r2, r3);
                    assembly::Comment("branch to " + labelMSBTheSame);
                    assembly::SET("1" + labelMSBTheSame, r5);
                    assembly::SET("2" + labelMSBTheSame, r6);
                    assembly::SET("3" + labelMSBTheSame, r7);
                    assembly::BRP(r5, r6, r7, r3);

                    // We haven't branched, so MSBs are different
                    // if r1 is positive, r1 > r2
                    assembly::Comment("branch to " + labelR1Greater);
                    assembly::SET("1" + labelR1Greater, r5);
                    assembly::SET("2" + labelR1Greater, r6);
                    assembly::SET("3" + labelR1Greater, r7);
                    assembly::BRP(r5, r6, r7, r1);

                    // We again haven't branched, so r1 is negative, so r1 < r2
                    assembly::Comment("branch to " + labelR1Lower);
                    assembly::SET("1" + labelR1Lower, r5);
                    assembly::SET("2" + labelR1Lower, r6);
                    assembly::SET("3" + labelR1Lower, r7);
                    assembly::BRA(r5, r6, r7);

                    // MSBs are the same; let's compare the three remaining bits
                    // Let's first set the MSB of both bits to 0 by running and AND operation with 0111
                    assembly::LabelLatestInstruction(labelMSBTheSame);
                    assembly::SET(7, r4);
                    assembly::AND(r1, r4, r1);
                    assembly::AND(r2, r4, r2);
                    // Now let's subtract r2 from r1
                    assembly::SUB(r1, r2, r1);

                    // If the result is positive, r1 > r2
                    assembly::Comment("branch to " + labelR1Greater);
                    assembly::SET("1" + labelR1Greater, r5);
                    assembly::SET("2" + labelR1Greater, r6);
                    assembly::SET("3" + labelR1Greater, r7);
                    assembly::BRP(r5, r6, r7, r1);

                    // If the result is negative, r1 < r2
                    assembly::Comment("branch to " + labelR1Lower);
                    assembly::SET("1" + labelR1Lower, r5);
                    assembly::SET("2" + labelR1Lower, r6);
                    assembly::SET("3" + labelR1Lower, r7);
                    assembly::BRA(r5, r6, r7);

                    // r1 > r2
                    assembly::LabelLatestInstruction(labelR1Greater);
                    assembly::SET(registers::TRUE, r3);
                    assembly::Comment("branch to " + labelEnd);
                    assembly::SET("1" + labelEnd, r5);
                    assembly::SET("2" + labelEnd, r6);
                    assembly::SET("3" + labelEnd, r7);
                    assembly::BRA(r5, r6, r7);

                    // r1 < r2
                    assembly::LabelLatestInstruction(labelR1Lower);
                    assembly::SET(registers::FALSE, r3);

                    // end
                    assembly::LabelLatestInstruction(labelEnd);
                    assembly::NOP();

                    PushRegister(r3);

                    registers::Free(r4);
                    registers::Free(r5);
                    registers::Free(r6);
                    registers::Free(r7);
                }
            }

            namespace Term_LESS {
                auto Last() -> void {
                    // Term_LESS -> NONE
                    if (node->children.size() == 1) { return; }

                    const int r3 = registers::Allocate();
                    const int r4 = registers::Allocate();

                    const int r5 = registers::Allocate();
                    const int r6 = registers::Allocate();
                    const int r7 = registers::Allocate();

                    // We're checking if r1 > r2 but the order is inverted when we pop
                    const int r2 = PopRegister();
                    const int r1 = PopRegister();

                    const string labelMSBTheSame      = assembly::GenerateLabel("MSBTheSame");
                    const string labelR1Greater       = assembly::GenerateLabel("R1Greater");
                    const string labelR1Lower         = assembly::GenerateLabel("R1Lower");
                    const string labelEnd             = assembly::GenerateLabel("End");

                    // If the MSB of both r2 and r3 are the same (result of XOR is positive), we can just focus on the 3 other bits
                    assembly::XOR(r1, r2, r3);
                    assembly::Comment("branch to " + labelMSBTheSame);
                    assembly::SET("1" + labelMSBTheSame, r5);
                    assembly::SET("2" + labelMSBTheSame, r6);
                    assembly::SET("3" + labelMSBTheSame, r7);
                    assembly::BRP(r5, r6, r7, r3);

                    // We haven't branched, so MSBs are different
                    // if r1 is positive, r1 > r2
                    assembly::Comment("branch to " + labelR1Greater);
                    assembly::SET("1" + labelR1Greater, r5);
                    assembly::SET("2" + labelR1Greater, r6);
                    assembly::SET("3" + labelR1Greater, r7);
                    assembly::BRP(r5, r6, r7, r1);

                    // We again haven't branched, so r1 is negative, so r1 < r2
                    assembly::Comment("branch to " + labelR1Lower);
                    assembly::SET("1" + labelR1Lower, r5);
                    assembly::SET("2" + labelR1Lower, r6);
                    assembly::SET("3" + labelR1Lower, r7);
                    assembly::BRA(r5, r6, r7);

                    // MSBs are the same; let's compare the three remaining bits
                    // Let's first set the MSB of both bits to 0 by running and AND operation with 0111
                    assembly::LabelLatestInstruction(labelMSBTheSame);
                    assembly::SET(7, r4);
                    assembly::AND(r1, r4, r1);
                    assembly::AND(r2, r4, r2);
                    // Now let's subtract r2 from r1
                    assembly::SUB(r1, r2, r1);

                    // If the result is positive, r1 > r2
                    assembly::Comment("branch to " + labelR1Greater);
                    assembly::SET("1" + labelR1Greater, r5);
                    assembly::SET("2" + labelR1Greater, r6);
                    assembly::SET("3" + labelR1Greater, r7);
                    assembly::BRP(r5, r6, r7, r1);

                    // If the result is negative, r1 < r2
                    assembly::Comment("branch to " + labelR1Lower);
                    assembly::SET("1" + labelR1Lower, r5);
                    assembly::SET("2" + labelR1Lower, r6);
                    assembly::SET("3" + labelR1Lower, r7);
                    assembly::BRA(r5, r6, r7);

                    // r1 > r2
                    assembly::LabelLatestInstruction(labelR1Greater);
                    assembly::SET(registers::FALSE, r3);
                    assembly::Comment("branch to " + labelEnd);
                    assembly::SET("1" + labelEnd, r5);
                    assembly::SET("2" + labelEnd, r6);
                    assembly::SET("3" + labelEnd, r7);
                    assembly::BRA(r5, r6, r7);

                    // r1 < r2
                    assembly::LabelLatestInstruction(labelR1Lower);
                    assembly::SET(registers::TRUE, r3);

                    // end
                    assembly::LabelLatestInstruction(labelEnd);
                    assembly::NOP();

                    PushRegister(r3);

                    registers::Free(r4);
                    registers::Free(r5);
                    registers::Free(r6);
                    registers::Free(r7);
                }
            }

            namespace Term_LESS_OR_EQUAL {
                auto Last() -> void {
                    // Term_LESS_OR_EQUAL -> NONE
                    if (node->children.size() == 1) { return; }

                    const int r3 = registers::Allocate();
                    const int r4 = registers::Allocate();

                    const int r5 = registers::Allocate();
                    const int r6 = registers::Allocate();
                    const int r7 = registers::Allocate();

                    // We're checking if r1 > r2 but the order is inverted when we pop
                    const int r2 = PopRegister();
                    const int r1 = PopRegister();

                    const string labelMSBTheSame      = assembly::GenerateLabel("MSBTheSame");
                    const string labelR1Greater       = assembly::GenerateLabel("R1Greater");
                    const string labelR1Lower         = assembly::GenerateLabel("R1Lower");
                    const string labelEnd             = assembly::GenerateLabel("End");

                    // If the MSB of both r2 and r3 are the same (result of XOR is positive), we can just focus on the 3 other bits
                    assembly::XOR(r1, r2, r3);
                    assembly::Comment("branch to " + labelMSBTheSame);
                    assembly::SET("1" + labelMSBTheSame, r5);
                    assembly::SET("2" + labelMSBTheSame, r6);
                    assembly::SET("3" + labelMSBTheSame, r7);
                    assembly::BRP(r5, r6, r7, r3);

                    // We haven't branched, so MSBs are different
                    // if r1 is positive, r1 > r2
                    assembly::Comment("branch to " + labelR1Greater);
                    assembly::SET("1" + labelR1Greater, r5);
                    assembly::SET("2" + labelR1Greater, r6);
                    assembly::SET("3" + labelR1Greater, r7);
                    assembly::BRP(r5, r6, r7, r1);

                    // We again haven't branched, so r1 is negative, so r1 < r2
                    assembly::Comment("branch to " + labelR1Lower);
                    assembly::SET("1" + labelR1Lower, r5);
                    assembly::SET("2" + labelR1Lower, r6);
                    assembly::SET("3" + labelR1Lower, r7);
                    assembly::BRA(r5, r6, r7);

                    // MSBs are the same; let's compare the three remaining bits
                    // Let's first set the MSB of both bits to 0 by running and AND operation with 0111
                    assembly::LabelLatestInstruction(labelMSBTheSame);
                    assembly::SET(7, r4);
                    assembly::AND(r1, r4, r1);
                    assembly::AND(r2, r4, r2);
                    // Now let's subtract r2 from r1
                    assembly::SUB(r1, r2, r1);
                    // Subtract a further 1 so that the result is negative if the two numbers are equal
                    assembly::SET(1, r4);
                    assembly::SUB(r1, r4, r1);

                    // If the result is positive, r1 > r2
                    assembly::Comment("branch to " + labelR1Greater);
                    assembly::SET("1" + labelR1Greater, r5);
                    assembly::SET("2" + labelR1Greater, r6);
                    assembly::SET("3" + labelR1Greater, r7);
                    assembly::BRP(r5, r6, r7, r1);

                    // If the result is negative, r1 < r2
                    assembly::Comment("branch to " + labelR1Lower);
                    assembly::SET("1" + labelR1Lower, r5);
                    assembly::SET("2" + labelR1Lower, r6);
                    assembly::SET("3" + labelR1Lower, r7);
                    assembly::BRA(r5, r6, r7);

                    // r1 > r2
                    assembly::LabelLatestInstruction(labelR1Greater);
                    assembly::SET(registers::FALSE, r3);
                    assembly::Comment("branch to " + labelEnd);
                    assembly::SET("1" + labelEnd, r5);
                    assembly::SET("2" + labelEnd, r6);
                    assembly::SET("3" + labelEnd, r7);
                    assembly::BRA(r5, r6, r7);

                    // r1 < r2
                    assembly::LabelLatestInstruction(labelR1Lower);
                    assembly::SET(registers::TRUE, r3);

                    // end
                    assembly::LabelLatestInstruction(labelEnd);
                    assembly::NOP();

                    PushRegister(r3);

                    registers::Free(r4);
                    registers::Free(r5);
                    registers::Free(r6);
                    registers::Free(r7);
                }
            }

            namespace Term_SUB {
                auto Last() -> void {
                    // Term_SUB -> NONE
                    if (node->children.size() == 1) { return; }

                    // Term_SUB -> SUB Term_9 Term_SUB
                    const int r3 = registers::Allocate();

                    const int r1 = PopRegister();
                    const int r2 = PopRegister();

                    assembly::SUB(r2, r1, r3);

                    PushRegister(r3);
                }
            }

            namespace Term_ADD {
                auto Last() -> void {
                    // Term_ADD -> NONE
                    if (node->children.size() == 1) { return; }

                    // Term_ADD -> ADD Term_10 Term_ADD
                    const int r3 = registers::Allocate();

                    const int r1 = PopRegister();
                    const int r2 = PopRegister();

                    assembly::ADD(r2, r1, r3);

                    PushRegister(r3);
                }
            }

            namespace Term_MULTIPLY {
                auto Last() -> void {
                    // Term_ADD -> NONE
                    if (node->children.size() == 1) { return; }

                    // Term_MULTIPLY -> MULTIPLY Term_11 Term_MULTIPLY
                    const string labelMultiplyLoop = assembly::GenerateLabel("multiplyLoop");

                    const int r3 = registers::Allocate();
                    const int r4 = registers::Allocate();

                    const int r5 = registers::Allocate();
                    const int r6 = registers::Allocate();
                    const int r7 = registers::Allocate();

                    const int r1 = PopRegister();
                    const int r2 = PopRegister();

                    assembly::SET(1, r3);
                    assembly::SET(0, r4);
                    assembly::SUB(r2, r3, r2);
                    assembly::LabelLatestInstruction(labelMultiplyLoop);
                    assembly::SUB(r2, r3, r2);
                    assembly::ADD(r4, r1, r4);
                    assembly::Comment("branch to " + labelMultiplyLoop);
                    assembly::SET("1" + labelMultiplyLoop, r5);
                    assembly::SET("2" + labelMultiplyLoop, r6);
                    assembly::SET("3" + labelMultiplyLoop, r7);
                    assembly::BRP(r5, r6, r7, r2);

                    PushRegister(r4);

                    registers::Free(r3);
                    registers::Free(r5);
                    registers::Free(r6);
                    registers::Free(r7);
                }
            }

            namespace Term_DIVIDE {
                auto Last() -> void {
                    // Term_DIVIDE -> NONE
                    if (node->children.size() == 1) { return; }

                    // Term_DIVIDE -> DIVIDE Term_12 Term_DIVIDE
                    const string labelDivideLoop = assembly::GenerateLabel("divideLoop");

                    const int r3 = registers::Allocate();
                    const int r4 = registers::Allocate();

                    const int r5 = registers::Allocate();
                    const int r6 = registers::Allocate();
                    const int r7 = registers::Allocate();

                    const int r2 = PopRegister();
                    const int r1 = PopRegister();

                    // r1 / r2
                    assembly::SET(1, r3);
                    assembly::SET(0, r4);
                    assembly::SUB(r4, r3, r4);
                    assembly::LabelLatestInstruction(labelDivideLoop);
                    assembly::SUB(r1, r2, r1);
                    assembly::ADD(r4, r3, r4);
                    assembly::Comment("branch to " + labelDivideLoop);
                    assembly::SET("1" + labelDivideLoop, r5);
                    assembly::SET("2" + labelDivideLoop, r6);
                    assembly::SET("3" + labelDivideLoop, r7);
                    assembly::BRP(r5, r6, r7, r1);

                    PushRegister(r4);

                    registers::Free(r3);
                    registers::Free(r5);
                    registers::Free(r6);
                    registers::Free(r7);
                }
            }

            namespace Term_MODULUS {
                auto Last() -> void {
                    // Term_MODULUS -> NONE
                    if (node->children.size() == 1) { return; }

                    // Term_MODULUS -> MODULUS Term_12 Term_MODULUS
                    // TODO (algorithm)
                }

            }

            // Term
            // Term_0
            // Term_1
            // Term_2
            // Term_3
            // Term_4
            // Term_5
            // Term_6
            // Term_7
            // Term_8
            // Term_9
            // Term_10
            // Term_11
            // Term_12
            // Term_13
            // Term_14

            // Datatype
            // VoidableDatatype

            namespace Reference {
                // TODO (12-bit datatype)
                auto First() -> void {}
                auto Last() -> void {}
            }

            namespace Dereference {
                // TODO (12-bit datatype)
                auto First() -> void {}
                auto Last() -> void {}
            }

            namespace ListIndex {
                // TODO (lists)
                auto First() -> void {}
                auto Last() -> void {}
            }

            namespace Listindex {
                // TODO (12-bit datatype)
                auto First() -> void {}
                auto Last() -> void {}
            }

            // Value
            namespace Variable {
                auto First() -> void {
                    // Variable -> Dereference
                    if (node->children.size() == 1) {}

                    // Variable -> IDENTIFIER IdentifierSuffix
                    else {
                        // IdentifierSuffix -> NONE
                        // Variable -> IDENTIFIER
                        if (node->children.at(1).children.at(0).token.type == NONE) {
                            const string identifier = GetChildText(0);

                            const int address = symbolTable.LookupAllScopes(identifier).address;

                            const int a1 = div(address, 256).quot;
                            const int a2 = div(div(address, 256).rem, 16).quot;
                            const int a3 = div(div(address, 256).rem, 16).rem;

                            const int r1 = registers::Allocate();
                            const int r2 = registers::Allocate();
                            const int r3 = registers::Allocate();

                            const int r4 = registers::Allocate();

                            assembly::SET(a1, r1);
                            assembly::SET(a2, r2);
                            assembly::SET(a3, r3);

                            assembly::Comment("load " + identifier);
                            assembly::LDA(r1, r2, r3);
                            assembly::MOV(registers::MDR1, r4);

                            PushRegister(r4);

                            registers::Free(r1);
                            registers::Free(r2);
                            registers::Free(r3);
                        }

                        // IdentifierSuffix -> ListIndex
                        // Variable -> IDENTIFIER ListIndex
                        else {
                            // TODO (12-bit datatype)
                        }
                    }
                }
            }

            namespace IdentifierSuffix {
                // TODO (lists)
                auto First() -> void {}
                auto Last() -> void {}
            }

            namespace Literal {
                auto First() -> void {
                    const int r1 = registers::Allocate();

                    // Literal -> NUMBER
                    if (GetChildType(0) == NUMBER) {
                        int v1 = std::atoi(GetChildText(0).c_str());
                        if (v1 < 0) {
                            v1 = 16 + v1;
                        }
                        assembly::SET(v1, r1);
                    }

                    // Literal -> TRUE
                    if (GetChildType(0) == TRUE) {
                        assembly::SET(registers::TRUE, r1);
                    }

                    // Literal -> FALSE
                    if (GetChildType(0) == FALSE) {
                        assembly::SET(registers::FALSE, r1);
                    }

                    PushRegister(r1);
                }
            }

            namespace InputTerm {
                auto First() -> void {

                    // Literal -> INPUT
                    if (GetChildType(0) == INPUT) {
                        // TODO (new CPU section)
                    }
                }
            }

            namespace AssignmentOperation {
                auto Last() -> void {
                    // AssignmentOperation -> ASSIGN InputTerm
                    if (GetChildType(0) == ASSIGN) {
                        const int assignFrom = PopRegister();
                        const int assignTo = registerStack.top();
                        assembly::MOV(assignFrom, assignTo);
                        return;
                    }

                    // AssignmentOperation -> ADD_ASSIGN Term
                    if (GetChildType(0) == ADD_ASSIGN) {
                        const int assignFrom = PopRegister();
                        const int assignTo = registerStack.top();
                        assembly::ADD(assignTo, assignFrom, assignTo);
                        return;
                    }

                    // AssignmentOperation -> SUBTRACT_ASSIGN Term
                    if (GetChildType(0) == SUBTRACT_ASSIGN) {
                        const int assignFrom = PopRegister();
                        const int assignTo = registerStack.top();
                        assembly::SUB(assignTo, assignFrom, assignTo);
                        return;
                    }

                    // AssignmentOperation -> MULTIPLY_ASSIGN Term
                    if (GetChildType(0) == MULTIPLY_ASSIGN) {
                        const string labelMultiplyLoop = assembly::GenerateLabel("multiplyLoop");

                        const int r5 = registers::Allocate();
                        const int r6 = registers::Allocate();

                        const int r7 = registers::Allocate();
                        const int r8 = registers::Allocate();
                        const int r9 = registers::Allocate();

                        const int assignFrom = PopRegister();
                        const int assignTo = registerStack.top();

                        assembly::SET(1, r5);
                        assembly::SET(0, r6);
                        assembly::SUB(assignTo, r5, assignTo);
                        assembly::LabelLatestInstruction(labelMultiplyLoop);
                        assembly::SUB(assignTo, r5, assignTo);
                        assembly::ADD(r6, assignFrom, r6);
                        assembly::Comment("branch to " + labelMultiplyLoop);
                        assembly::SET("1" + labelMultiplyLoop, r7);
                        assembly::SET("2" + labelMultiplyLoop, r8);
                        assembly::SET("3" + labelMultiplyLoop, r9);
                        assembly::BRP(r7, r8, r9, assignTo);
                        assembly::MOV(r6, assignTo);

                        registers::Free(r5);
                        registers::Free(r6);
                        registers::Free(r7);
                        registers::Free(r8);
                        registers::Free(r9);

                        return;
                    }

                    // AssignmentOperation -> DIVIDE_ASSIGN Term
                    if (GetChildType(0) == DIVIDE_ASSIGN) {
                        // TODO (algorithm)
                        return;
                    }

                    // AssignmentOperation -> MODULUS_ASSIGN Term
                    if (GetChildType(0) == MODULUS_ASSIGN) {
                        // TODO (algorithm)
                        return;
                    }
                }
            }

            namespace Assignment {
                auto Last() -> void {
                    // Assignment -> Variable AssignmentOperation
                    {
                        // Variable -> IDENTIFIER IdentifierSuffix
                        // Assignment -> IDENTTIFIER IdentifierSuffix AssignmentOperation
                        if (node->children.at(0).children.at(0).token.type == IDENTIFIER) {
                            const int r1 = registers::Allocate();
                            const int r2 = registers::Allocate();
                            const int r3 = registers::Allocate();
                            
                            const int r4 = PopRegister();

                            const string identifier = node->children.at(0).children.at(0).token.text;

                            const int address = symbolTable.LookupAllScopes(identifier).address;

                            const int a1 = div(address, 256).quot;
                            const int a2 = div(div(address, 256).rem, 16).quot;
                            const int a3 = div(div(address, 256).rem, 16).rem;

                            assembly::SET(a1, r1);
                            assembly::SET(a2, r2);
                            assembly::SET(a3, r3);

                            assembly::MOV(r4, registers::MDR1);
                            assembly::Comment("store " + identifier);
                            assembly::STA(r1, r2, r3);

                            registers::Free(r1);
                            registers::Free(r2);
                            registers::Free(r3);
                        }

                        // Variable -> Dereference
                        // Assignment -> Dereference AssignmentOperation
                        else {
                            // TODO (12-bit datatype)
                        }
                    }
                }
            }

            // Declaration_0

            namespace Declaration {
                auto Last() -> void {
                    // Declaration -> Datatype Declaration_0
                    {
                        // Declaration_0 -> OPEN_SQUARE_BRACKET NUMBER CLOSE_SQUARE_BRACKET IDENTIFIER
                        // Declaration -> Datatype OPEN_SQUARE_BRACKET NUMBER CLOSE_SQUARE_BRACKET IDENTIFIER
                        if (node->children.at(1).children.at(0).token.type == OPEN_SQUARE_BRACKET) {
                            // TODO (lists)
                        }

                        // Declaration_0 -> POINTER IDENTIFIER ASSIGN Reference
                        // Declaration -> Datatype POINTER IDENTIFIER ASSIGN Reference
                        if (node->children.at(1).children.at(0).token.type == POINTER) {
                            // TODO (12-bit datatype)
                        }

                        // Declaration_0 -> IDENTIFIER ASSIGN InputTerm
                        // Declaration -> Datatype IDENTIFIER ASSIGN InputTerm
                        if (node->children.at(1).children.at(0).token.type == IDENTIFIER) {
                            const int r1 = registers::Allocate();
                            const int r2 = registers::Allocate();
                            const int r3 = registers::Allocate();

                            const int r4 = PopRegister();
                            
                            const string identifier = node->children.at(1).children.at(0).token.text;

                            const int address = symbolTable.LookupAllScopes(identifier).address;

                            const int a1 = div(address, 256).quot;
                            const int a2 = div(div(address, 256).rem, 16).quot;
                            const int a3 = div(div(address, 256).rem, 16).rem;
                            
                            assembly::SET(a1, r1);
                            assembly::SET(a2, r2);
                            assembly::SET(a3, r3);

                            assembly::MOV(r4, registers::MDR1);
                            assembly::Comment("store " + identifier);
                            assembly::STA(r1, r2, r3);

                            registers::Free(r1);
                            registers::Free(r2);
                            registers::Free(r3);
                        }
                    }
                }

            }

            namespace SimpleStatement {
                auto First() -> void {
                    // SimpleStatement -> OUTPUT Term SEMICOLON
                    if (GetChildType(0) != OUTPUT) {
                        // TODO (new CPU section)
                    }
                }
            }

            namespace LoopCondition {
                auto First() -> void {
                    loopStartStack.push(assembly::GenerateLabel("loopStart"));
                    loopEndStack.push(assembly::GenerateLabel("loopEnd"));

                    assembly::NOP();
                    assembly::LabelLatestInstruction(loopStartStack.top());
                }

                auto Last() -> void {
                    const int r1 = PopRegister();

                    const int r2 = registers::Allocate();
                    const int r3 = registers::Allocate();
                    const int r4 = registers::Allocate();

                    assembly::NOT(r1, r1);
                    assembly::SET("1" + loopEndStack.top(), r2);
                    assembly::SET("2" + loopEndStack.top(), r3);
                    assembly::SET("3" + loopEndStack.top(), r4);
                    assembly::Comment("branch to " + loopEndStack.top());
                    assembly::BRP(r2, r3, r4, r1);

                    registers::Free(r2);
                    registers::Free(r3);
                    registers::Free(r4);
                }
            }

            namespace For {
                auto Last() -> void {
                    const int r1 = registers::Allocate();
                    const int r2 = registers::Allocate();
                    const int r3 = registers::Allocate();

                    assembly::SET("1" + loopStartStack.top(), r1);
                    assembly::SET("2" + loopStartStack.top(), r2);
                    assembly::SET("3" + loopStartStack.top(), r3);
                    assembly::Comment("branch to " + loopStartStack.top());
                    assembly::BRA(r1, r2, r3);
                    assembly::NOP();
                    assembly::LabelLatestInstruction(loopEndStack.top());

                    registers::Free(r1);
                    registers::Free(r2);
                    registers::Free(r3);

                    loopStartStack.pop();
                    loopEndStack.pop();
                }
            }

            namespace While {
                auto Last() -> void {
                    const int r1 = registers::Allocate();
                    const int r2 = registers::Allocate();
                    const int r3 = registers::Allocate();

                    assembly::SET("1" + loopStartStack.top(), r1);
                    assembly::SET("2" + loopStartStack.top(), r2);
                    assembly::SET("3" + loopStartStack.top(), r3);
                    assembly::Comment("branch to " + loopStartStack.top());
                    assembly::BRA(r1, r2, r3);
                    assembly::NOP();
                    assembly::LabelLatestInstruction(loopEndStack.top());

                    registers::Free(r1);
                    registers::Free(r2);
                    registers::Free(r3);

                    loopStartStack.pop();
                    loopEndStack.pop();
                }
            }

            namespace Parameter {
                auto First() -> void {
                    const string identifier = node->children.at(1).token.text;

                    const int address = symbolTable.LookupAllScopes(identifier).address;

                    const int a1 = div(address, 256).quot;
                    const int a2 = div(div(address, 256).rem, 16).quot;
                    const int a3 = div(div(address, 256).rem, 16).rem;

                    const int r1 = registers::Allocate();
                    const int r2 = registers::Allocate();
                    const int r3 = registers::Allocate();

                    assembly::SET(a1, r1);
                    assembly::SET(a2, r2);
                    assembly::SET(a3, r3);

                    assembly::POP();
                    assembly::Comment("load " + identifier);
                    assembly::LDA(r1, r2, r3);

                    registers::Free(r1);
                    registers::Free(r2);
                    registers::Free(r3);
                }
            }

            // NextParameter
            // ParameterList_0
            // ParameterList_1

            namespace FunctionDeclaration {

                auto First() -> void {
                    // FunctionDeclaration -> VoidableDatatype IDENTIFIER ParameterList_1 N_Block FunctionDeclaration

                    if (node->children.size() == 5) {
                        const string identifier = node->children.at(1).token.text;
                        assembly::NOP();
                        assembly::LabelLatestInstruction(identifier);
                    }
                }

                auto Last() -> void {
                    // FunctionDeclaration -> VoidableDatatype IDENTIFIER ParameterList_1 N_Block FunctionDeclaration
                    if (node->children.size() == 5) {
                        assembly::RET();
                    }
                }
            }

            namespace Argument {
                auto First() -> void {
                    const int r1 = PopRegister();
                    assembly::MOV(r1, registers::MDR1);
                    assembly::PSH();
                }
            }

            // ArgumentList_0
            // ArgumentList_1

            namespace FunctionCall {
                auto Last() -> void {
                    const int IHaveNoIdea = 0;
                    assembly::PPC(IHaveNoIdea);
                }
            }

            namespace ReturnContents {
                auto Last() -> void {

                    const int returnRegister = registers::Allocate();

                    // ReturnContents -> NONE
                    if (node->children.at(0).token.type == NONE) {
                        assembly::SET(0, returnRegister);
                    }

                    // ReturnContents -> Term
                    else {
                        const int r1 = PopRegister();
                        assembly::MOV(r1, returnRegister);
                    }
                    
                    PushRegister(returnRegister);
                }
            }

            namespace N_Block_0 {
                auto First() -> void {}
                auto Last() -> void {}
            }

            namespace L_Block_0 {
                auto First() -> void {}
                auto Last() -> void {}
            }

            namespace Program {
                auto First() -> void {
                    const int r1 = registers::Allocate();
                    const int r2 = registers::Allocate();
                    const int r3 = registers::Allocate();

                    const int IHaveNoIdea = 4;
                    
                    assembly::Comment("branch to main");
                    assembly::PPC(IHaveNoIdea);
                    assembly::SET("1" "main", r1);
                    assembly::SET("2" "main", r2);
                    assembly::SET("3" "main", r3);
                    assembly::BRA(r1, r2, r3);

                    assembly::Comment("branch to end of program");
                    assembly::SET(14, r1);
                    assembly::SET(15, r2);
                    assembly::SET(7, r3);
                    assembly::BRA(r1, r2, r3);

                    registers::Free(r1);
                    registers::Free(r2);
                    registers::Free(r3);
                }
            }
        }

        unordered_map<int, void(*)()> firstGeneratorFunctions = {
            {Reference, generate::Reference::First},
            {Dereference, generate::Dereference::First},
            {ListIndex, generate::ListIndex::First},
            {Variable, generate::Variable::First},
            {IdentifierSuffix, generate::IdentifierSuffix::First},
            {Literal, generate::Literal::First},
            {InputTerm, generate::InputTerm::First},

            {SimpleStatement, generate::SimpleStatement::First},
            {LoopCondition, generate::LoopCondition::First},

            //{If, generate::If::First},
            //{ElseIf, generate::ElseIf::First},
            //{Else, generate::Else::First},
            //{N_If, generate::N_If::First},
            //{N_ElseIf, generate::N_ElseIf::First},
            //{N_Else, generate::N_Else::First},
            //{N_IfBlock, generate::N_IfBlock::First},
            //{L_If, generate::L_If::First},
            //{L_ElseIf, generate::L_ElseIf::First},
            //{L_Else, generate::L_Else::First},
            //{L_IfBlock, generate::L_IfBlock::First},

            //{N_Case_0, generate::N_Case_0::First},
            //{N_Case_1, generate::N_Case_1::First},
            //{N_Case_2, generate::N_Case_2::First},
            //{L_Case_0, generate::L_Case_0::First},
            //{L_Case_1, generate::L_Case_1::First},
            //{L_Case_2, generate::L_Case_2::First},
            //{Switch_0, generate::Switch_0::First},
            //{N_Switch_1, generate::N_Switch_1::First},
            //{L_Switch_1, generate::L_Switch_1::First},

            {Parameter, generate::Parameter::First},
            {FunctionDeclaration, generate::FunctionDeclaration::First},

            {Argument, generate::Argument::First},

            {N_Block, generate::N_Block_0::First},
            {L_Block, generate::L_Block_0::First},
            {Program, generate::Program::First},
        };

        unordered_map<int, void(*)()> lastGeneratorFunctions = {
            {Term_AND, generate::Term_AND::Last},
            {Term_OR, generate::Term_OR::Last},
            {Term_EQUALS, generate::Term_EQUALS::Last},
            {Term_NOT_EQUALS, generate::Term_NOT_EQUALS::Last},
            {Term_GREATER, generate::Term_GREATER::Last},
            {Term_GREATER_OR_EQUAL, generate::Term_GREATER_OR_EQUAL::Last},
            {Term_LESS, generate::Term_LESS::Last},
            {Term_LESS_OR_EQUAL, generate::Term_LESS_OR_EQUAL::Last},
            {Term_SUB, generate::Term_SUB::Last},
            {Term_ADD, generate::Term_ADD::Last},
            {Term_MULTIPLY, generate::Term_MULTIPLY::Last},
            {Term_DIVIDE, generate::Term_DIVIDE::Last},
            {Term_MODULUS, generate::Term_MODULUS::Last},

            {Reference, generate::Reference::Last},
            {Dereference, generate::Dereference::Last},
            {ListIndex, generate::ListIndex::Last},
            {IdentifierSuffix, generate::IdentifierSuffix::Last},

            {Assignment, generate::Assignment::Last},

            {AssignmentOperation, generate::AssignmentOperation::Last},

            {Declaration, generate::Declaration::Last},

            {LoopCondition, generate::LoopCondition::Last},
            {For, generate::For::Last},
            {While, generate::While::Last},

            //{If, generate::If::Last},
            //{ElseIf, generate::ElseIf::Last},
            //{Else, generate::Else::Last},
            //{N_If, generate::N_If::Last},
            //{N_ElseIf, generate::N_ElseIf::Last},
            //{N_Else, generate::N_Else::Last},
            //{N_IfBlock, generate::N_IfBlock::Last},
            //{L_If, generate::L_If::Last},
            //{L_ElseIf, generate::L_ElseIf::Last},
            //{L_Else, generate::L_Else::Last},
            //{L_IfBlock, generate::L_IfBlock::Last},

            //{N_Case_0, generate::N_Case_0::Last},
            //{N_Case_1, generate::N_Case_1::Last},
            //{N_Case_2, generate::N_Case_2::Last},
            //{L_Case_0, generate::L_Case_0::Last},
            //{L_Case_1, generate::L_Case_1::Last},
            //{L_Case_2, generate::L_Case_2::Last},
            //{Switch_0, generate::Switch_0::Last},
            //{N_Switch_1, generate::N_Switch_1::Last},
            //{L_Switch_1, generate::L_Switch_1::Last},

            {FunctionDeclaration, generate::FunctionDeclaration::Last},

            {FunctionCall, generate::FunctionCall::Last},

            {ReturnContents, generate::ReturnContents::Last},
            {N_Block_0, generate::N_Block_0::Last},
            {L_Block_0, generate::L_Block_0::Last}
        };

        auto RecursiveGenerate(parser::TreeNode &_node) -> void;

        auto RecursiveGenerateFunctionDeclaration(parser::TreeNode &node) -> void {
            if (node.children.empty()) {
                return;
            }
            RecursiveGenerate(node.children.at(0));
            RecursiveGenerate(node.children.at(1));
            symbolTable.Next();
            RecursiveGenerate(node.children.at(2));
            RecursiveGenerate(node.children.at(3));
            symbolTable.Next();
            RecursiveGenerate(node.children.at(4));
        }

        auto RecursiveGenerateBlock(parser::TreeNode &node) -> void {
            if (node.children.empty()) {
                return;
            }
            symbolTable.Next();
            RecursiveGenerate(node.children.at(0));
            RecursiveGenerate(node.children.at(1));
            RecursiveGenerate(node.children.at(2));
            symbolTable.Next();
        }

        auto RecursiveGenerateBlockWithoutEnteringScope(parser::TreeNode &node) -> void {
            if (node.children.empty()) {
                return;
            }
            RecursiveGenerate(node.children.at(0));
            RecursiveGenerate(node.children.at(1));
            RecursiveGenerate(node.children.at(2));
        }

        auto RecursiveGenerateFor(parser::TreeNode &node) -> void {
            symbolTable.Next();
            RecursiveGenerate(node.children.at(0));
            RecursiveGenerate(node.children.at(1));
            RecursiveGenerate(node.children.at(2));
            RecursiveGenerate(node.children.at(3));
            RecursiveGenerateBlockWithoutEnteringScope(node.children.at(4));
            symbolTable.Next();
        }

        auto RecursiveGenerateWhile(parser::TreeNode &node) -> void {
            symbolTable.Next();
            RecursiveGenerate(node.children.at(0));
            RecursiveGenerate(node.children.at(1));
            RecursiveGenerate(node.children.at(2));
            RecursiveGenerate(node.children.at(3));
            RecursiveGenerateBlockWithoutEnteringScope(node.children.at(4));
            symbolTable.Next();
        }

        auto RecursiveGenerate(parser::TreeNode &_node) -> void {
            node = &_node;
            if (firstGeneratorFunctions.count(node->token.type) != 0) {
                (firstGeneratorFunctions.at(node->token.type))();
            }

            if ((node->token.type == N_Block) || (node->token.type == L_Block)) {
                RecursiveGenerateBlock(_node);
            } else if (node->token.type == FunctionDeclaration) {
                RecursiveGenerateFunctionDeclaration(_node);
            } else if (node->token.type == For) {
                RecursiveGenerateFor(_node);
            } else if (node->token.type == While) {
                RecursiveGenerateWhile(_node);
            } else {
                for (parser::TreeNode &child : node->children) {
                    RecursiveGenerate(child);
                }
            }

            node = &_node;
            if (lastGeneratorFunctions.count(node->token.type) != 0) {
                (lastGeneratorFunctions.at(node->token.type))();
            }
        }
    }

    auto Reset() -> void {
        node = nullptr;
        while (!registerStack.empty())  { registerStack.pop();  };
        while (!loopStartStack.empty()) { loopStartStack.pop(); };
        while (!loopEndStack.empty())   { loopEndStack.pop();   };
    }

    auto Generate(parser::TreeNode &_node, const SymbolTable &_symbolTable, const bool _debugMode) -> std::pair<vector<string>, unordered_map<int, string>> {
        debugMode = _debugMode;
        symbolTable = _symbolTable;
        symbolTable.Next(); // push global scope
        RecursiveGenerate(_node);
        symbolTable.Next(); // pop global scope

        if (!registerStack.empty()) {
            std::cout << colors::CYAN << registerStack.size() << colors::RED << " registers remaining on register stack" << colors::WHITE << "\n";
        }

        assembly::ResolveLabels();

        return std::make_pair(assembly::GetProgram(), assembly::GetComments());
    }
}