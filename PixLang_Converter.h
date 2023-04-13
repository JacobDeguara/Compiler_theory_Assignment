#ifndef __PIXLANG_CONVERTER_H__
#define __PIXLANG_CONVERTER_H__

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>

#include "enums.h"

using namespace std;

struct variables
{
    // this is what the variable is called
    string id;
    // this is how to call the variable
    string call;
    int pos;
    int frame;
};

class PixLang_Converter
{
private:
    vector<string> code;
    vector<variables> var_decl;
    string file_out;

    // exactly like block but no return
    void convert_program(shared_ptr<ASTree> node);
    // <Func_Decl> | <Variable_Decl> | <Assignment> | <Delay> | <Print> | <Pixel> | <While> | <For> | <If>
    void convert_block(shared_ptr<ASTree> node, vector<variables> v, int scope_value);
    // <Expr> <Block> [ <Block> ]
    void convert_if(shared_ptr<ASTree> node, vector<variables> v, int scope_value);
    // <Expr> <Block>
    void convert_while(shared_ptr<ASTree> node, vector<variables> v, int scope_value);
    // [ <Variable_Decliration> ] <Expr> [ <Assignment> ] <Block>
    void convert_for(shared_ptr<ASTree> node, vector<variables> v, int scope_value);
    // <Identitifer> <Expr>
    variables convert_variable_decliration(shared_ptr<ASTree> node, vector<variables> v, int scope_value);
    // <Identifier> <Expr>
    void convert_assignment(shared_ptr<ASTree> node, vector<variables> v);
    // <Expr>
    void convert_delay(shared_ptr<ASTree> node, vector<variables> v);
    // <Expr>
    void convert_print(shared_ptr<ASTree> node, vector<variables> v);
    // <Expr> <Expr> <Expr>
    void convert_pixel(shared_ptr<ASTree> node, vector<variables> v);
    // <Expr> <Expr> <Expr> <Expr> <Expr>
    void convert_pixelr(shared_ptr<ASTree> node, vector<variables> v);
    // <Identifier> <Formal_Parms> <Type> <Block>
    void convert_function_decl(shared_ptr<ASTree> node);
    // <Expr>
    void convert_return(shared_ptr<ASTree> node, vector<variables> v);
    // <Function_Call> | <Unary> | <identifier> | <randi> | <read> | <height> | <width> | <number> | <bool> | <>
    void convert_expr(shared_ptr<ASTree> node, vector<variables> v);
    // convert_expr -> convert_expr2 to make it easier to deal with Expr
    void convert_expr2(shared_ptr<ASTree> node, vector<variables> v);
    // <identifier> ( <Actual_Params> = { <Expr> } )
    void convert_function_call(shared_ptr<ASTree> node, vector<variables> v);
    // <Expr>
    void convert_unary(shared_ptr<ASTree> node, vector<variables> v);
    // <Identifier>
    void convert_identifier(shared_ptr<ASTree> node, vector<variables> v);
    // <Expr>
    void convert_randi(shared_ptr<ASTree> node, vector<variables> v);
    // <Expr>
    void convert_read(shared_ptr<ASTree> node, vector<variables> v);
    // nothing
    void convert_width(shared_ptr<ASTree> node);
    // nothing
    void convert_height(shared_ptr<ASTree> node);
    // <Expr> <Expr>
    void convert_additive(shared_ptr<ASTree> node, vector<variables> v);
    // <Expr> <Expr>
    void convert_multiplicative(shared_ptr<ASTree> node, vector<variables> v);
    // <Expr> <Expr>
    void convert_relational(shared_ptr<ASTree> node, vector<variables> v);
    // nothing
    void convert_exit(shared_ptr<ASTree> node);
    // <Expr> <Expr>
    void convert_min_max(shared_ptr<ASTree> node, vector<variables> v);
    // <Expr> | nothing
    void convert_fill(shared_ptr<ASTree> node, vector<variables> v);

    // this is to get the right number for pos based on scope value
    size_t variable_size_based_on_scope(vector<variables> v, int scope_value);

    // prints the code vector to out file
    void print_code_to_file();

public:
    // sets up file out
    PixLang_Converter(string file_out);

    // this is to test file out if needed
    void test();

    // prints code from tree to file
    void print_code(shared_ptr<ASTree> start_node);

    ~PixLang_Converter() = default;
};

#endif // __PIXLANG_CONVERTER_H__