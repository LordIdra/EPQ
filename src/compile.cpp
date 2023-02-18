#include "generator/assembly.hpp"
#include "generator/generator.hpp"
#include "generator/memory.hpp"
#include "generator/registers.hpp"
#include "parser/parser.hpp"
#include "parser/sets/first.hpp"
#include "parser/table.hpp"
#include "readfile/readfile.hpp"
#include "scanner/scanner.hpp"
#include "scanner/stateGenerator.hpp"
#include "semanticAnalyser/scopes/scopeTraverser.hpp"
#include "semanticAnalyser/semanticAnalyser.hpp"
#include "simulator/simulator.hpp"
#include "util/errors.hpp"
#include <iostream>

#include <parser/sets/first.hpp>
#include <parser/sets/follow.hpp>
#include <unordered_map>
#include <utility>



namespace compile {
    namespace {
        ScopeTraverser traverser;

        auto Reset() -> void {
            errors::Reset();
            scanner::Reset();
            parser::Reset();
            semanticAnalyser::Reset();
            assembly::Reset();
            memory::Reset();
            registers::Reset();
            generator::Reset();
            simulator::Reset();
        }

        auto GenerateSets() -> void {
            first::ComputeFirstSet();
            follow::ComputeFollowSet();
            table::GenerateTable();
        }

        auto CheckErrors() -> void {
            if (errors::GetErrorCode() != errors::NONE) {
                errors::OutputErrors();
                throw;
            }
        }
    }

    auto Compile(const string &file) -> void {
        Reset();

        const vector<string> &input = readfile::Read(file + ".txt");
        CheckErrors();

        const vector<Token> &scannedInput = scanner::Scan(input);
        CheckErrors();

        GenerateSets();

        parser::TreeNode abstractSyntaxTree = parser::Parse(scannedInput);
        CheckErrors();

        Scope& scope = semanticAnalyser::Analyse(abstractSyntaxTree);
        CheckErrors();

        traverser = ScopeTraverser(scope);
        generator::Generate(abstractSyntaxTree, scope, false);
        CheckErrors();
    }

    auto GetTraverser() -> ScopeTraverser& {
        return traverser;
    }

    auto GetProgram() -> const vector<string>& {
        return generator::GetProgram();
    }

    auto GetComments() -> const unordered_map<int, string>& {
        return generator::GetComments();
    }
}