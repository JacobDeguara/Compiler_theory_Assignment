#ifndef __SEMENTIC_ANALYSIS_H__
#define __SEMENTIC_ANALYSIS_H__

#include "Semantic_Analysis.h"

// Function declirations
void Function_Decliration_Analysis(shared_ptr<ASTree> pixelr, vector<parameter> variables_declared_list);

// functions with block
void While_Analysis(shared_ptr<ASTree> pixelr, vector<parameter> variables_declared_list);
void For_Analysis(shared_ptr<ASTree> pixelr, vector<parameter> variables_declared_list);
void If_Analysis(shared_ptr<ASTree> pixelr, vector<parameter> variables_declared_list);

// Type checking with comparision
AST_token Return_Analysis(shared_ptr<ASTree> return_node, vector<parameter> variables_declared_list)
{
    // sanity check
    if (return_node->token != RTRN_STATEMENT)
    {
        cerr << "RETURN not of type RETURN" << endl;
        exit(EXIT_FAILURE);
    }

    return Type_checking(return_node->Leaf.at(0), variables_declared_list);
}
parameter Variable_decl_Analysis(shared_ptr<ASTree> variable_decl_node, vector<parameter> variables_declared_list)
{
    // sanity check
    if (variable_decl_node->token != VARIABLE_DECL)
    {
        cerr << "VARIABLE_DECL not of type VARIABLE_DECL" << endl;
        exit(EXIT_FAILURE);
    }

    auto id = variable_decl_node->Leaf.at(0)->text;
    auto type = type_converter(variable_decl_node->Leaf.at(1)->text);
    auto Expr = Type_checking(variable_decl_node->Leaf.at(2), variables_declared_list);

    // check if variable exits before hand
    int i;
    bool exists_flag;
    tie(i, exists_flag) = if_exists_variable(id, variables_declared_list);

    if (exists_flag == true)
    {
        cerr << "Variable " << id << " has already be initilized" << endl;
        exit(EXIT_SUCCESS);
    }

    // check if expr is of correct type
    if (type != Expr)
    {
        cerr << "Variable " << id << " must be of type " << variable_decl_node->Leaf.at(1)->text << endl;
        exit(EXIT_SUCCESS);
    }

    // create new parameter and return it
    return create_parameter_struct(id, type);
}

void Assignment_Analysis(shared_ptr<ASTree> assignment_node, vector<parameter> variables_declared_list)
{
    // sanity check
    if (assignment_node->token != ASSIGNMENT)
    {
        cerr << "ASSIGNMENT not of type ASSIGNMENT" << endl;
        exit(EXIT_FAILURE);
    }

    auto id = assignment_node->Leaf.at(0)->text;
    auto Expr = Type_checking(assignment_node->Leaf.at(1), variables_declared_list);

    int position;
    bool exists_flag = false;
    tie(position, exists_flag) = if_exists_variable(id, variables_declared_list);

    if (exists_flag == false)
    {
        cerr << "Variable " << id << " hasnt been initilized yet" << endl;
        exit(EXIT_SUCCESS);
    }
}

void Delay_Analysis(shared_ptr<ASTree> delay, vector<parameter> variables_declared_list)
{
    // sanity check
    if (delay->token != PRINT_STATEMENT)
    {
        cerr << "DELAY not of type DELAY" << endl;
        exit(EXIT_FAILURE);
    }

    auto Expr = Type_checking(delay->Leaf.at(0), variables_declared_list);

    if (Expr == INTEGER_LITERAL)
    {
        return;
    }

    cerr << "The delay operator requires 1 integers types only" << endl;
    exit(EXIT_SUCCESS);
}

void Print_Analysis(shared_ptr<ASTree> print, vector<parameter> variables_declared_list)
{
    // sanity check
    if (print->token != PRINT_STATEMENT)
    {
        cerr << "PRINT not of type PRINT" << endl;
        exit(EXIT_FAILURE);
    }

    auto Expr = Type_checking(print->Leaf.at(0), variables_declared_list);

    // as long as its a valid Expr it works
    return;
}
void Pixel_Analysis(shared_ptr<ASTree> pixel, vector<parameter> variables_declared_list)
{
    // sanity check
    if (pixel->token != PIXEL_STATEMENT)
    {
        cerr << "__PIXEL not of type __PIXEL" << endl;
        exit(EXIT_FAILURE);
    }

    auto Expr1 = Type_checking(pixel->Leaf.at(0), variables_declared_list);
    auto Expr2 = Type_checking(pixel->Leaf.at(1), variables_declared_list);
    auto Expr3 = Type_checking(pixel->Leaf.at(2), variables_declared_list);

    if (Expr1 == INTEGER_LITERAL && Expr2 == INTEGER_LITERAL && Expr3 == COLOUR_LITERAL)
    {
        return;
    }

    cerr << "The pixel operator requires 2 integers types and 1 colour type at the end" << endl;
    exit(EXIT_SUCCESS);
}

