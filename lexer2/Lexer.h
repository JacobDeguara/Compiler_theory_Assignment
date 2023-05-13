#ifndef __LEXER_H__
#define __LEXER_H__

#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <string>
#include <tuple>
#include <map>
#include <iomanip>

enum token_type // final state type return
{
    ENDOFFILE,           // end of file or errors
    ExceptionCharacter,  // ::= ',' | '=' | ':' | ';' | '(' | ')' | '{'| '}' | '->'
    MultiplicativeOp,    // ::= '*' | '/' | 'and'
    AdditiveOp,          // ::= '+' | '-' | 'or'
    RelationalOp,        // ::= '<' | '>' | '==' | '!=' | '<=' | '>='
    BooleanLiteral,      // ::= 'true' | 'false'
    UnaryOp,             // ::= 'not' | '!' ('-' is considered as part of this group but it already belongs to AdditiveOp)
    IntegerLiteral,      // ::= [0-9]
    FloatLiteral,        // ::= <Integer> '.' <Integer>
    ColourLiteral,       // ::= '#' <Hex> <Hex> <Hex> <Hex> <Hex> <Hex>
    Type,                // ::= 'float' | 'int' | 'bool' | 'colour'
    StatementOp,         // ::= 'return' | 'if' | 'for' | 'while' |'else'
    SpecialStatementsOp, // ::= '__print' | '__delay' | '__pixelr' | '__pixel' | '__fill' | '__clear' | '__exit'
    PadOp,               // ::= '__width' | '__height' | '__read' | '__randi' | '__max' | '__min'
    Declarator,          // ::= 'let'
    Func,                // ::= 'fun'
    Identifier,          // ::=  ( <Letter> | '_' ) { '_' | <Letter> | <Digit>}
    Comment,             // ::=  '//' {<anything>} '\n'
    error_token_type,
};

enum type_transition // type of item (single item types ' ' no more then 2)
{
    letter,               // [A-Z a-z]
    digit,                // [0-9]
    hex_,                 // [A-F a-f]
    hash_tag,             // [#]
    dot,                  // [.]
    comma,                // [,]
    equal_,               // [=]
    openBracket,          // [(]
    closeBracket,         // [)]
    openCurlyBracket,     // [{]
    closeCurlyBracket,    // [}]
    colon,                // [:]
    semi_colon,           // [;]
    greaterthen,          // [>]
    lessthen,             // [<]
    exclamationMark,      // [!]
    plus_,                // [+]
    minus_,               // [-]
    multiply,             // [*]
    divition,             // [/]
    underscore,           // [_]
    space,                // [' ']
    endOfLine,            // [\n]
    ENDOFFILE_transition, // [-1]
    anything_else,
};

enum state // the state
{
    start_state,                             // start ::= start here always
    identifier_state,                        // I  ::= ( <Letter> | '_' ) { '_' | <Letter> | <Digit>}
    comment_state,                           // C  ::= '//' {<anything>}
    hash_start,                              // HS ::= '#'
    hex1,                                    // H5 ::= <Hex>
    hex2,                                    // H5 ::= <Hex>
    hex3,                                    // H5 ::= <Hex>
    hex4,                                    // H5 ::= <Hex>
    hex5,                                    // H5 ::= <Hex>
    colour_state,                            // CO ::= '#' <Hex> <Hex> <Hex> <Hex> <Hex> <Hex>
    integer_state,                           // IN ::= {[0-9]}
    float_state,                             // F  ::= <Integer> '.' <Integer>
    unary,                                   // U  ::= '!'
    relationalOp_state,                      // RO ::= '<' | '>'
    relationalOp_state_2,                    // RO2 ::= '==' | '!=' | '<=' | '>='
    additiveOp_state,                        // AO ::= '+'
    slash_special_multiplicativeOp_state,    // SAO::= '/'
    multiplicativeOp_state,                  // MO ::= '*'
    minus_speical_additiveOp_state,          // SMO::= '-'
    exceptionCharacter_state,                // EC ::= ',' | ':' | ';' | '(' | ')' | '{'| '}' | '->'
    equals_special_expectionCharacter_state, // SEC::= '='
    ENDOFFILE_state,                         // EOF ::= -1

};

std::string convert_state_to_string(state s);
std::string convert_transition_to_string(type_transition tt);

std::string convert_transition_to_string(type_transition tt)
{
    switch (tt)
    {
    case letter:
        return "letter";
    case digit:
        return "digit";
    case hex_:
        return "hex";
    case hash_tag:
        return "#";
    case dot:
        return ".";
    case comma:
        return ",";
    case equal_:
        return "=";
    case openBracket:
        return "(";
    case closeBracket:
        return ")";
    case openCurlyBracket:
        return "{";
    case closeCurlyBracket:
        return "}";
    case colon:
        return ":";
    case semi_colon:
        return ";";
    case greaterthen:
        return ">";
    case lessthen:
        return "<";
    case exclamationMark:
        return "!";
    case plus_:
        return "+";
    case minus_:
        return "-";
    case multiply:
        return "*";
    case divition:
        return "/";
    case underscore:
        return "_";
    case space:
        return "' '";
    case endOfLine:
        return "endOfLine";
    case ENDOFFILE_transition:
        return "ENDOFFILE";
    case anything_else:
        return "anything_else";
    }
    return "None";
}

std::string convert_state_to_string(state s)
{
    switch (s)
    {
    case start_state:
        return "start_state";
    case identifier_state:
        return "identifier_state";
    case comment_state:
        return "comment_state";
    case hash_start:
        return "hash_start";
    case hex1:
        return "hex1";
    case hex2:
        return "hex2";
    case hex3:
        return "hex3";
    case hex4:
        return "hex4";
    case hex5:
        return "hex5";
    case colour_state:
        return "colour_state";
    case integer_state:
        return "integer_state";
    case float_state:
        return "float_state";
    case unary:
        return "unary";
    case relationalOp_state:
        return "relationalOp_state";
    case relationalOp_state_2:
        return "relationalOp_state_2";
    case additiveOp_state:
        return "additiveOp_state";
    case slash_special_multiplicativeOp_state:
        return "(/)-multiplicativeOp_state";
    case multiplicativeOp_state:
        return "multiplicativeOp_state";
    case minus_speical_additiveOp_state:
        return "(-)-additiveOp_state";
    case exceptionCharacter_state:
        return "exceptionCharacter_state";
    case equals_special_expectionCharacter_state:
        return "(=)-expectionCharacter_state";
    case ENDOFFILE_state:
        return "ENDOFFILE_state";
    }
    return "None";
}

using namespace std;

class Lexer
{
private:
    map<tuple<state, type_transition>, state> transitions;

    string file_name;
    long pos;
    tuple<std::string, token_type> current_token;
    bool echo = false;

    state current_state = start_state;

    // opens the file
    ifstream open_file();

    // closes the file
    void close_file(ifstream &file);

    type_transition get_transition_type(char c);

    bool automata(char c);

    // get_next_token
    tuple<std::string, state> get_next_token(ifstream &file);

    // checks the final state
    token_type get_final_state(state s);

    token_type identify(std::string str);

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

    ~Lexer() = default;
};

#endif // __LEXER_H__