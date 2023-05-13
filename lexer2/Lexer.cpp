#include "Lexer.h"

Lexer::Lexer()
{
    map<tuple<state, type_transition>, state>::iterator it = transitions.begin();

    // create transition tree
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, letter), identifier_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, underscore), identifier_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(identifier_state, letter), identifier_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(identifier_state, underscore), identifier_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(identifier_state, digit), identifier_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, hash_tag), hash_start));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(hash_start, hex_), hex1));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(hex1, hex_), hex2));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(hex2, hex_), hex3));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(hex3, hex_), hex4));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(hex4, hex_), hex5));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(hex5, hex_), colour_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, digit), integer_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(integer_state, digit), integer_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(integer_state, dot), float_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(float_state, digit), float_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, exclamationMark), unary));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(unary, equal_), relationalOp_state_2));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, greaterthen), relationalOp_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, lessthen), relationalOp_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(relationalOp_state, equal_), relationalOp_state_2));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, equal_), equals_special_expectionCharacter_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(equals_special_expectionCharacter_state, equal_), relationalOp_state_2));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, multiply), multiplicativeOp_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, plus_), additiveOp_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, divition), slash_special_multiplicativeOp_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, minus_), minus_speical_additiveOp_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(slash_special_multiplicativeOp_state, divition), comment_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(minus_speical_additiveOp_state, greaterthen), exceptionCharacter_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, comma), exceptionCharacter_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, openBracket), exceptionCharacter_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, closeBracket), exceptionCharacter_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, openCurlyBracket), exceptionCharacter_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, closeCurlyBracket), exceptionCharacter_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, colon), exceptionCharacter_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, semi_colon), exceptionCharacter_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, space), start_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, ENDOFFILE_transition), ENDOFFILE_state));
    transitions.insert(it, pair<tuple<state, type_transition>, state>(pair<state, type_transition>(start_state, endOfLine), start_state));

    // display transition tree for testing
    /*
    std::cout << "transitions map contains:\n";
    std::cout << "(" << std::setw(28);
    std::cout << "-State-"
              << " , " << std::setw(12);
    std::cout << "-Transition-"
              << ") => "
              << "-State-" << '\n';
    for (it = transitions.begin(); it != transitions.end(); ++it)
    {
        std::cout << "(" << std::setw(28);
        std::cout << convert_state_to_string(get<0>(it->first)) << " , " << std::setw(12);
        std::cout << convert_transition_to_string(get<1>(it->first)) << ") => " << convert_state_to_string(it->second) << '\n';
    }
    */
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

type_transition Lexer::get_transition_type(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return letter;
    if ((c >= '0' && c <= '9'))
        return digit;
    if (c == '#')
        return hash_tag;
    if (c == '.')
        return dot;
    if (c == ',')
        return comma;
    if (c == '=')
        return equal_;
    if (c == '(')
        return openBracket;
    if (c == ')')
        return closeBracket;
    if (c == '{')
        return openCurlyBracket;
    if (c == '}')
        return closeCurlyBracket;
    if (c == ';')
        return semi_colon;
    if (c == '>')
        return greaterthen;
    if (c == '<')
        return lessthen;
    if (c == '!')
        return exclamationMark;
    if (c == '+')
        return plus_;
    if (c == '-')
        return minus_;
    if (c == '*')
        return multiply;
    if (c == '/')
        return divition;
    if (c == '_')
        return underscore;
    if (c == ' ')
        return space;
    if (c == '\n')
        return endOfLine;
    if (c == ':')
        return colon;
    if (c == -1)
        return ENDOFFILE_transition;

    return anything_else;
}

bool check_hex(char c)
{
    if ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9'))
        return true;

    return false;
}

bool Lexer::automata(char c)
{
    /* --- get transition type from char c --- */
    auto trans_type = get_transition_type(c);

    /* --- special case if in hash_start onwards we need to check if c is hash --- */
    if (current_state == hex1 || current_state == hex2 || current_state == hex3 || current_state == hex4 || current_state == hex5 || current_state == hash_start)
    {
        if (check_hex(c))
        {
            trans_type = hex_;
        }
    }

    /* --- find the mapping of current state and transition type --- */
    auto found_item = transitions.find(pair<state, type_transition>(current_state, trans_type));

    /* --- if transition wasnt found return failer --- */
    if (found_item == transitions.end())
        return false;

    /* --- else set current state to next state and return success --- */
    this->current_state = found_item->second;
    return true;
}

