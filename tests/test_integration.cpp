#include "generator/assembly.hpp"
#include "generator/registers.hpp"
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



TEST_CASE("[7|INT] Integration 1") {
    compile::Compile(PATH + "1");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 5);
}

TEST_CASE("[7|INT] Integration 2") {
    compile::Compile(PATH + "2");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 8);
}

TEST_CASE("[7|INT] Integration 3") {
    compile::Compile(PATH + "3");
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
}

TEST_CASE("[7|INT] Integration 4") {
    compile::Compile(PATH + "4");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_AND").address).first == 5);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_OR").address).first == 13);
}

TEST_CASE("[7|INT] Integration 5") {
    compile::Compile(PATH + "5");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == registers::FALSE);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_7").address).first == registers::TRUE);
}

TEST_CASE("[7|INT] Integration 6") {
    compile::Compile(PATH + "6");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == registers::TRUE);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_7").address).first == registers::FALSE);
}

TEST_CASE("[7|INT] Integration 7") {
    compile::Compile(PATH + "7");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == registers::FALSE);
    
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == registers::TRUE);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_7").address).first == registers::FALSE);
}

TEST_CASE("[7|INT] Integration 8") {
    compile::Compile(PATH + "8");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == registers::FALSE);
    
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == registers::TRUE);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_7").address).first == registers::TRUE);
}

TEST_CASE("[7|INT] Integration 9") {
    compile::Compile(PATH + "9");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == registers::TRUE);
    
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == registers::FALSE);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_7").address).first == registers::FALSE);
}

TEST_CASE("[7|INT] Integration 10") {
    compile::Compile(PATH + "10");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == registers::TRUE);
    
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == registers::FALSE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == registers::FALSE);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_5").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_6").address).first == registers::TRUE);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_7").address).first == registers::TRUE);
}

TEST_CASE("[7|INT] Integration 11") {
    compile::Compile(PATH + "11");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 3);
}

TEST_CASE("[7|INT] Integration 12") {
    compile::Compile(PATH + "12");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 15);
}

TEST_CASE("[7|INT] Integration 13") {
    compile::Compile(PATH + "13");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 12);
}

TEST_CASE("[7|INT] Integration 14") {
    compile::Compile(PATH + "14");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == 2);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == 4);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_3").address).first == 2);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_4").address).first == 14);
}

TEST_CASE("[7|INT] Integration 15") {
    compile::Compile(PATH + "15");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 2);
}

TEST_CASE("[7|INT] Integration 16") {
    compile::Compile(PATH + "16");
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
}

TEST_CASE("[7|INT] Integration 17") {
    compile::Compile(PATH + "17");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 1);
}

TEST_CASE("[7|INT] Integration 18") {
    compile::Compile(PATH + "18");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == 3);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == 6);
}

TEST_CASE("[7|INT] Integration 19") {
    compile::Compile(PATH + "19");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == 6);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == 7);
}

TEST_CASE("[7|INT] Integration 20") {
    compile::Compile(PATH + "20");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 2);
}

TEST_CASE("[7|INT] Integration 21") {
    compile::Compile(PATH + "21");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_1").address).first == 1);
    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT_2").address).first == 6);
}

TEST_CASE("[7|INT] Integration 22") {
    compile::Compile(PATH + "22");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 5);
}

TEST_CASE("[7|INT] Integration 23") {
    compile::Compile(PATH + "23");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 2);
}

TEST_CASE("[7|INT] Integration 24") {
    compile::Compile(PATH + "24");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, true);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 2);
}

TEST_CASE("[7|INT] Integration 25") {
    compile::Compile(PATH + "25");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 3);
}

TEST_CASE("[7|INT] Integration 26") {
    compile::Compile(PATH + "26");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 14);
}

TEST_CASE("[7|INT] Integration 27") {
    compile::Compile(PATH + "27");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 3);
}

TEST_CASE("[7|INT] Integration 28") {
    compile::Compile(PATH + "28");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 1);
}

TEST_CASE("[7|INT] Integration 29") {
    compile::Compile(PATH + "29");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 6);
}

TEST_CASE("[7|INT] Integration 30") {
    compile::Compile(PATH + "30");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 12);
}

TEST_CASE("[7|INT] Integration 31") {
    compile::Compile(PATH + "31");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 4);
}

TEST_CASE("[7|INT] Integration 32") {
    compile::Compile(PATH + "32");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 14);
}

TEST_CASE("[7|INT] Integration 33") {
    compile::Compile(PATH + "33");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 11);
}

TEST_CASE("[7|INT] Integration 34") {
    compile::Compile(PATH + "34");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 7);
}

TEST_CASE("[7|INT] Integration 35") {
    compile::Compile(PATH + "35");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 3);
}

TEST_CASE("[7|INT] Integration 36") {
    compile::Compile(PATH + "36");
    const auto &program = compile::GetProgram();
    const auto &comments = compile::GetComments();
    auto &traverser = compile::GetTraverser();

    simulator::Run(program, comments, false);

    REQUIRE(simulator::GetData(traverser.GlobalLookup("TEST_OUTPUT").address).first == 3);
}