#include <iostream> // std::cout
#include <fstream>  // std::ifstream

#include "Compiler.cpp"

int main()
{
    Compiler cmp = Compiler();

    cout << "Which file do you want to be compiled?" << endl;
    string file_in;
    cin >> file_in;
    cmp.change_file(file_in);

    cout << "Where do you want the compiled file to be named?" << endl;
    string file_out;
    cin >> file_out;
    cmp.set_return_file(file_out);

    cout << "Do you want to enable testing? (yes/no)" << endl;
    string enable;
    cin >> enable;

    if (enable.compare("yes") == 0)
    {
        cout << "You enabled testing,\nDo you want to show the lexer tags? (yes/no)" << endl;
        cin >> enable;

        if (enable.compare("yes") == 0)
        {
            cmp.enable_lexer_echo();
        }

        cout << "Do you want to generate a XML table for the AST tree? (yes/no)" << endl;
        cin >> enable;

        if (enable.compare("yes") == 0)
        {
            cmp.enable_AST_XML_generator();

            cout << "Do you want to show the hidden nodes (block/Expr)? (yes/no)" << endl;
            cin >> enable;
            if (enable.compare("yes") == 0)
            {
                cmp.show_hidden_XML_nodes();
            }
        }
    }

    cmp.compile_file();
}
