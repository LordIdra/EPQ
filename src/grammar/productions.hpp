#pragma once

#include "grammar/terminals.hpp"
#include <grammar/nonTerminals.hpp>



typedef int ProductionLeft;
typedef vector<int> ProductionRight;
typedef vector<ProductionRight> ProductionRightList;
typedef std::pair<ProductionLeft, ProductionRight> ProductionPair;



const unordered_map<ProductionLeft, vector<ProductionRight>> productions {

    /* Operator Terms */
    {Term_AND, {
        {NONE},
        {AND, Term_1, Term_AND}}},
    
    {Term_OR, {
        {NONE},
        {OR, Term_2, Term_OR}}},
    
    {Term_EQUALS, {
        {NONE},
        {EQUALS, Term_3, Term_EQUALS}}},

    {Term_NOT_EQUALS, {
        {NONE},
        {NOT_EQUALS, Term_4, Term_NOT_EQUALS}}},
    
    {Term_GREATER, {
        {NONE},
        {GREATER, Term_5, Term_GREATER}}},
    
    {Term_GREATER_OR_EQUAL, {
        {NONE},
        {GREATER_OR_EQUAL, Term_6, Term_GREATER_OR_EQUAL}}},
    
    {Term_LESS, {
        {NONE},
        {LESS, Term_7, Term_LESS}}},
    
    {Term_LESS_OR_EQUAL, {
        {NONE},
        {LESS_OR_EQUAL, Term_8, Term_LESS_OR_EQUAL}}},

    {Term_ADD, {
        {NONE},
        {ADD, Term_9, Term_ADD}}},

    {Term_SUB, {
        {NONE},
        {SUBTRACT, Term_10, Term_SUB}}},

    {Term_MULTIPLY, {
        {NONE},
        {MULTIPLY, Term_11, Term_MULTIPLY}}},

    {Term_DIVIDE, {
        {NONE},
        {DIVIDE, Term_12, Term_DIVIDE}}},

    {Term_MODULUS, {
        {NONE},
        {MODULUS, Term_13, Term_MODULUS}}},


    /* Term Definitions */
    {Term, {
        {Term_0}}},

    {Term_0, {
        {Term_1, Term_AND}}},

    {Term_1, {
        {Term_2, Term_OR}}},

    {Term_2, {
        {Term_3, Term_EQUALS}}},
    
    {Term_3, {
        {Term_4, Term_NOT_EQUALS}}},

    {Term_4, {
        {Term_5, Term_GREATER}}},

    {Term_5, {
        {Term_6, Term_GREATER_OR_EQUAL}}},
    
    {Term_6, {
        {Term_7, Term_LESS}}},
    
    {Term_7, {
        {Term_8, Term_LESS_OR_EQUAL}}},
    
    {Term_8, {
        {Term_9, Term_ADD}}},
    
    {Term_9, {
        {Term_10, Term_SUB}}},
    
    {Term_10, {
        {Term_11, Term_MULTIPLY}}},
    
    {Term_11, {
        {Term_12, Term_DIVIDE}}},
    
    {Term_12, {
        {Term_13, Term_MODULUS}}},
    
    {Term_13, {
        {Term_14},
        {NOT, Term_14}}},
    
    {Term_14, {
        {OPEN_PARENTHESIS, Term, CLOSE_PARENTHESIS},
        {Value}}},


    /* Datatypes */
    {Datatype, {
        {INT1},
        {INT2},
        {INT4},
        {INT8},
        {INT16},
        {INT32},
        {INT64}}},

    {VoidableDatatype, {
        {Datatype},
        {VOID}}},
        
    
    /* Variables*/
    {Reference, {
        {REFERENCE, IDENTIFIER}}},

    {Dereference, {
        {DEREFERENCE, IDENTIFIER}}},

    {ListIndex, {
        {OPEN_SQUARE_BRACKET, Term, CLOSE_SQUARE_BRACKET}}},
    
    {Value, {
        {Variable},
        {Literal},
        {FunctionCall}}},
    
    {Variable, {
        {IDENTIFIER, IdentifierSuffix},
        {Dereference}}},

    {IdentifierSuffix, {
        {NONE},
        {ListIndex}}},
    
    {Literal, {
        {NUMBER},
        {TRUE},
        {FALSE}}},
    
    {InputTerm, {
        {Term},
        {INPUT}}},


    /* Assignment*/
    {AssignmentOperation, {
        {ASSIGN, InputTerm},
        {ADD_ASSIGN, Term},
        {SUBTRACT_ASSIGN, Term},
        {MULTIPLY_ASSIGN, Term},
        {DIVIDE_ASSIGN, Term},
        {MODULUS_ASSIGN, Term}}},

    {Assignment, {
        {Variable, AssignmentOperation}}},


    /* Declaration */
    {Declaration_0, {
        {OPEN_SQUARE_BRACKET, NUMBER, CLOSE_SQUARE_BRACKET, IDENTIFIER},
        {POINTER, IDENTIFIER, ASSIGN, Reference},
        {IDENTIFIER, ASSIGN, InputTerm}}},

    {Declaration, {
        {Datatype, Declaration_0}}},
    

    /* Statements */
    {SimpleStatement, {
        {Assignment, SEMICOLON},
        {FunctionCall, SEMICOLON},
        {Declaration, SEMICOLON},
        {OUTPUT, Term, SEMICOLON}}},

    {LoopCondition, {
        {Term}}},
    
    {ForSequence, {
        {Declaration, SEMICOLON, LoopCondition, SEMICOLON, Assignment}}},
    

    {For, {
        {FOR, OPEN_PARENTHESIS, ForSequence, CLOSE_PARENTHESIS, L_Block}}},

    {While, {
        {WHILE, OPEN_PARENTHESIS, LoopCondition, CLOSE_PARENTHESIS, L_Block}}},

    /* If-else*/
    {If, {
        {IF, OPEN_PARENTHESIS, Term, CLOSE_PARENTHESIS}}},

    {ElseIf, {
        {ELSEIF, OPEN_PARENTHESIS, Term, CLOSE_PARENTHESIS}}},

    {Else, {
        {ELSE}}},

    {N_If, {
        {If, N_Block}}},

    {N_ElseIf, {
        {NONE},
        {ElseIf, N_Block, N_ElseIf}}},

    {N_Else, {
        {NONE},
        {Else, N_Block}}},

    {N_IfBlock, {
        {N_If, N_ElseIf, N_Else}}},

    {L_If, {
        {If, L_Block}}},

    {L_ElseIf, {
        {NONE},
        {ElseIf, L_Block, L_ElseIf}}},

    {L_Else, {
        {NONE},
        {Else, L_Block}}},
    
    {L_IfBlock, {
        {L_If, L_ElseIf, L_Else}}},


    /* Case-switch */
    {N_Case_0, {
        {DEFAULT, N_Block},
        {NONE}}},

    {N_Case_1, {
        {CASE, OPEN_PARENTHESIS, Term, CLOSE_PARENTHESIS, N_Block, N_Case_1},
        {NONE}}},

    {N_Case_2, {
        {N_Case_1, N_Case_0}}},


    {L_Case_0, {
        {DEFAULT, L_Block},
        {NONE}}},

    {L_Case_1, {
        {CASE, OPEN_PARENTHESIS, Term, CLOSE_PARENTHESIS, L_Block, L_Case_1},
        {NONE}}},

    {L_Case_2, {
        {L_Case_1, L_Case_0}}},


    {Switch_0, {
        {SWITCH, OPEN_PARENTHESIS, Term, CLOSE_PARENTHESIS}}},

    {N_Switch_1, {
        {Switch_0, OPEN_BRACE, N_Case_2, CLOSE_BRACE}}},

    {L_Switch_1, {
        {Switch_0, OPEN_BRACE, L_Case_2, CLOSE_BRACE}}},


    /* Function definitions */
    {Parameter, {
        {Datatype, IDENTIFIER}}},

    {NextParameter, {
        {NONE},
        {COMMA, Parameter, NextParameter}}},

    {ParameterList_0, {
        {Parameter, NextParameter},
        {NONE}}},

    {ParameterList_1, {
        {OPEN_PARENTHESIS, ParameterList_0, CLOSE_PARENTHESIS}}},

    {FunctionDeclaration, {
        {NONE},
        {VoidableDatatype, IDENTIFIER, ParameterList_1, N_Block, FunctionDeclaration}}},

    
    /* Function calls */
    {Argument, {
        {NONE},
        {COMMA, Term, Argument}}},

    {ArgumentList_0, {
        {NONE},
        {Term, Argument}}},

    {ArgumentList_1, {
        {OPEN_PARENTHESIS, ArgumentList_0, CLOSE_PARENTHESIS}}},

    {FunctionCall, {
        {CALL, IDENTIFIER, ArgumentList_1}}},


    /* Blocks */
    {ReturnContents, {
        {NONE},
        {Term}}},

    {N_Block_0, {
        {NONE},
        {SimpleStatement, N_Block_0},
        {For, N_Block_0},
        {While, N_Block_0},
        {N_Switch_1, N_Block_0},
        {N_IfBlock, N_Block_0},
        {RETURN, ReturnContents, SEMICOLON, N_Block_0}}},

    {N_Block, {
        {OPEN_BRACE, N_Block_0, CLOSE_BRACE}}},

    {L_Block_0, {
        {NONE},
        {SimpleStatement, L_Block_0},
        {For, L_Block_0},
        {While, L_Block_0},
        {L_Switch_1, L_Block_0},
        {L_IfBlock, L_Block_0},
        {RETURN, ReturnContents, SEMICOLON, L_Block_0},
        {BREAK, SEMICOLON, L_Block_0},
        {CONTINUE, SEMICOLON, L_Block_0}}},

    {L_Block, {
        {OPEN_BRACE, L_Block_0, CLOSE_BRACE}}},

    {Program, {
        {FunctionDeclaration}}}
};