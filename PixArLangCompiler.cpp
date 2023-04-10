#include <iostream> // std::cout
#include <fstream>  // std::ifstream

#include "Parser.cpp"
#include "Semantic_Analysis.cpp"

int main()
{
    Parser parser = Parser("test.txt");
    auto result = parser.Compile(false, false, false);

    if (result)
    {
        cout << "SUCCESS" << endl;
    }
    else
    {
        cout << "FAIL" << endl;
    }
}
