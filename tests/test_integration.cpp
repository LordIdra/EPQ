#include "assembler/assembler.hpp"
#include "generator/assembly.hpp"
#include "generator/dataValues.hpp"
#include "semanticAnalyser/scopes/scopeTraverser.hpp"
#include "semanticAnalyser/semanticAnalyser.hpp"
#include "simulator/simulator.hpp"
#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <unordered_map>
#include <util/errors.hpp>
#include <readfile/readfile.hpp>
#include <scanner/scanner.hpp>
#include <parser/sets/first.hpp>
#include <parser/sets/follow.hpp>
#include <parser/table.hpp>
#include <parser/parser.hpp>
#include <semanticAnalyser/semanticAnalyser.hpp>
#include <generator/generator.hpp>
#include "compile.hpp"



const string PATH = "../../tests/resources/integration/";



void RunTest(const string &name) {
    compile::Compile(PATH + "arithmetic/add");

    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();

    simulator::Run(program, comments, false);
}

TEST_CASE("[7|INT] Arithmetic - Add") {
    RunTest("ariithmetic/add");
    
    auto &traverser = compile::GetTraverser();

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == 5);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == 1);
    //REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == 146);
    //REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == 2323);
}

TEST_CASE("[7|INT] Arithmetic - Sub") {
    compile::Compile(PATH + "arithmetic/sub");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == 8);
    //REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == 24);
    //REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == 812);
}

TEST_CASE("[7|INT] Arithmetic - Multiply") {
    compile::Compile(PATH + "arithmetic/multiply");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == 4);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == 10);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == 1);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == 0);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == 10);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == 13);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_7").address).first == 3);

    //REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_8").address).first == 100);
    //REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_9").address).first == 1050);
}

TEST_CASE("[7|INT] Arithmetic - Divide") {
    compile::Compile(PATH + "arithmetic/divide");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == 2);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == 4);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == 2);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == 14);

    //REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == 25);
    //REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == 206); //-50
}

TEST_CASE("[7|INT] Arithmetic - Modulus") {
    compile::Compile(PATH + "arithmetic/modulus");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == 0);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == 0);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == 2);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == 15);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == 15);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == 1);

    //REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_7").address).first == 2);
}

TEST_CASE("[7|INT] Logic - And") {
    compile::Compile(PATH + "logic/and");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 5);
}

TEST_CASE("[7|INT] Logic - Or") {
    compile::Compile(PATH + "logic/or");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 13);
}

TEST_CASE("[7|INT] Assign - Add") {
    compile::Compile(PATH + "assign/add");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 3);
}

TEST_CASE("[7|INT] Assign - Sub") {
    compile::Compile(PATH + "assign/sub");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 15);
}

TEST_CASE("[7|INT] Assign - Multiply") {
    compile::Compile(PATH + "assign/multiply");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 12);
}

TEST_CASE("[7|INT] Assign - Divide") {
    compile::Compile(PATH + "assign/divide");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 2);
}

TEST_CASE("[7|INT] Assign - Modulus") {
    compile::Compile(PATH + "assign/modulus");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 1);
}

TEST_CASE("[7|INT] Comparison - Equal") {
    compile::Compile(PATH + "comparison/equal");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == dataValues::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == dataValues::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == dataValues::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == dataValues::FALSE);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == dataValues::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == dataValues::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_7").address).first == dataValues::TRUE);
}

TEST_CASE("[7|INT] Comparison - Not Equal") {
    compile::Compile(PATH + "comparison/not_equal");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == dataValues::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == dataValues::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == dataValues::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == dataValues::TRUE);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == dataValues::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == dataValues::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_7").address).first == dataValues::FALSE);
}

TEST_CASE("[7|INT] Comparison - Greater") {
    compile::Compile(PATH + "comparison/greater");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == dataValues::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == dataValues::FALSE);
    
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == dataValues::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == dataValues::TRUE);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == dataValues::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == dataValues::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_7").address).first == dataValues::FALSE);
}

TEST_CASE("[7|INT] Comparison - Greater/Equal") {
    compile::Compile(PATH + "comparison/greater_equal");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == dataValues::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == dataValues::FALSE);
    
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == dataValues::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == dataValues::TRUE);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == dataValues::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == dataValues::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_7").address).first == dataValues::TRUE);
}

TEST_CASE("[7|INT] Comparison - Less") {
    compile::Compile(PATH + "comparison/less");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == dataValues::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == dataValues::TRUE);
    
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == dataValues::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == dataValues::FALSE);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == dataValues::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == dataValues::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_7").address).first == dataValues::FALSE);
}

TEST_CASE("[7|INT] Comparison - Less/Equal") {
    compile::Compile(PATH + "comparison/less_equal");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == dataValues::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == dataValues::TRUE);
    
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == dataValues::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == dataValues::FALSE);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == dataValues::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == dataValues::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_7").address).first == dataValues::TRUE);
}

TEST_CASE("[7|INT] Branching - If 1") {
    compile::Compile(PATH + "branching/if_1");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 3);
}

TEST_CASE("[7|INT] Branching - If 2") {
    compile::Compile(PATH + "branching/if_2");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 1);
}

TEST_CASE("[7|INT] Branching - Else 1") {
    compile::Compile(PATH + "branching/else_1");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 3);
}

TEST_CASE("[7|INT] Branching - Else 2") {
    compile::Compile(PATH + "branching/else_2");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 14);
}

TEST_CASE("[7|INT] Branching - Elseif 1") {
    compile::Compile(PATH + "branching/elseif_1");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 4);
}

TEST_CASE("[7|INT] Branching - Elseif 2") {
    compile::Compile(PATH + "branching/elseif_2");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 14);
}

TEST_CASE("[7|INT] Branching - Elseif 3") {
    compile::Compile(PATH + "branching/elseif_3");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 11);
}

TEST_CASE("[7|INT] Branching - Elseif 4") {
    compile::Compile(PATH + "branching/elseif_4");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 7);
}

TEST_CASE("[7|INT] Loops - While") {
    compile::Compile(PATH + "loops/while");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == 3);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == 6);
}

TEST_CASE("[7|INT] Loops - For") {
    compile::Compile(PATH + "loops/for");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == 6);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == 7);
}

TEST_CASE("[7|INT] Loops - Nested For") {
    compile::Compile(PATH + "loops/nested_for");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 6);
}

TEST_CASE("[7|INT] Loops - For-If") {
    compile::Compile(PATH + "loops/for_if");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 12);
}

TEST_CASE("[7|INT] Loops - Continue 1") {
    compile::Compile(PATH + "loops/continue_1");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 2);
}

TEST_CASE("[7|INT] Loops - Continue 2") {
    compile::Compile(PATH + "loops/continue_2");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 3);
}

TEST_CASE("[7|INT] Loops - Break 1") {
    compile::Compile(PATH + "loops/break_1");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 2);
}

TEST_CASE("[7|INT] Loops - Break 2") {
    compile::Compile(PATH + "loops/break_2");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 3);
}

TEST_CASE("[7|INT] Functions - Call") {
    compile::Compile(PATH + "functions/call");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 2);
}

TEST_CASE("[7|INT] Functions - Parameters 1") {
    compile::Compile(PATH + "functions/parameters_1");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 2);
}

TEST_CASE("[7|INT] Functions - Parameters 2") {
    compile::Compile(PATH + "functions/parameters_2");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 5);
}

TEST_CASE("[7|INT] Functions - Multiple return values") {
    compile::Compile(PATH + "functions/return_multiple");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == 1);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == 6);
}

TEST_CASE("[7|INT] Functions - Return") {
    compile::Compile(PATH + "functions/return");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 5);
}