void Pixelr_Analysis(shared_ptr<ASTree> pixelr, vector<parameter> variables_declared_list)
{
    // sanity check
    if (pixelr->token != PIXELR_STATEMENT)
    {
        cerr << "__PIXELR not of type __PIXELR" << endl;
        exit(EXIT_FAILURE);
    }

    auto Expr1 = Type_checking(pixelr->Leaf.at(0), variables_declared_list);
    auto Expr2 = Type_checking(pixelr->Leaf.at(1), variables_declared_list);
    auto Expr3 = Type_checking(pixelr->Leaf.at(2), variables_declared_list);
    auto Expr4 = Type_checking(pixelr->Leaf.at(3), variables_declared_list);
    auto Expr5 = Type_checking(pixelr->Leaf.at(4), variables_declared_list);

    if (Expr1 == INTEGER_LITERAL && Expr2 == INTEGER_LITERAL && Expr3 == INTEGER_LITERAL && Expr4 == INTEGER_LITERAL && Expr5 == COLOUR_LITERAL)
    {
        return;
    }

    cerr << "The pixelr operator requires 4 integers types and 1 colour type at the end" << endl;
    exit(EXIT_SUCCESS);
}

// Type checking to be used then Finding Expr;
AST_token Type_checking(shared_ptr<ASTree> Expr, vector<parameter> variables_declared_list)
{
    // sanity check
    if (Expr->token != EXPR)
    {
        cerr << "EXPR not of type EXPR" << endl;
        exit(EXIT_FAILURE);
    }
    return Expr_analise(Expr->Leaf.at(0), variables_declared_list);
}

AST_token Expr_analise(shared_ptr<ASTree> tree_token, vector<parameter> variables_declared_list)
{
    auto token = tree_token->token;

    switch (token)
    {
    case PAD_RANDI: // ::= int
        return RandI_Analysis(tree_token, variables_declared_list);
    case PAD_WIDTH: // ::= int
        return INTEGER_LITERAL;
    case PAD_HEIGHT: // ::= int
        return INTEGER_LITERAL;
    case PAD_READ: // ::= colour
        return Read_Analysis(tree_token, variables_declared_list);
    case BOOL_LITERAL: // ::= bool
        return BOOL_LITERAL;
    case INTEGER_LITERAL: // ::= int
        return INTEGER_LITERAL;
    case FLOAT_LITERAL: // ::= float
        return FLOAT_LITERAL;
    case COLOUR_LITERAL: // ::= colour
        return COLOUR_LITERAL;
    case IDENTIFIER: // ::= any
        return Identifier_Analysis(tree_token, variables_declared_list);
    case FUNCTION_CALL: // ::= <identifier> , <ActualParams>
        return Function_Call_Analysis(tree_token, variables_declared_list);
    case UNARY: // ::= not and ! only works with bool, - works with int, float
        return Unary_Analysis(tree_token, variables_declared_list);
    case ADDITIVEOP: // ::= int + int => int , float + int/ float => float, colour => fail, 'or' works with bool
        return AdditiveOp_Analysis(tree_token, variables_declared_list);
    case MULTIPLICATIVEOP: // ::= float * or / with int or float => float, int * int=> int , int / int => float, colour => fail , 'and' works with bool
        return MultiplicativeOp_Analysis(tree_token, variables_declared_list);
    case RELATIONALOP: // ::=  < , > , <= , >= work with float, colour and int, but == , != only work with everything
        return RelationalOp_Analysis(tree_token, variables_declared_list);
    default:
        cerr << "EXPR FAILED type" << token << endl;
        exit(EXIT_FAILURE);
        break;
    }
}

AST_token Read_Analysis(shared_ptr<ASTree> read_node, vector<parameter> variables_declared_list)
{
    // sanity check
    if (read_node->token != PAD_READ)
    {
        cerr << "__READ not of type __READ" << endl;
        exit(EXIT_FAILURE);
    }

    auto Expr1 = Type_checking(read_node->Leaf.at(0), variables_declared_list);
    auto Expr2 = Type_checking(read_node->Leaf.at(1), variables_declared_list);

    if (Expr1 == INTEGER_LITERAL && Expr2 == INTEGER_LITERAL)
    {
        return COLOUR_LITERAL;
    }

    cerr << "__read can only take int type " << endl;
    exit(EXIT_SUCCESS);
}

