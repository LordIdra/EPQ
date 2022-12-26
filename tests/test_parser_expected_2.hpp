#pragma once

#include "grammar/nonTerminals.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"

using parser::TreeNode;


const TreeNode expected_2 = {
    TreeNode{nullptr, Token{Program, ""}, vector<TreeNode>{
        TreeNode{nullptr, Token{FunctionSequence, ""}, vector<TreeNode>{
            TreeNode{nullptr, {VoidableDatatype, ""}, vector<TreeNode>{
                TreeNode{nullptr, {VOID, "void"}, vector<TreeNode>{}}
            }},
            TreeNode{nullptr, {IDENTIFIER, "DoThing"}, vector<TreeNode>{}},
            TreeNode{nullptr, {ParameterList_1, ""}, vector<TreeNode>{
                TreeNode{nullptr, {OPEN_PARENTHESIS, "("}},
                TreeNode{nullptr, {ParameterList_0, ""}, vector<TreeNode>{
                    TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}},
                }},
                TreeNode{nullptr, {CLOSE_PARENTHESIS, ")"}, vector<TreeNode>{}}
            }},
            TreeNode{nullptr, {N_Block, ""}, vector<TreeNode>{
                TreeNode{nullptr, {OPEN_BRACE, "{"}, vector<TreeNode>{}},
                TreeNode{nullptr, {N_Block_0, ""}, vector<TreeNode>{
                    TreeNode{nullptr, {SimpleStatement, ""}, vector<TreeNode>{
                        TreeNode{nullptr, {ConstDeclaration}, vector<TreeNode>{
                            TreeNode{nullptr, {CONST, "const"}, vector<TreeNode>{}},
                            TreeNode{nullptr, {Datatype}, vector<TreeNode>{
                                TreeNode{nullptr, {INT4, "int4"}, vector<TreeNode>{}}
                            }},
                            TreeNode{nullptr, {IDENTIFIER, "x"}, vector<TreeNode>{}},
                            TreeNode{nullptr, {ASSIGN, "="}, vector<TreeNode>{}},
                            TreeNode{nullptr, {Literal, ""}, vector<TreeNode>{
                                TreeNode{nullptr, {TRUE, "true"}, vector<TreeNode>{}}
                            }}
                        }},
                        TreeNode{nullptr, {SEMICOLON, ";"}, vector<TreeNode>{}}
                    }},
                    TreeNode{nullptr, {N_Block_0, ""}, vector<TreeNode>{
                        TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                    }}
                }},
                TreeNode{nullptr, {CLOSE_BRACE, "}"}, vector<TreeNode>{}}
            }},
            TreeNode{nullptr, {FunctionSequence, ""}, vector<TreeNode>{
                
            }},
        }},
    }},
};