#include <iostream> // std::cout
#include <fstream>  // std::ifstream

#include "Parser.cpp"

int main()
{
    Parser parser = Parser("test.txt");
    auto result = parser.Compile(false, true, false);

    if (result)
    {
        cout << "SUCCESS" << endl;
    }
    else
    {
        cout << "FAIL" << endl;
    }
}