AST_token RandI_Analysis(shared_ptr<ASTree> randi_node, vector<parameter> variables_declared_list)
{
    // sanity check
    if (randi_node->token != PAD_RANDI)
    {
        cerr << "__RANDI not of type __RANDI" << endl;
        exit(EXIT_FAILURE);
    }

    auto Expr1 = Type_checking(randi_node->Leaf.at(0), variables_declared_list);

    if (Expr1 == INTEGER_LITERAL)
    {
        return INTEGER_LITERAL;
    }

    cerr << "__randi can only take int type " << endl;
    exit(EXIT_SUCCESS);
}

AST_token AdditiveOp_Analysis(shared_ptr<ASTree> additive_op, vector<parameter> variables_declared_list)
{
    // sanity check
    if (additive_op->token != ADDITIVEOP)
    {
        cerr << "ADDITIVEOP not of type ADDITIVEOP" << endl;
        exit(EXIT_FAILURE);
    }

    // operators has 2 leafs always
    auto type_left = Expr_analise(additive_op->Leaf.at(0), variables_declared_list);
    auto type_right = Expr_analise(additive_op->Leaf.at(1), variables_declared_list);

    if (additive_op->text.compare("+") == 0 || additive_op->text.compare("-") == 0)
    {
        if ((type_left == FLOAT_LITERAL || type_left == INTEGER_LITERAL) && (type_right == FLOAT_LITERAL || type_right == INTEGER_LITERAL))
        {
            if (type_left == FLOAT_LITERAL || type_right == FLOAT_LITERAL)
            {
                return FLOAT_LITERAL;
            }
            return INTEGER_LITERAL;
        }

        cerr << "Operator " << additive_op->text << " can only be used with float or integer type" << endl;
        exit(EXIT_SUCCESS);
    }

    if (additive_op->text.compare("or") == 0)
    {
        if (type_left == BOOL_LITERAL && type_right == BOOL_LITERAL)
        {
            return BOOL_LITERAL;
        }

        cerr << "Operator " << additive_op->text << " can only be used with bool type" << endl;
        exit(EXIT_SUCCESS);
    }

    cerr << "OPERATOR is " << additive_op->text << " which isnt correct" << endl;
    exit(EXIT_FAILURE);
}

AST_token MultiplicativeOp_Analysis(shared_ptr<ASTree> multi_op, vector<parameter> variables_declared_list)
{
    // sanity check
    if (multi_op->token != MULTIPLICATIVEOP)
    {
        cerr << "MULTIPLICATIVEOP not of type MULTIPLICATIVEOP" << endl;
        exit(EXIT_FAILURE);
    }

    // operators has 2 leafs always
    auto type_left = Expr_analise(multi_op->Leaf.at(0), variables_declared_list);
    auto type_right = Expr_analise(multi_op->Leaf.at(1), variables_declared_list);

    if (multi_op->text.compare("*") == 0 || multi_op->text.compare("/") == 0)
    {
        if ((type_left == FLOAT_LITERAL || type_left == INTEGER_LITERAL) && (type_right == FLOAT_LITERAL || type_right == INTEGER_LITERAL))
        {
            if (type_left == FLOAT_LITERAL || type_right == FLOAT_LITERAL)
            {
                return FLOAT_LITERAL;
            }
            return INTEGER_LITERAL;
        }

        cerr << "Operator " << multi_op->text << " can only be used with float or integer type" << endl;
        exit(EXIT_SUCCESS);
    }

    if (multi_op->text.compare("and") == 0)
    {
        if (type_left == BOOL_LITERAL && type_right == BOOL_LITERAL)
        {
            return BOOL_LITERAL;
        }

        cerr << "Operator " << multi_op->text << " can only be used with bool type" << endl;
        exit(EXIT_SUCCESS);
    }

    cerr << "OPERATOR is " << multi_op->text << " which isnt correct" << endl;
    exit(EXIT_FAILURE);
}

