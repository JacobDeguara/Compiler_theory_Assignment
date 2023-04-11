#ifndef __COMPILER_H__
#define __COMPILER_H__

#include "Parser.cpp"
#include "Semantic_Analysis.cpp"

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
    void change_file(string file);
    void compile_file();
    void enable_lexer_echo();
    void enable_AST_XML_generator();
    void show_hidden_XML_nodes();
    void set_return_file(string file);
    ~Compiler() = default;
};

#endif // __COMPILER_H__