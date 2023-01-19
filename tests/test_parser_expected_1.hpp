#pragma once

#include "grammar/nonTerminals.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"

using parser::TreeNode;


const TreeNode expected_1 = {
    TreeNode{nullptr, Token{Program, ""}, vector<TreeNode>{
        TreeNode{nullptr, Token{FunctionDeclaration, ""}, vector<TreeNode>{
            TreeNode{nullptr, {VoidableDatatype, ""}, vector<TreeNode>{
                TreeNode{nullptr, {Datatype, ""}, vector<TreeNode>{
                    TreeNode{nullptr, {INT64, "int64"}, vector<TreeNode>{}}
                }},
            }},
            TreeNode{nullptr, {IDENTIFIER, "Factorial"}, vector<TreeNode>{}},
            TreeNode{nullptr, {ParameterList_1, ""}, vector<TreeNode>{
                TreeNode{nullptr, {OPEN_PARENTHESIS, "("}},
                TreeNode{nullptr, {ParameterList_0, ""}, vector<TreeNode>{
                    TreeNode{nullptr, {Parameter, ""}, vector<TreeNode>{
                        TreeNode{nullptr, {Datatype, ""}, vector<TreeNode>{
                            TreeNode{nullptr, {INT32, "int32"}, vector<TreeNode>{}}
                        }},
                        TreeNode{nullptr, {IDENTIFIER, "x"}, vector<TreeNode>{}},
                    }},
                    TreeNode{nullptr, {NextParameter, ""}, vector<TreeNode>{
                        TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                    }},
                }},
                TreeNode{nullptr, {CLOSE_PARENTHESIS, ")"}, vector<TreeNode>{}}
            }},
            TreeNode{nullptr, {N_Block, ""}, vector<TreeNode>{
                TreeNode{nullptr, {OPEN_BRACE, "{"}, vector<TreeNode>{}},
                TreeNode{nullptr, {N_Block_0, ""}, vector<TreeNode>{
                    TreeNode{nullptr, {SimpleStatement, ""}, vector<TreeNode>{
                        TreeNode{nullptr, {Declaration}, vector<TreeNode>{
                            TreeNode{nullptr, {Datatype}, vector<TreeNode>{
                                TreeNode{nullptr, {INT16, "int16"}, vector<TreeNode>{}}
                            }},
                            TreeNode{nullptr, {Declaration_0, ""}, vector<TreeNode>{
                                {nullptr, {IDENTIFIER, "y"}, vector<TreeNode>{}},
                                {nullptr, {ASSIGN, "="}, vector<TreeNode>{}},
                                {nullptr, {InputTerm, ""}, vector<TreeNode>{
                                    {nullptr, {INPUT, "input"}, vector<TreeNode>{}}
                                }},
                            }},
                        }},
                        TreeNode{nullptr, {SEMICOLON, ";"}, vector<TreeNode>{}}
                    }},
                    TreeNode{nullptr, {N_Block_0, ""}, vector<TreeNode>{
                        TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                    }}
                }},
                TreeNode{nullptr, {CLOSE_BRACE, "}"}, vector<TreeNode>{}}
            }},
            TreeNode{nullptr, {FunctionDeclaration, ""}, vector<TreeNode>{}},
        }},
    }},
};