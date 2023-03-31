#include <iostream> // std::cout
#include <fstream>  // std::ifstream

#include "Lexer.cpp"

bool run(const string &file_name);

int main()
{
    Lexer lexer = Lexer();
    lexer.set_up_file("test.txt");

    while (get<token_type>(lexer.get_next()) != ENDOFFILE)
    {
        cout << get<string>(lexer.get_current()) << endl;
    };
}
