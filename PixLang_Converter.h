#ifndef __PIXLANG_CONVERTER_H__
#define __PIXLANG_CONVERTER_H__

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>

#include "enums.h"

using namespace std;

class PixLang_Converter
{
private:
    vector<string> code;
    string file_out;

    void convert_program(shared_ptr<ASTree> node);
    // <Func_Decl> | <Variable_Decl> | <Assignment> | <Delay> | <Print> | <Pixel> | <While> | <For> | <If>
    void convert_block(shared_ptr<ASTree> node);
    // <Expr> <Block> [ <Block> ]
    void convert_if(shared_ptr<ASTree> node);
    // <Expr> <Block>
    void convert_while(shared_ptr<ASTree> node);
    // [ <Variable_Decliration> ] <Expr> [ <Assignment> ] <Block>
    void convert_for(shared_ptr<ASTree> node);
    // <Identitifer> <Expr>
    void convert_variable_decliration(shared_ptr<ASTree> node);
    // <Identifier> <Expr>
    void convert_assignment(shared_ptr<ASTree> node);
    // <Expr>
    void convert_delay(shared_ptr<ASTree> node);
    // <Expr>
    void convert_print(shared_ptr<ASTree> node);
    // <Expr> <Expr> <Expr>
    void convert_pixel(shared_ptr<ASTree> node);
    // <Expr> <Expr> <Expr> <Expr> <Expr>
    void convert_pixelr(shared_ptr<ASTree> node);
    // <Identifier> <Formal_Parms> <Type> <Block>
    void convert_function_decl(shared_ptr<ASTree> node);
    // <Expr>
    void convert_return(shared_ptr<ASTree> node);
    // <Function_Call> | <Unary> | <identifier> | <randi> | <read> | <height> | <width> | <number> | <bool> | <>
    void convert_expr(shared_ptr<ASTree> node);
    void convert_expr2(shared_ptr<ASTree> node);
    // <identifier> ( <Actual_Params> = { <Expr> } )
    void convert_function_call(shared_ptr<ASTree> node);
    // <Expr>
    void convert_unary(shared_ptr<ASTree> node);
    // <Identifier>
    void convert_identifier(shared_ptr<ASTree> node);
    // <Expr>
    void convert_randi(shared_ptr<ASTree> node);
    // <Expr>
    void convert_read(shared_ptr<ASTree> node);
    // nothing
    void convert_width(shared_ptr<ASTree> node);
    // nothing
    void convert_height(shared_ptr<ASTree> node);

public:
    PixLang_Converter(string file_out);
    void print_code_to_file();
    void test();

    void print_tree(shared_ptr<ASTree> start_node);
    // <Block>

    ~PixLang_Converter() = default;
};

// and -> min
// or -> max

#endif // __PIXLANG_CONVERTER_H__