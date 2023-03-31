#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <string>
#include <tuple>

using namespace std;

enum token_type
{
    ENDOFEQUATION,
    Letter,
    Digit,
    Hex,
    Type,
    BooleanLiteral,
    IntegerLiteral,
    FloatLiteral,
    ColourLiteral,
    MultiplicativeOp,
    AdditiveOp,
    RelationalOp,
    Identifier,
};

class Lexer
{
private:
    string file_name;
    long pos;
    tuple<string, token_type> current_token;

public:
    Lexer();

    // sets up the Lexer
    bool set_up_file(const string &file_name);

    // will give the next token
    tuple<string, token_type> get_next();

    // will give the string file_nametoken given before
    tuple<string, token_type> get_current();

    ~Lexer();
};

Lexer::Lexer(){};

bool Lexer::set_up_file(const string &file_name)
{
    this->file_name = file_name;
    pos = 0;
    ifstream file;
    file.open(file_name, std::ifstream::in);
    if (file.is_open())
    {

        return true;
    }
    else
    {
        cerr << "File wasnt setup" << endl;
        return false;
    }
};

tuple<string, token_type> Lexer::get_next()
{
    ifstream file;
    file.open(file_name, std::ifstream::in);
    file.seekg(pos);
    char c;
    while (file.good())
    {
        c = file.get();
        std::cout << c;
        if (c == '\n')
        {
            break;
        }
    }
    pos = file.tellg();
    return {" ", ENDOFEQUATION};
};

tuple<string, token_type> Lexer::get_current()
{
    return {" ", ENDOFEQUATION};
};

Lexer::~Lexer(){};
