#ifndef __LEXER_H__
#define __LEXER_H__

#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <string>
#include <tuple>
#include <map>

#include "enums.h"
#include "FSA.cpp"

using namespace std;

class Lexer
{
private:
    FSA fsa = FSA();
    string file_name;
    long pos;
    tuple<string, token_type> current_token;
    bool echo = false;

    // opens the file
    ifstream open_file();

    // closes the file
    void close_file(ifstream &file);

    // get_next_token
    tuple<string, token_type> get_next_token(ifstream &file);

    // identifies the type of token (if token was given as identifier)
    tuple<string, token_type> identify(tuple<string, token_type> token);

public:
    Lexer();

    // sets up the Lexer
    bool set_up_file(const string &file_name);

    // will give the next token
    tuple<string, token_type> get_next();

    // will give the string file_nametoken given before
    tuple<string, token_type> get_current();

    // sets echo for testing
    void set_echo(bool echo);

    ~Lexer();
};

#endif // __LEXER_H__