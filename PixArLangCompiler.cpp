#include <iostream> // std::cout
#include <fstream>  // std::ifstream

#include "Lexer.cpp"

bool run(const string &file_name);

int main()
{
    Lexer lexer = Lexer();
    lexer.set_up_file("test.txt");
    lexer.get_next();
    lexer.get_next();
    lexer.get_next();
    lexer.get_next();
}