tuple<string, state> Lexer::get_next_token(ifstream &file)
{
    string temp;
    char c;

    while (file.good())
    {
        c = file.peek();

        if (!automata(c))
        {
            state s = current_state;
            this->current_state = start_state;
            return {temp, s};
        }

        // if space dont add to temp
        if (c != ' ' && c != '\n')
        {
            temp.push_back(c);
        }

        // token is a comment
        if (current_state == comment_state)
        {
            while (file.get() != '\n')
            {
            }
            temp.clear();
            this->current_state = start_state;
        }
        else
            // go next;
            file.get();
    }

    return {"END OF FILE", ENDOFFILE_state};
}

token_type Lexer::get_final_state(state s)
{
    switch (s)
    {
    case identifier_state:
        return Identifier;
    case comment_state:
        return Comment;
    case colour_state:
        return ColourLiteral;
    case integer_state:
        return IntegerLiteral;
    case float_state:
        return FloatLiteral;
    case unary:
        return UnaryOp;
    case relationalOp_state:
        return RelationalOp;
    case relationalOp_state_2:
        return RelationalOp;
    case additiveOp_state:
        return AdditiveOp;
    case slash_special_multiplicativeOp_state:
        return MultiplicativeOp;
    case multiplicativeOp_state:
        return MultiplicativeOp;
    case minus_speical_additiveOp_state:
        return AdditiveOp;
    case exceptionCharacter_state:
        return ExceptionCharacter;
    case equals_special_expectionCharacter_state:
        return ExceptionCharacter;
    case ENDOFFILE_state:
        return ENDOFFILE;
    }
    return error_token_type;
}

token_type Lexer::identify(std::string str)
{
    if (str.compare("and") == 0)
        return MultiplicativeOp;
    if (str.compare("or") == 0)
        return AdditiveOp;
    if (str.compare("true") == 0 || str.compare("false") == 0)
        return BooleanLiteral;
    if (str.compare("not") == 0)
        return UnaryOp;
    if (str.compare("float") == 0 || str.compare("int") == 0 || str.compare("bool") == 0 || str.compare("colour") == 0)
        return Type;
    if (str.compare("return") == 0 || str.compare("if") == 0 || str.compare("for") == 0 || str.compare("while") == 0 || str.compare("else") == 0)
        return StatementOp;
    if (str.compare("__print") == 0 || str.compare("__delay") == 0 || str.compare("__pixelr") == 0 || str.compare("__pixel") == 0 || str.compare("__fill") == 0 || str.compare("__clear") == 0 || str.compare("__exit") == 0)
        return SpecialStatementsOp;
    if (str.compare("__width") == 0 || str.compare("__height") == 0 || str.compare("__read") == 0 || str.compare("__randi") == 0 || str.compare("__max") == 0 || str.compare("__min") == 0)
        return PadOp;
    if (str.compare("let") == 0)
        return Declarator;
    if (str.compare("fun") == 0)
        return Func;
    return Identifier;
}

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

tuple<std::string, token_type> Lexer::get_next()
{
    /* --- open file --- */
    auto file(open_file());

    /* --- unpack elements (string, state) --- */

    string str;
    state state_s;
    std::tie(str, state_s) = get_next_token(file);

    /* --- get final state type --- */
    token_type final_type = get_final_state(state_s);

    /* --- if final_state error exit --- */
    if (final_type == error_token_type)
    {
        cout << "error occured when lexing the tokens (probs: have #123456 not up to 6 hexes)" << endl;
        exit(EXIT_SUCCESS);
    }

    /* --- if final_state is identifier check if its any special character --- */
    if (final_type == Identifier)
    {
        final_type = identify(str);
    }

    /* --- tokenize the string --- */
    auto result = make_tuple(str, final_type);

    if (echo)
    {
        cout << "Token: \"" << get<0>(result) << "\" is type " << get<1>(result) << endl;
    }

    /* --- close file --- */
    close_file(file);

    /* --- set current token --- */
    this->current_token = result;

    return result;
}

tuple<string, token_type> Lexer::get_current()
{
    return current_token; // current_token;
}

void Lexer::set_echo(bool echo)
{
    this->echo = echo;
}
