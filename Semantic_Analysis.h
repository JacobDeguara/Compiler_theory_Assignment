#ifndef __SEMANTIC_ANALYSIS_H__
#define __SEMANTIC_ANALYSIS_H__

#include <iostream>
#include <string>
#include <tuple>
#include <memory>

#include "enums.h"

using namespace std;

struct parameter
{
    string identity;
    AST_token type;
};

struct function_set
{
    string identity;
    AST_token return_type;
    vector<parameter> parameters;

    // sets the parameter token passed as new paramter in paramters list
    void set_parameter(parameter parameter_token)
    {
        parameters.push_back(parameter_token);
    }

    /*
    checks weather the identity passed exists in the parameters list
    returns:
        > if found: <true, i {its position in the parameters list}>
        > else: <false, -1>
    */
    tuple<bool, int> if_paramter_id_exists(string identity)
    {
        for (size_t i = 0; i < parameters.size(); i++)
        {
            if (parameters.at(i).identity.compare(identity) == 0)
            {
                return {true, i};
            }
        }
        return {false, -1};
    }

    // returns the size of the list of paramters
    int get_paramters_size()
    {
        return parameters.size();
    }

    // get all the types of the paramters, in a vector list
    vector<AST_token> get_paramter_types()
    {
        vector<AST_token> list;
        for (size_t i = 0; i < parameters.size(); i++)
        {
            list.push_back(parameters.at(i).type);
        }
        return list;
    }

    // gets the parameters types at i
    AST_token get_parameter_type_at(int i)
    {
        return parameters.at(i).type;
    }
};

class Semantic_Analysis
{
private:
    vector<function_set> FUNCTION_DECLIRATION;

    void Block_Analysis_Start(shared_ptr<ASTree> block_node);

    // Function declirations
    void Function_Decliration_Analysis(shared_ptr<ASTree> func);
    void Block_Analysis_Func(shared_ptr<ASTree> block_node, vector<parameter> variables_declared_list, AST_token return_type);
    void While_Analysis_Func(shared_ptr<ASTree> while_node, vector<parameter> variables_declared_list, AST_token return_type);
    void For_Analysis_Func(shared_ptr<ASTree> for_node, vector<parameter> variables_declared_list, AST_token return_type);
    void If_Analysis_Func(shared_ptr<ASTree> if_node, vector<parameter> variables_declared_list, AST_token return_type);

    // functions with block
    void Block_Analysis(shared_ptr<ASTree> block_node, vector<parameter> variables_declared_list);
    void While_Analysis(shared_ptr<ASTree> while_node, vector<parameter> variables_declared_list);
    void For_Analysis(shared_ptr<ASTree> for_node, vector<parameter> variables_declared_list);
    void If_Analysis(shared_ptr<ASTree> if_node, vector<parameter> variables_declared_list);

    // Type checking with comparision
    AST_token Return_Analysis(shared_ptr<ASTree> return_node, vector<parameter> variables_declared_list);
    parameter Variable_decl_Analysis(shared_ptr<ASTree> variable_decl_node, vector<parameter> variables_declared_list);
    void Assignment_Analysis(shared_ptr<ASTree> assignment_node, vector<parameter> variables_declared_list);
    void Delay_Analysis(shared_ptr<ASTree> delay, vector<parameter> variables_declared_list);
    void Print_Analysis(shared_ptr<ASTree> print, vector<parameter> variables_declared_list);
    void Pixel_Analysis(shared_ptr<ASTree> pixel, vector<parameter> variables_declared_list);
    void Pixelr_Analysis(shared_ptr<ASTree> pixelr, vector<parameter> variables_declared_list);

    // Type checking to be used then Finding Expr;
    AST_token Type_checking(shared_ptr<ASTree> Expr, vector<parameter> variables_declared_list);
    AST_token Expr_analise(shared_ptr<ASTree> tree_token, vector<parameter> variables_declared_list);
    AST_token Unary_Analysis(shared_ptr<ASTree> unary, vector<parameter> variables_declared_list);
    AST_token Function_Call_Analysis(shared_ptr<ASTree> function_call, vector<parameter> variables_declared_list);
    AST_token Identifier_Analysis(shared_ptr<ASTree> identifier, vector<parameter> variables_declared_list);
    AST_token AdditiveOp_Analysis(shared_ptr<ASTree> identifier, vector<parameter> variables_declared_list);
    AST_token MultiplicativeOp_Analysis(shared_ptr<ASTree> identifier, vector<parameter> variables_declared_list);
    AST_token RelationalOp_Analysis(shared_ptr<ASTree> identifier, vector<parameter> variables_declared_list);
    AST_token Read_Analysis(shared_ptr<ASTree> identifier, vector<parameter> variables_declared_list);
    AST_token RandI_Analysis(shared_ptr<ASTree> identifier, vector<parameter> variables_declared_list);

    parameter create_parameter_struct(string id, AST_token type)
    {
        parameter temp;
        temp.identity = id;
        temp.type = type;
        return temp;
    }

    function_set create_function_set_struct(string id, AST_token return_type)
    {
        function_set temp;
        temp.identity = id;
        temp.return_type = return_type;
        return temp;
    }

    tuple<bool, int> if_exists_function_call(string id)
    {
        int i;
        for (i = 0; i < FUNCTION_DECLIRATION.size(); i++)
        {
            if (FUNCTION_DECLIRATION.at(i).identity.compare(id) == 0)
            {
                return {true, i};
            }
        }
        return {false, -1};
    }

    tuple<bool, int> if_exists_variable(string id, vector<parameter> variables_declared_list)
    {
        int i;
        for (i = 0; i < variables_declared_list.size(); i++)
        {
            auto param_identity = variables_declared_list.at(i).identity;
            if (param_identity.compare(id) == 0)
            {
                return {true, i};
            }
        }
        return {false, -1};
    }

    AST_token type_converter(string type)
    {
        if (type.compare("int") == 0)
        {
            return INTEGER_LITERAL;
        }
        else if (type.compare("float") == 0)
        {
            return FLOAT_LITERAL;
        }
        else if (type.compare("colour") == 0)
        {
            return COLOUR_LITERAL;
        }
        else if (type.compare("bool") == 0)
        {
            return BOOL_LITERAL;
        }
        return FAIL;
    }

public:
    Semantic_Analysis(/* args */);
    ~Semantic_Analysis() = default;
    void Program_Analysis(shared_ptr<ASTree> program);
};

#endif // __SEMANTIC_ANALYSIS_H__