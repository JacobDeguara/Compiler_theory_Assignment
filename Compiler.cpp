#include "Compiler.h"

Compiler::Compiler()
{
}

void Compiler::change_file(string file)
{
    this->filename = file;
}

void Compiler::compile_file()
{
    Parser parser = Parser(filename);
    auto result = parser.Compile(echo, print_tree, show_hidden);

    if (result)
    {
        Semantic_Analysis SemAn = Semantic_Analysis();
        try
        {
            SemAn.Program_Analysis(parser.get_root());
        }
        catch (const std::exception &e)
        {
            result == FAIL;
        }
    }

    if (result)
    {
        cout << "Compliation has succeded" << endl;
        PixLang_Converter out = PixLang_Converter(new_filename);
        out.print_code(parser.get_root());
    }
    else
    {
        cout << "Complilation has failed, check possible error message to resolve issue." << endl;
    }
}

void Compiler::enable_lexer_echo()
{
    this->echo = !echo;
}
void Compiler::enable_AST_XML_generator()
{
    this->print_tree = !print_tree;
}
void Compiler::show_hidden_XML_nodes()
{
    this->show_hidden = !show_hidden;
}
void Compiler::set_return_file(string file)
{
    this->new_filename = file;
}