#pragma once

#include <grammar/terminals.hpp>



enum NonTerminal {
    // Start just after the final token in the enum
    // This allows us to refer to both terminals and non terminals with a single integer
    Term_AND = LAST_TERMINAL + 1,
    Term_OR,
    Term_EQUALS,
    Term_NOT_EQUALS,
    Term_GREATER,
    Term_GREATER_OR_EQUAL,
    Term_LESS,
    Term_LESS_OR_EQUAL,
    Term_SUB,
    Term_ADD,
    Term_MULTIPLY,
    Term_DIVIDE,
    Term_MODULUS,

    Term,
    Term_0,
    Term_1,
    Term_2,
    Term_3,
    Term_4,
    Term_5,
    Term_6,
    Term_7,
    Term_8,
    Term_9,
    Term_10,
    Term_11,
    Term_12,
    Term_13,
    Term_14,

    Datatype,
    VoidableDatatype,

    Reference,
    Dereference,
    ListIndex,

    Value,
    Variable,
    IdentifierSuffix,

    Literal,

    InputTerm,

    AssignmentOperation,
    Assignment,

    Declaration_0,
    Declaration,

    SimpleStatement,
    LoopCondition,
    ForSequence,
    For,
    While,
    
    If,
    ElseIf,
    Else,

    N_If,
    N_ElseIf,
    N_Else,
    N_IfBlock,

    L_If,
    L_ElseIf,
    L_Else,
    L_IfBlock,

    N_Case_0,
    N_Case_1,
    N_Case_2,

    L_Case_0,
    L_Case_1,
    L_Case_2,

    Switch_0,
    N_Switch_1,
    L_Switch_1,

    Parameter,
    NextParameter,
    ParameterList_0,
    ParameterList_1,

    FunctionDeclaration,

    Argument,
    ArgumentList_0,
    ArgumentList_1,
    FunctionCall,

    ReturnContents,

    N_Block_0,
    N_Block,

    L_Block_0,
    L_Block,

    Program
};

const int FIRST_NON_TERMINAL = Term_AND;
const int LAST_NON_TERMINAL = Program;