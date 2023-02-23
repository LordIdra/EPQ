#include "generator/memory.hpp"
#include "generator/dataValues.hpp"
#include "grammar/nonTerminals.hpp"
#include "grammar/symbolNames.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"
#include "semanticAnalyser/scopes/scope.hpp"
#include "semanticAnalyser/scopes/scopeTraverser.hpp"
#include "util/errors.hpp"
#include <cstdlib>
#include <generator/generator.hpp>

#include <generator/assembly.hpp>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <util/types.hpp>

#include "code/code.cpp"
#include "code/logic.cpp"
#include "code/arithmetic.cpp"
#include "code/comparison.cpp"
#include "code/values.cpp"
#include "code/loops.cpp"
#include "code/ifelse.cpp"
#include "code/functions.cpp"
#include "code/assignment.cpp"
#include "code/blocks.cpp"



namespace generator {

    unordered_map<int, void(*)()> firstGeneratorFunctions = {
        {NonTerminal::Reference, Reference::First},
        {NonTerminal::Dereference, Dereference::First},
        {NonTerminal::ListIndex, ListIndex::First},
        {NonTerminal::Variable, Variable::First},
        {NonTerminal::IdentifierSuffix, IdentifierSuffix::First},
        {NonTerminal::Literal, Literal::First},
        {NonTerminal::InputTerm, InputTerm::First},

        {NonTerminal::SimpleStatement, SimpleStatement::First},
        {NonTerminal::LoopCondition, LoopCondition::First},

        {Terminal::BREAK, Break::First},
        {Terminal::CONTINUE, Continue::First},

        {NonTerminal::If, If::First},
        {NonTerminal::ElseIf, ElseIf::First},

        {NonTerminal::Parameter, Parameter::First},
        {NonTerminal::FunctionDeclaration, FunctionDeclaration::First},

        {NonTerminal::FunctionCall, FunctionCall::First},

        {NonTerminal::Argument, Argument::First},

        {NonTerminal::N_Block, N_Block_0::First},
        {NonTerminal::L_Block, L_Block_0::First},
        {NonTerminal::Program, Program::First},
    };

    unordered_map<int, void(*)()> lastGeneratorFunctions = {
        {NonTerminal::Term_AND, Term_AND::Last},
        {NonTerminal::Term_OR, Term_OR::Last},
        {NonTerminal::Term_EQUALS, Term_EQUALS::Last},
        {NonTerminal::Term_NOT_EQUALS, Term_NOT_EQUALS::Last},
        {NonTerminal::Term_GREATER, Term_GREATER::Last},
        {NonTerminal::Term_GREATER_OR_EQUAL, Term_GREATER_OR_EQUAL::Last},
        {NonTerminal::Term_LESS, Term_LESS::Last},
        {NonTerminal::Term_LESS_OR_EQUAL, Term_LESS_OR_EQUAL::Last},
        {NonTerminal::Term_SUB, Term_SUB::Last},
        {NonTerminal::Term_ADD, Term_ADD::Last},
        {NonTerminal::Term_MULTIPLY, Term_MULTIPLY::Last},
        {NonTerminal::Term_DIVIDE, Term_DIVIDE::Last},
        {NonTerminal::Term_MODULUS, Term_MODULUS::Last},

        {NonTerminal::Reference, Reference::Last},
        {NonTerminal::Dereference, Dereference::Last},
        {NonTerminal::ListIndex, ListIndex::Last},
        {NonTerminal::IdentifierSuffix, IdentifierSuffix::Last},

        {NonTerminal::Assignment, Assignment::Last},

        {NonTerminal::AssignmentOperation, AssignmentOperation::Last},

        {NonTerminal::Declaration, Declaration::Last},

        {NonTerminal::LoopCondition, LoopCondition::Last},
        {NonTerminal::For, For::Last},
        {NonTerminal::While, While::Last},

        {NonTerminal::If, If::Last},
        {NonTerminal::ElseIf, ElseIf::Last},
        {NonTerminal::N_If, N_If::Last},
        {NonTerminal::N_ElseIf, N_ElseIf::Last},
        {NonTerminal::N_IfBlock, N_IfBlock::Last},
        {NonTerminal::L_If, L_If::Last},
        {NonTerminal::L_ElseIf, L_ElseIf::Last},
        {NonTerminal::L_IfBlock, L_IfBlock::Last},

        {NonTerminal::FunctionDeclaration, FunctionDeclaration::Last},

        {NonTerminal::FunctionCall, FunctionCall::Last},

        {NonTerminal::ReturnContents, ReturnContents::Last},
        {NonTerminal::N_Block_0, N_Block_0::Last},
        {NonTerminal::L_Block_0, L_Block_0::Last}
    };

