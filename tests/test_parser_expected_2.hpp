#pragma once

#include "grammar/nonTerminals.hpp"
#include "grammar/terminals.hpp"
#include "parser/parser.hpp"

using parser::TreeNode;


const TreeNode expected_2 = {
    TreeNode{nullptr, Token{Program, ""}, vector<TreeNode>{
        TreeNode{nullptr, Token{FunctionDeclaration, ""}, vector<TreeNode>{
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
                        TreeNode{nullptr, {Declaration}, vector<TreeNode>{
                            TreeNode{nullptr, {Datatype}, vector<TreeNode>{
                                TreeNode{nullptr, {INT4, "int4"}, vector<TreeNode>{}}
                            }},
                            TreeNode{nullptr, {Declaration_0}, vector<TreeNode>{
                                TreeNode{nullptr, {IDENTIFIER, "x"}, vector<TreeNode>{}},
                                TreeNode{nullptr, {ASSIGN, "="}, vector<TreeNode>{}},
                                TreeNode{nullptr, {InputTerm, ""}, vector<TreeNode>{
                                    TreeNode{nullptr, {Term, ""}, vector<TreeNode>{
                                        TreeNode{nullptr, {Term_0, ""}, vector<TreeNode>{
                                            TreeNode{nullptr, {Term_1, ""}, vector<TreeNode>{
                                                TreeNode{nullptr, {Term_2, ""}, vector<TreeNode>{
                                                    TreeNode{nullptr, {Term_3, ""}, vector<TreeNode>{
                                                        TreeNode{nullptr, {Term_4, ""}, vector<TreeNode>{
                                                            TreeNode{nullptr, {Term_5, ""}, vector<TreeNode>{
                                                                TreeNode{nullptr, {Term_6, ""}, vector<TreeNode>{
                                                                    TreeNode{nullptr, {Term_7, ""}, vector<TreeNode>{
                                                                        TreeNode{nullptr, {Term_8, ""}, vector<TreeNode>{
                                                                            TreeNode{nullptr, {Term_9, ""}, vector<TreeNode>{
                                                                                TreeNode{nullptr, {Term_10, ""}, vector<TreeNode>{
                                                                                    TreeNode{nullptr, {Term_11, ""}, vector<TreeNode>{
                                                                                        TreeNode{nullptr, {Term_12, ""}, vector<TreeNode>{
                                                                                            TreeNode{nullptr, {Term_13, ""}, vector<TreeNode>{
                                                                                                TreeNode{nullptr, {Term_14, ""}, vector<TreeNode>{
                                                                                                    TreeNode{nullptr, {Value, ""}, vector<TreeNode>{
                                                                                                        TreeNode{nullptr, {Literal, ""}, vector<TreeNode>{
                                                                                                            TreeNode{nullptr, {TRUE, "true"}, vector<TreeNode>{}},
                                                                                                        }},
                                                                                                    }},
                                                                                                }},
                                                                                            }},
                                                                                            TreeNode{nullptr, {Term_MODULUS, ""}, vector<TreeNode>{
                                                                                                TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                                                                                            }},
                                                                                        }},
                                                                                        TreeNode{nullptr, {Term_DIVIDE, ""}, vector<TreeNode>{
                                                                                            TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                                                                                        }},
                                                                                    }},
                                                                                    TreeNode{nullptr, {Term_MULTIPLY, ""}, vector<TreeNode>{
                                                                                        TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                                                                                    }},
                                                                                }},
                                                                                TreeNode{nullptr, {Term_ADD, ""}, vector<TreeNode>{
                                                                                    TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                                                                                }},
                                                                            }},
                                                                            TreeNode{nullptr, {Term_SUB, ""}, vector<TreeNode>{
                                                                                TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                                                                            }},
                                                                        }},
                                                                        TreeNode{nullptr, {Term_LESS_OR_EQUAL, ""}, vector<TreeNode>{
                                                                            TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                                                                        }},
                                                                    }},
                                                                    TreeNode{nullptr, {Term_LESS, ""}, vector<TreeNode>{
                                                                        TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                                                                    }},
                                                                }},
                                                                TreeNode{nullptr, {Term_GREATER_OR_EQUAL, ""}, vector<TreeNode>{
                                                                    TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                                                                }},
                                                            }},
                                                            TreeNode{nullptr, {Term_GREATER, ""}, vector<TreeNode>{
                                                                TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                                                            }},
                                                        }},
                                                        TreeNode{nullptr, {Term_NOT_EQUALS, ""}, vector<TreeNode>{
                                                            TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                                                        }},
                                                    }},
                                                    TreeNode{nullptr, {Term_EQUALS, ""}, vector<TreeNode>{
                                                        TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                                                    }},
                                                }},
                                                TreeNode{nullptr, {Term_OR, ""}, vector<TreeNode>{
                                                    TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                                                }},
                                            }},
                                            TreeNode{nullptr, {Term_AND, ""}, vector<TreeNode>{
                                                TreeNode{nullptr, {NONE, ""}, vector<TreeNode>{}}
                                            }},
                                        }},
                                    }},
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