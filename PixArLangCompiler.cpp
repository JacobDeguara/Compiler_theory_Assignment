#include <iostream> // std::cout
#include <fstream>  // std::ifstream

#include "Parser.cpp"
#include "AST_XML_gen.cpp"

int main()
{
    Parser parser = Parser("test.txt");
    auto result = parser.Compile(false, true, false);
}