    auto RecursiveGenerate(parser::TreeNode &_node) -> void;

    auto RecursiveGenerateFunctionDeclaration(parser::TreeNode &node) -> void {
        if (node.children.empty()) {
            return;
        }
        RecursiveGenerate(node.children.at(0));
        RecursiveGenerate(node.children.at(1));
        scopeTraverser.Next();
        RecursiveGenerate(node.children.at(2));
        RecursiveGenerate(node.children.at(3));
        assembly::RET();
        scopeTraverser.Next();
        RecursiveGenerate(node.children.at(4));
    }

    auto RecursiveGenerateBlock(parser::TreeNode &node) -> void {
        if (node.children.empty()) {
            return;
        }
        scopeTraverser.Next();
        RecursiveGenerate(node.children.at(0));
        RecursiveGenerate(node.children.at(1));
        RecursiveGenerate(node.children.at(2));
        scopeTraverser.Next();
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
        const string loopAssignment = assembly::GenerateLabel("loopAssignment");
        loopAssignmentStack.push(loopAssignment);
        scopeTraverser.Next();
        RecursiveGenerate(node.children.at(0));
        RecursiveGenerate(node.children.at(1).children.at(0)); // Declaration
        RecursiveGenerate(node.children.at(1).children.at(2)); // LoopCondition
        RecursiveGenerateBlockWithoutEnteringScope(node.children.at(2));
        assembly::NOP();
        assembly::LabelLatestInstruction(loopAssignment);
        RecursiveGenerate(node.children.at(1).children.at(4)); // Assignment
        scopeTraverser.Next();
    }

    auto RecursiveGenerateWhile(parser::TreeNode &node) -> void {
        scopeTraverser.Next();
        RecursiveGenerate(node.children.at(0));
        RecursiveGenerate(node.children.at(1));
        RecursiveGenerateBlockWithoutEnteringScope(node.children.at(2));
        scopeTraverser.Next();
    }

    auto RecursiveGenerate(parser::TreeNode &_node) -> void {
        node = &_node;
        if (firstGeneratorFunctions.count(node->token.type) != 0) {
            (firstGeneratorFunctions.at(node->token.type))();
        }

        if ((node->token.type == N_Block) || (node->token.type == L_Block)) {
            RecursiveGenerateBlock(_node);
        } else if (node->token.type == NonTerminal::FunctionDeclaration) {
            RecursiveGenerateFunctionDeclaration(_node);
        } else if (node->token.type == NonTerminal::For) {
            RecursiveGenerateFor(_node);
        } else if (node->token.type == NonTerminal::While) {
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

    auto Reset() -> void {
        node = nullptr;
        while (!dataStack.empty())  { dataStack.pop();  };
        while (!loopStartStack.empty()) { loopStartStack.pop(); };
        while (!loopEndStack.empty())   { loopEndStack.pop();   };
    }

    auto Generate(parser::TreeNode &_node, Scope &_scopeTree, const bool _debugMode) -> void {
        debugMode = _debugMode;
        scopeTraverser = ScopeTraverser(_scopeTree);
        scopeTraverser.Next(); // push global scope
        RecursiveGenerate(_node);
        scopeTraverser.Next(); // pop global scope

        if (!dataStack.empty()) {
            cout << colors::CYAN << dataStack.size() << colors::RED << " registers remaining on register stack" << colors::WHITE << "\n";
        }

        assembly::ResolveLabels();
    }

    auto GetProgram() -> const vector<string>& {
        return assembly::GetProgram();
    }

    auto GetComments() -> const unordered_map<int, string>& {
        return assembly::GetComments();
    }
}