#pragma once

#include <scanner/tokens.hpp>

typedef vector<TokenType> TokenTypeSet;


auto AddTokenSet(const TokenTypeSet &from, TokenTypeSet to) -> TokenTypeSet {
    for (TokenType tokenType : from) {
        to.push_back(tokenType);
    }
    return to;
}

namespace first {
    // PROGRAM
    const TokenTypeSet Program_a {};
    const TokenTypeSet Program = AddTokenSet(Program_a, Datatype);

    // FUNCTIONS
    const TokenTypeSet FunctionSequence_a {};
    const TokenTypeSet FunctionSequence = AddTokenSet(FunctionSequence_a, Datatype);

    // PARAMETERS
    const TokenTypeSet ParameterList_0 {OPEN_PARENTHESIS};

    const TokenTypeSet ParameterList_1_a {};
    const TokenTypeSet ParameterList_1 = AddTokenSet(ParameterList_1_a, ParameterList_0);
    
    const TokenTypeSet ParameterList_2_a {};
    const TokenTypeSet ParameterList_2 = AddTokenSet(ParameterList_2_a, ParameterList_0);

    const TokenTypeSet ParameterList_3_a {};
    const TokenTypeSet ParameterList_3 = AddTokenSet(ParameterList_3_a, ParameterList_2);

    const TokenTypeSet NextParameter {COMMA, NONE};

    const TokenTypeSet Parameter_a {CONST};
    const TokenTypeSet Parameter = AddTokenSet(Parameter_a, Datatype);

    // BLOCKS
    const TokenTypeSet L_Block {OPEN_BRACE};

    const TokenTypeSet L_Block_0 {CONTINUE, BREAK, RETURN, SimpleStatement, For, While, L_Switch_1, L_If_3};

    const TokenTypeSet N_Block {OPEN_BRACE};

    const TokenTypeSet N_Block_0 {RETURN, SimpleStatement, For, While, N_Switch_1, N_If_3};

    // LOOPS
    const TokenTypeSet While {WHILE};

    const TokenTypeSet For {FOR};

    // CASE
    const TokenTypeSet N_Case_0 {DEFAULT, NONE};

    const TokenTypeSet N_Case_1 {CASE, NONE};

    const TokenTypeSet N_Case_2_a {};
    const TokenTypeSet N_Case_2 = AddTokenSet(N_Case_2_a, N_Case_1);

    const TokenTypeSet L_Case_0 {DEFAULT, NONE};

    const TokenTypeSet L_Case_1 {CASE, NONE};

    const TokenTypeSet L_Case_2_a {};
    const TokenTypeSet L_Case_2 = AddTokenSet(L_Case_2_a, L_Case_1);

    // SWITCH
    const TokenTypeSet Switch_0 {SWITCH};

    const TokenTypeSet N_Switch_1 {SWITCH};

    const TokenTypeSet L_Switch_1 {SWITCH};

    // SIMPLE STATEMENT
    const TokenTypeSet SimpleStatement_a {OUTPUT};
    const TokenTypeSet SimpleStatement_b = AddTokenSet(SimpleStatement_a, Term);
    const TokenTypeSet SimpleStatement_c = AddTokenSet(SimpleStatement_b, Boolean);
    const TokenTypeSet SimpleStatement_d = AddTokenSet(SimpleStatement_c, Assignment);
    const TokenTypeSet SimpleStatement_e = AddTokenSet(SimpleStatement_d, ConstDeclaration);
    const TokenTypeSet SimpleStatement_f = AddTokenSet(SimpleStatement_e, NonConstDeclaration);

    // IF ELSE (generic)
    const TokenTypeSet If_0 {IF};

    // IF ELSE (N)
    const TokenTypeSet N_If_ElseIf {ELSE, NONE};

    const TokenTypeSet N_If_Else {ELSE, NONE};

    const TokenTypeSet N_If_1_a {};
    const TokenTypeSet N_If_1 = AddTokenSet(N_If_1_a, If_0);
    
    const TokenTypeSet N_If_2_a {};
    const TokenTypeSet N_If_2 = AddTokenSet(N_If_2_a, N_If_1);

    const TokenTypeSet N_If_3_a {};
    const TokenTypeSet N_If_3 = AddTokenSet(N_If_3_a, N_If_2);
    
    // IF ELSE (L)
    const TokenTypeSet L_If_ElseIf {ELSE, NONE};

    const TokenTypeSet L_If_Else {ELSE, NONE};

    const TokenTypeSet L_If_1_a {};
    const TokenTypeSet L_If_1 = AddTokenSet(L_If_1_a, If_0);

    const TokenTypeSet L_If_2_a {};
    const TokenTypeSet L_If_2 = AddTokenSet(L_If_2_a, L_If_1);

    const TokenTypeSet L_If_3_a {};
    const TokenTypeSet L_If_3 = AddTokenSet(L_If_3_a, L_If_2);

    // VARIABLES
    const TokenTypeSet Datatype {INT4, INT8, INT16, INT32, INT64, BOOL};

    const TokenTypeSet Reference {REFERENCE};

    const TokenTypeSet Dereference {DEREFERENCE};
}