AST_token RelationalOp_Analysis(shared_ptr<ASTree> relation_op, vector<parameter> variables_declared_list)
{
    // sanity check
    if (relation_op->token != RELATIONALOP)
    {
        cerr << "RELATIONALOP not of type RELATIONALOP" << endl;
        exit(EXIT_FAILURE);
    }

    // operators has 2 leafs always
    auto type_left = Expr_analise(relation_op->Leaf.at(0), variables_declared_list);
    auto type_right = Expr_analise(relation_op->Leaf.at(1), variables_declared_list);

    if (relation_op->text.compare("<") == 0 || relation_op->text.compare(">") == 0 || relation_op->text.compare("<=") == 0 || relation_op->text.compare(">=") == 0)
    {
        if ((type_left == FLOAT_LITERAL || type_left == INTEGER_LITERAL) && (type_right == FLOAT_LITERAL || type_right == INTEGER_LITERAL))
        {
            return BOOL_LITERAL;
        }

        if (type_left == COLOUR_LITERAL && type_right == COLOUR_LITERAL)
        {
            return BOOL_LITERAL;
        }

        cerr << "Operator " << relation_op->text << " can only be used with float and int type or with only colour types" << endl;
        exit(EXIT_SUCCESS);
    }

    if (relation_op->text.compare("==") == 0 || relation_op->text.compare("!=") == 0)
    {
        return BOOL_LITERAL;
    }

    cerr << "OPERATOR is " << relation_op->text << " which isnt correct" << endl;
    exit(EXIT_FAILURE);
}

AST_token Identifier_Analysis(shared_ptr<ASTree> identifier, vector<parameter> variables_declared_list)
{
    // sanity check
    if (identifier->token != IDENTIFIER)
    {
        cerr << "IDENTIFIER not of type IDENTIFIER" << endl;
        exit(EXIT_FAILURE);
    }

    auto id = identifier->text;
    int position;
    bool exists_flag;

    tie(position, exists_flag) = if_exists_variable(id, variables_declared_list);

    if (exists_flag)
    {
        return variables_declared_list.at(position).type;
    }
    cerr << "Variable " << id << " hasnt been initilized yet" << endl;
    exit(EXIT_SUCCESS);
}

AST_token Unary_Analysis(shared_ptr<ASTree> unary, vector<parameter> variables_declared_list)
{
    // sanity check
    if (unary->token != UNARY)
    {
        cerr << "UNARY not of type UNARY" << endl;
        exit(EXIT_FAILURE);
    }

    //<Unary> -> leaf.at(0) => <Expr>
    auto type = Type_checking(unary->Leaf.at(0), variables_declared_list);

    // if unary == '!' | 'not' but type != bool
    if ((unary->text.compare("!") == 0 || unary->text.compare("not") == 0) && type != BOOL_LITERAL)
    {
        cerr << "'!' and 'not' can only be used with type bool" << endl;
        exit(EXIT_SUCCESS);
    }

    // if unary == '-' but type != int | bool
    if (unary->text.compare("-") == 0 && (type != FLOAT_LITERAL || type != INTEGER_LITERAL))
    {
        cerr << "'-' can only be used with type int or float" << endl;
        exit(EXIT_SUCCESS);
    }

    return type;
}

AST_token Function_Call_Analysis(shared_ptr<ASTree> function_call, vector<parameter> variables_declared_list)
{
    // sanity check
    if (function_call->token != FUNCTION_CALL)
    {
        cerr << "FUNCTION_CALL not of type FUNCTION_CALL" << endl;
        exit(EXIT_FAILURE);
    }

    // function_call -> Leaf.at(0) => Identfier , Leaf.at(1) => <Actual_Parameters> -> Leaf => vector<Actual_parameter>
    string identifier = function_call->Leaf.at(0)->text;
    auto function_parameters_list = function_call->Leaf.at(1)->Leaf;

    // check if function call exists
    int position;
    bool exists_flag;
    tie(exists_flag, position) = if_exists_function_call(identifier);
    if (exists_flag == false)
    {
        cerr << "Function " << identifier << "() wasnt created yet." << endl;
        exit(EXIT_SUCCESS);
    }

    auto parameters_list_types = FUNCTION_DECLIRATION.at(position).get_paramter_types();

    // check if the size of both lists is equal
    if (parameters_list_types.size() != function_parameters_list.size())
    {
        cerr << "Function " << identifier << "() requires " << function_parameters_list.size() << " you wrote " << parameters_list_types.size() << " amount of paramters." << endl;
        exit(EXIT_SUCCESS);
    }

    // check if each type is correct
    for (size_t i = 0; i < parameters_list_types.size(); i++)
    {
        if (Type_checking(function_parameters_list.at(i), variables_declared_list) != parameters_list_types.at(i))
        {
            cerr << "Function " << identifier << "() requires parameter " << i << " to be of correct typeing";
            exit(EXIT_SUCCESS);
        }
    }

    // return the return_type of the function
    return FUNCTION_DECLIRATION.at(position).return_type;
}

#endif // __SEMANTIC_ANALYSIS_H__