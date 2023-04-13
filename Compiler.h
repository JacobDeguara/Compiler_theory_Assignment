#ifndef __COMPILER_H__
#define __COMPILER_H__

#include <iostream>

#include "Parser.cpp"
#include "Semantic_Analysis.cpp"
#include "PixLang_Converter.cpp"

class Compiler
{
private:
    string filename;
    bool echo = false;
    bool print_tree = false;
    bool show_hidden = false;
    string new_filename;

public:
    Compiler();

    // chnages input file
    void change_file(string file);
    // echo's lexer output
    void enable_lexer_echo();
    // generates XML for tree
    void enable_AST_XML_generator();
    // shows XML hidden nodes (<Expr> / <Block>)
    void show_hidden_XML_nodes();
    // changes return file
    void set_return_file(string file);
    // compiles the file
    void compile_file();
    ~Compiler() = default;
};

#endif // __COMPILER_H__