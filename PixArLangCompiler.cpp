#include <iostream> // std::cout
#include <fstream>  // std::ifstream

#include "Parser.cpp"

int main()
{
    Parser parser = Parser("test.txt");
    parser.Compile();

    Lexer lexer = Lexer();
    lexer.set_up_file("test.txt");
    // while (get<token_type>(lexer.get_next()) != ENDOFFILE)
}
