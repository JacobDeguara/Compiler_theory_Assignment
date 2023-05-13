#include <iostream>

#include "Lexer.cpp"
using namespace std;

int main()
{
    Lexer lexer = Lexer();

    lexer.set_up_file("test.txt");
    lexer.set_echo(true);
    while (get<1>(lexer.get_next()) != ENDOFFILE)
    {
    }
}