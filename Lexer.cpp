#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <string>
#include <tuple>

using namespace std;

enum token_type
{
    ENDOFFILE,
    ExceptionCharacter,
    Type,
    Boolean,
    Integer,
    Float,
    Colour,
    MultiplicativeOp,
    AdditiveOp,
    RelationalOp,
    Identifier,
    StatmentOp,
    UnaryOp,
    HexIdentifier,
    Comment,
};

class Lexer
{
private:
    string file_name;
    long pos;
    tuple<string, token_type> current_token;

    // opens the file
    ifstream open_file();

    // closes the file
    void close_file(ifstream &file);

    // get_next_token
    tuple<string, token_type> get_next_token(ifstream &file);

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
    if (!file.fail())
    {
        file.close();
        return true;
    }
    else
    {
        cerr << "file " << file_name << " doesnt exists" << endl;
        return false;
    }
};

ifstream Lexer::open_file()
{
    ifstream file;
    file.open(file_name, std::ifstream::in);
    file.seekg(pos);
    return file;
};

void Lexer::close_file(ifstream &file)
{
    pos = file.tellg();
    file.close();
};

tuple<string, token_type> Lexer::get_next_token(ifstream &file)
{
    string temp;
    char c;
    while (file.good())
    {
        if (file.eof())
            break;

        string next_char;

        // we peek the next character
        c = file.peek();
        switch (c)
        {
        case -1:
            if (temp.size() > 0)
                return {temp, Identifier};
            c = file.get();
            break;
        case ' ':
            if (temp.size() > 0)
                return {temp, Identifier};
            c = file.get();
            break;
        case '\n':
            c = file.get();
            break;
        case '(':
            if (temp.size() == 0)
            {
                c = file.get();
                return {"(", ExceptionCharacter};
            }
            return {temp, Identifier};
            break;
        case ')':
            if (temp.size() == 0)
            {
                c = file.get();
                return {")", ExceptionCharacter};
            }
            return {temp, Identifier};
            break;
        case '{':
            if (temp.size() == 0)
            {
                c = file.get();
                return {"{", ExceptionCharacter};
            }
            return {temp, Identifier};
            break;
        case '}':
            if (temp.size() == 0)
            {
                c = file.get();
                return {"}", ExceptionCharacter};
            }
            return {temp, Identifier};
            break;
        case ';':
            if (temp.size() == 0)
            {
                c = file.get();
                return {";", ExceptionCharacter};
            }
            return {temp, Identifier};
            break;
        case ',':
            if (temp.size() == 0)
            {
                c = file.get();
                return {",", ExceptionCharacter};
            }
            return {temp, Identifier};
            break;
        case ':':
            if (temp.size() == 0)
            {
                c = file.get();
                return {":", ExceptionCharacter};
            }
            return {temp, Identifier};
            break;
        case '+':
            if (temp.size() == 0)
            {
                c = file.get();
                return {"+", AdditiveOp};
            }
            return {temp, Identifier};
            break;
        case '*':
            if (temp.size() == 0)
            {
                c = file.get();
                return {"*", MultiplicativeOp};
            }
            return {temp, Identifier};
            break;
        case '#':
            if (temp.size() == 0)
            {
                c = file.get();
                return {"#", HexIdentifier};
            }
            return {temp, Identifier};
            break;
        case '/':
            if (temp.size() == 0)
            {
                temp.push_back(file.get());
                c = file.peek();
                if (c == '/')
                {
                    temp.push_back(file.get());
                    c = file.get();
                    while (file.good())
                    {
                        if (c == '\n')
                        {
                            break;
                        }
                        temp.push_back(c);
                        c = file.get();
                    }
                    return {temp, Comment};
                }
                else
                {
                    return {temp, ExceptionCharacter};
                }
            }
            return {temp, Identifier};
            break;
        case '=':
            if (temp.size() == 0)
            {
                temp.push_back(file.get());
                c = file.peek();
                if (c == '=')
                {
                    temp.push_back(file.get());
                    return {temp, RelationalOp};
                }
                else
                {
                    return {temp, ExceptionCharacter};
                }
            }
            return {temp, Identifier};
            break;
        case '!':
            if (temp.size() == 0)
            {
                temp.push_back(file.get());
                c = file.peek();
                if (c == '=')
                {
                    temp.push_back(file.get());
                    return {temp, RelationalOp};
                }
                else
                {
                    return {temp, UnaryOp};
                }
            }
            return {temp, Identifier};
            break;
        case '<':
            if (temp.size() == 0)
            {
                temp.push_back(file.get());
                c = file.peek();
                if (c == '=')
                {
                    temp.push_back(file.get());
                    return {temp, RelationalOp};
                }
                else
                {
                    return {temp, RelationalOp};
                }
            }
            return {temp, Identifier};
            break;
        case '>':
            if (temp.size() == 0)
            {
                temp.push_back(file.get());
                c = file.peek();
                if (c == '=')
                {
                    temp.push_back(file.get());
                    return {temp, RelationalOp};
                }
                else
                {
                    return {temp, RelationalOp};
                }
            }
            return {temp, Identifier};
            break;
        case '-':
            if (temp.size() == 0)
            {
                temp.push_back(file.get());
                c = file.peek();
                if (c == '>')
                {
                    temp.push_back(file.get());
                    return {temp, AdditiveOp};
                }
                else
                {
                    return {temp, ExceptionCharacter};
                }
            }
            return {temp, Identifier};
            break;
        }

        if (c != ' ' && c != '\n')
        {
            c = file.get();
            temp.push_back(c);
        }
    }
    return {"END OF FILE", ENDOFFILE};
}

tuple<string, token_type> Lexer::get_next()
{
    // open file
    auto file(open_file());

    // get token
    auto token = get_next_token(file);

    cout << " String(" << get<string>(token) << ") -> Token type: " << get<token_type>(token) << endl;

    /* identifier is the base case for every token
     * now we are gana check the identifier token is correct
     * by using a table driven FSA for stuff like Integer, Float, Colour, ect...
     */
    if (get<token_type>(token) == Identifier)
    {
    }

    // close file
    close_file(file);

    this->current_token = token;
    // give token
    return token;
};

tuple<string, token_type> Lexer::get_current()
{
    return current_token;
};

Lexer::~Lexer(){};
