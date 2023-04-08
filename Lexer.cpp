#include "Lexer.h"

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
            if (temp.size() > 0)
                return {temp, Identifier};
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
                    return {temp, MultiplicativeOp};
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
                    return {temp, ExceptionCharacter};
                }
                else
                {
                    return {temp, AdditiveOp};
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

tuple<string, token_type> Lexer::identify(tuple<string, token_type> token)
{
    auto token_name = get<string>(token);
    auto type = fsa.StartNFSA(token_name);
    if (type == ENDOFFILE)
    {
        cerr << "Syntax Error: " << token_name << " has wrong syntax" << endl;
        exit(EXIT_FAILURE);
    }

    if (type == Identifier)
    {
        if (token_name.compare("float") == 0 || token_name.compare("int") == 0 || token_name.compare("bool") == 0 || token_name.compare("colour") == 0)
        {
            type = Type;
        }
        else if (token_name.compare("return") == 0 || token_name.compare("if") == 0 || token_name.compare("for") == 0 || token_name.compare("while") == 0 || token_name.compare("else") == 0)
        {
            type = StatementOp;
        }
        else if (token_name.compare("and") == 0)
        {
            type = MultiplicativeOp;
        }
        else if (token_name.compare("or") == 0)
        {
            type = AdditiveOp;
        }
        else if (token_name.compare("not") == 0)
        {
            type = UnaryOp;
        }
        else if (token_name.compare("__width") == 0 || token_name.compare("__height") == 0 || token_name.compare("__read") == 0 || token_name.compare("__randi") == 0)
        {
            type = PadOp;
        }
        else if (token_name.compare("true") == 0 || token_name.compare("false") == 0)
        {
            type = BooleanLiteral;
        }
        else if (token_name.compare("let") == 0)
        {
            type = Declarator;
        }
        else if (token_name.compare("__print") == 0 || token_name.compare("__delay") == 0 || token_name.compare("__pixelr") == 0 || token_name.compare("__pixel") == 0)
        {
            type = SpecialStatementsOp;
        }
        else if (token_name.compare("fun") == 0)
        {
            type = Func;
        }
    }

    return {token_name, type};
}

tuple<string, token_type> Lexer::get_next()
{
    // open file
    auto file(open_file());

    // get token
    auto token = get_next_token(file);

    // dont give comments to the parser, we ignore them.
    while (get<token_type>(token) == Comment)
    {
        // this is for testing purpuses
        // cout << "-- lexer note: comment ignored --" << endl;
        token = get_next_token(file);
    }

    // this is for testing purpuses
    // cout << " String(" << get<string>(token) << ") -> Token type: " << get<token_type>(token) << endl;

    /* identifier is the base case for every token
     * now we are gana check the identifier token is correct
     * by using a table driven FSA for stuff like Integer, Float, Colour, ect...
     */
    if (get<token_type>(token) == Identifier)
    {
        token = identify(token);
        // this is for testing purpuses
        //  cout << "-- Token type changed to: " << get<token_type>(token) << " --" << endl;
    }

    // this is for testing purpuses
    if (echo)
    {
        cout << " Token(" << get<string>(token) << ") -> Token type: " << get<token_type>(token) << endl;
    }

    // close file
    close_file(file);

    // set current token
    this->current_token = token;

    // give token
    return token;
};

tuple<string, token_type> Lexer::get_current()
{
    return current_token;
};

void Lexer::set_echo(bool echo)
{
    this->echo = echo;
}

Lexer::~Lexer(){};
