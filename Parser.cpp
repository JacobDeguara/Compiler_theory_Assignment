#include <tuple>
#include <vector>
#include <memory>

#include "Lexer.h"
#include "enums.h"

class Parser
{
private:
    // Program ::= { <Statement> }
    tuple<AST_token, shared_ptr<ASTree>> Program();
    // Statement ::= <VariableDecl> ';' | <Assignment> ';' | <PrintStatement> ';'| <DelayStatement> ';' | <PixelStatement> ';' | <IfStatement> | <ForStatement> | <WhileStatement> | <RtrnStatement> ';' | <FunctionDecl> | <Block>
    tuple<AST_token, shared_ptr<ASTree>> Statement();
    // Block ::= '{' { <Statement> } '}'
    tuple<AST_token, shared_ptr<ASTree>> Block();
    // Function_Decl ::= 'fun' <Identifier> '(' [ <FormalParams> ] ')' '->' <Type><Block>
    tuple<AST_token, shared_ptr<ASTree>> Function_Decl();
    // Formal_Params ::= <FormalParam> { ',' <FormalParam> }
    tuple<AST_token, shared_ptr<ASTree>> Formal_Params();
    // Formal_Parameter ::= <Identifier > ':' <Type>
    tuple<AST_token, shared_ptr<ASTree>> Formal_Parameter();
    // While_Statement ::= 'while' '(' <Expr> ')' <Block>
    tuple<AST_token, shared_ptr<ASTree>> While_Statement();
    // For_Statement ::= 'for' '(' [ <VariableDecl> ] ';' <Expr> ';' [ <Assignment> ] ')' <Block>
    tuple<AST_token, shared_ptr<ASTree>> For_Statement();
    // If_Statement ::= 'if' '(' <Expr> ')' <Block> [ 'else' <Block> ]
    tuple<AST_token, shared_ptr<ASTree>> If_Statement();
    // RTRN_Statement ::=  'return' <Expr>
    tuple<AST_token, shared_ptr<ASTree>> RTRN_Statement();
    // Pixel_Statement ::= '__pixelr' <Expr>','<Expr>','<Expr>','<Expr>','<Expr>| '__pixel' <Expr>','<Expr>','<Expr>
    tuple<AST_token, shared_ptr<ASTree>> Pixel_Statement();
    // Print_Statement ::= '__print' <Expr
    tuple<AST_token, shared_ptr<ASTree>> Print_Statement();
    // Delay_Statement ::= '__delay' <Expr>
    tuple<AST_token, shared_ptr<ASTree>> Delay_Statement();
    // Variable_Decl ::= 'let' <Identifier> ':' <Type> '=' <Expr>
    tuple<AST_token, shared_ptr<ASTree>> Variable_Decl();
    // Assigment ::= <Identifier> '=' <Expr>
    tuple<AST_token, shared_ptr<ASTree>> Assigment();
    // Expr ::= <SimpleExpr> { <RelationalOp> <SimpleExpr> }
    tuple<AST_token, shared_ptr<ASTree>> Expr();
    // Simple_Expr ::= <Term> { <AdditiveOp> <Term> }
    tuple<AST_token, shared_ptr<ASTree>> Simple_Expr();
    // Term ::= <Factor> { <MultiplicativeOp> <Factor> }
    tuple<AST_token, shared_ptr<ASTree>> Term();
    // Factor ::= <Literal> | <Identifier> | <FunctionCall> | <SubExpr> | <Unary> | <PadRandI> | <PadWidth> | <PadHeight> | <PadRead>
    tuple<AST_token, shared_ptr<ASTree>> Factor();
    // Literal ::= <BooleanLiteral> | <IntegerLiteral> | <FloatLiteral> | <ColourLiteral> | <PadWidth> | <PadHeight> | <PadRead>
    tuple<AST_token, shared_ptr<ASTree>> Literal();
    // Function_Call ::= <Identifier> '(' [ <ActualParams> ] ')'
    tuple<AST_token, shared_ptr<ASTree>> Function_Call();
    // Sub_Expr ::= '(' <Expr> ')'
    tuple<AST_token, shared_ptr<ASTree>> Sub_Expr();
    // Unary ::= ( '-' | 'not' | '!' ) <Expr>
    tuple<AST_token, shared_ptr<ASTree>> Unary();
    // Pad_RandI :: = '__randi' <Expr>
    tuple<AST_token, shared_ptr<ASTree>> Pad_RandI();
    // Pad_Read :: = '__read' <Expr>','<Expr>
    tuple<AST_token, shared_ptr<ASTree>> Pad_Read();
    // Actual_Params ::= <Expr> { ',' <Expr> }
    tuple<AST_token, shared_ptr<ASTree>> Actual_Params();
    // Pad_Width ::= '__width'
    tuple<AST_token, shared_ptr<ASTree>> Pad_Width();
    // Pad_Height ::= '__height'
    tuple<AST_token, shared_ptr<ASTree>> Pad_Height();

    Lexer lexer = Lexer();

public:
    Parser(string file_name);

    // Creates a new node
    shared_ptr<ASTree> ASTree_node_create(AST_token token);

    ~Parser() = default;
};

Parser::Parser(string file_name)
{
    lexer.set_up_file(file_name);
}

shared_ptr<ASTree> Parser::ASTree_node_create(AST_token token)
{
    auto temp = make_shared<ASTree>();
    temp->token = token;
    temp->text = " ";
}

/*
IMPORTANT RULES;
    RULE 1: ALWAYS END BY using get_next() unless you've already used it (since other functions also end get_next() they count)
    RULE 2: ALWAYS BEFORE entering another function use get_next() (unless you havent used get_current())
    RULE 3: ALWAYS START BY using get_current()

Only function that can break this rule is Program since we are gana start with it.
*/

tuple<AST_token, shared_ptr<ASTree>> Parser::Program()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Statement()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Block()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Function_Decl()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Formal_Params()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Formal_Parameter()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::While_Statement()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::For_Statement()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::If_Statement()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::RTRN_Statement()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pixel_Statement()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Print_Statement()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Delay_Statement()
{
    // Delay_Statement ::= '__delay' <Expr>

    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_type = get<token_type>(current_token);
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Variable_Decl()
{
    // Variable_Decl ::= 'let' <Identifier> ':' <Type> '=' <Expr>

    // 'let'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_type = get<token_type>(current_token);

    if (current_token_type != Declarator)
    {
        return {FAIL, NULL};
    }

    // <Identifier>
    auto next_token = lexer.get_next();
    auto next_token_type = get<token_type>(next_token);

    if (next_token_type != Identifier)
    {
        return {FAIL, NULL};
    }

    // ':'
    next_token = lexer.get_next();
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare(":") != 0)
    {
        return {FAIL, NULL};
    }

    // <Type>
    next_token = lexer.get_next();
    next_token_type = get<token_type>(next_token);

    if (next_token_type != Type)
    {
        return {FAIL, NULL};
    }

    // '='
    next_token = lexer.get_next();
    next_token_string = get<string>(next_token);

    if (next_token_string.compare("=") != 0)
    {
        return {FAIL, NULL};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == SUCCESS)
    {
        return {SUCCESS, NULL};
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Assigment()
{
    // Assigment ::= <Identifier> '=' <Expr>

    //<Identifier>
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_type = get<token_type>(current_token);

    if (current_token_type != Identifier)
    {
        return {FAIL, NULL};
    }

    // '='
    auto next_token = lexer.get_next();
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare("=") != 0)
    {
        return {FAIL, NULL};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == SUCCESS)
    {
        return {SUCCESS, NULL}; // RULE 1
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Expr()
{
    // Expr ::= <SimpleExpr> { <RelationalOp> <SimpleExpr> }

    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Simple_Expr();

    if (result == FAIL)
    {
        return {FAIL, NULL};
    }

    while (true)
    {
        auto next_token = lexer.get_current(); // RULE 3
        auto next_token_type = get<token_type>(next_token);

        if (next_token_type != RelationalOp)
        {
            return {SUCCESS, NULL};
        }

        lexer.get_next(); // RULE 2
        tie(result, tree) = Factor();

        if (result == FAIL)
        {
            return {FAIL, NULL};
        }
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Simple_Expr()
{
    // Simple_Expr ::= <Term> { <AdditiveOp> <Term> }

    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Term();

    if (result == FAIL)
    {
        return {FAIL, NULL};
    }

    while (true)
    {
        auto next_token = lexer.get_current(); // RULE 3
        auto next_token_type = get<token_type>(next_token);

        if (next_token_type != AdditiveOp)
        {
            return {SUCCESS, NULL};
        }

        lexer.get_next(); // RULE 2
        tie(result, tree) = Factor();

        if (result == FAIL)
        {
            return {FAIL, NULL};
        }
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Term()
{
    // Term ::= <Factor> { <MultiplicativeOp> <Factor> }
    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Factor();

    if (result == FAIL)
    {
        return {FAIL, NULL};
    }

    while (true)
    {
        auto next_token = lexer.get_current(); // RULE 3
        auto next_token_type = get<token_type>(next_token);

        if (next_token_type != MultiplicativeOp)
        {
            return {SUCCESS, NULL};
        }

        lexer.get_next(); // RULE 2
        tie(result, tree) = Factor();

        if (result == FAIL)
        {
            return {FAIL, NULL};
        }
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Factor()
{
    /* Factor ::= <Literal> | <Identifier> | <FunctionCall> |
     * <SubExpr> | <Unary> | <PadRandI> | <PadWidth> | <PadHeight> |
     * <PadRead>
     */
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_type = get<token_type>(current_token);

    switch (current_token_type)
    {
    case Identifier: // <Identifier> & <Function_Call>
        auto next_token = lexer.get_next();
        auto next_token_string = get<string>(current_token);

        if (next_token_string.compare("(") == 0)
        {
            AST_token result;
            shared_ptr<ASTree> tree;
            tie(result, tree) = Function_Call();

            if (result == SUCCESS)
            {
                return {SUCCESS, NULL}; // RULE 2
            }
            return {FAIL, NULL};
        }

        return {SUCCESS, NULL}; // RULE 2
        break;
    case ExceptionCharacter: // possible '(' for <SubExpr>
        AST_token result;
        shared_ptr<ASTree> tree;

        tie(result, tree) = Sub_Expr();

        if (result == SUCCESS)
        {
            return {SUCCESS, NULL};
        }
        break;
    case AdditiveOp:
    case UnaryOp: // possible for <Unary>
        AST_token result;
        shared_ptr<ASTree> tree;

        tie(result, tree) = Unary();

        if (result == SUCCESS)
        {
            return {SUCCESS, NULL};
        }
        break;
    case PadOp: // possible for <PadRandI>, <PadWidth>, <PadHeight>, <PadRead>
        AST_token result;
        shared_ptr<ASTree> tree;

        // ignored RULE 2 since Pad_** needs the PadOp string and we arent assumeing it knows

        tie(result, tree) = Pad_Height();

        if (result == SUCCESS)
            return {SUCCESS, NULL}; // RULE 3

        tie(result, tree) = Pad_Width();

        if (result == SUCCESS)
            return {SUCCESS, NULL}; // RULE 3

        tie(result, tree) = Pad_Read();

        if (result == SUCCESS)
            return {SUCCESS, NULL}; // RULE 3

        tie(result, tree) = Pad_RandI();

        if (result == SUCCESS)
            return {SUCCESS, NULL}; // RULE 3

        break;
    default: // finally check for <Literal>
        AST_token result;
        shared_ptr<ASTree> tree;

        tie(result, tree) = Literal();

        if (result == SUCCESS)
            return {SUCCESS, NULL};

        break;
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Literal()
{
    // Litreal ::= <BooleanLiteral> | <IntegerLiteral> | <FloatLiteral> | <ColourLiteral> | <PadWidth> | <PadHeight> | <PadRead>

    // Literals =>
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_type = get<token_type>(current_token);

    switch (current_token_type)
    {
    case BooleanLiteral:
        lexer.get_next(); // RULE 1
        return {SUCCESS, NULL};
        break;
    case IntegerLiteral:
        lexer.get_next(); // RULE 1
        return {SUCCESS, NULL};
        break;
    case FloatLiteral:
        lexer.get_next(); // RULE 1
        return {SUCCESS, NULL};
        break;
    case ColourLiteral:
        lexer.get_next(); // RULE 1
        return {SUCCESS, NULL};
        break;
    case PadOp: // randi is still part of this set but not part of <literal>
        AST_token result;
        shared_ptr<ASTree> tree;

        // ignored RULE 2 since Pad_** needs the PadOp string and we arent assumeing it knows

        tie(result, tree) = Pad_Height();

        if (result == SUCCESS)
            return {SUCCESS, NULL}; // RULE 3

        tie(result, tree) = Pad_Width();

        if (result == SUCCESS)
            return {SUCCESS, NULL}; // RULE 3

        tie(result, tree) = Pad_Read();

        if (result == SUCCESS)
            return {SUCCESS, NULL}; // RULE 3

        break;
    default:
        break;
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Function_Call()
{
    // Function_call ::= <Identifier> '(' [ <ActualParams> ] ')'

    // <Identifier>
    /*  <Identifier> was here but it was removed since we already checked for it
     *  in function Factor().
     */

    // '('
    auto current_token = lexer.get_current();
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("(") != 0)
    {
        cerr << "Syntax Error: Missing open Bracket" << endl;
        return {FAIL, NULL};
    }

    // [ <ActualParams> ]
    AST_token result;
    shared_ptr<ASTree> tree;

    lexer.get_next(); // RULE 2
    tie(result, tree) = Actual_Params();

    if (result == SUCCESS)
    {
        // tree stuff
    }
    // if fail no problem no paramaters

    // ')'
    current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare(")") != 0)
    {
        cerr << "Syntax Error: Missing open Bracket" << endl;
        return {FAIL, NULL};
    }

    lexer.get_next();
    return {SUCCESS, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Sub_Expr()
{
    // Sub_expr ::= '(' <Expr> ')'

    //  get current token
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("(") != 0)
        return {FAIL, NULL};

    AST_token result;
    shared_ptr<ASTree> tree;

    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    // test result
    if (result == FAIL)
    {
        auto token = lexer.get_current();
        cerr << "Syntax Error: (" << get<string>(token) << ")." << endl;
        return {FAIL, NULL};
    }

    current_token = lexer.get_current(); // RULE 3
    current_token_string = get<string>(current_token);

    if (current_token_string.compare(")") != 0)
        return {FAIL, NULL};

    lexer.get_next(); // RULE 1
    return {SUCCESS, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Unary()
{
    // Unary ::= ( '-' | 'not' | '!' ) <Expr>

    //  get current token
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (get<token_type>(current_token) == UnaryOp)
    {
        AST_token result;
        shared_ptr<ASTree> tree;

        lexer.get_next(); // RULE 2
        tie(result, tree) = Expr();

        // test result
        if (result == FAIL)
        {
            auto token = lexer.get_current();
            cerr << "Syntax Error: (" << get<string>(token) << ")." << endl;
            return {FAIL, NULL};
        }

        return {SUCCESS, NULL}; // RULE 1
    }
    else if (current_token_string.compare("-") == 0)
    {
        AST_token result;
        shared_ptr<ASTree> tree;

        lexer.get_next(); // RULE 2
        tie(result, tree) = Expr();

        // test result
        if (result == FAIL)
        {
            auto token = lexer.get_current();
            cerr << "Syntax Error: (" << get<string>(token) << ")." << endl;
            return {FAIL, NULL};
        }

        return {SUCCESS, NULL}; // RULE 1
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_RandI()
{
    // Pad_randI :: = '__randi' <Expr>

    //  get current token
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__randi") == 0)
    {
        AST_token result;
        shared_ptr<ASTree> tree;

        lexer.get_next(); // RULE 2
        tie(result, tree) = Expr();

        // test result
        if (result == FAIL)
        {
            auto token = lexer.get_current();
            cerr << "Syntax Error: (" << get<string>(token) << ")." << endl;
            return {FAIL, NULL};
        }

        return {SUCCESS, NULL}; // RULE 1
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_Read()
{
    // Pad_read :: = '__read' <Expr> ',' <Expr>

    //  get current token
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__read") == 0)
    {
        AST_token result;
        shared_ptr<ASTree> tree;

        lexer.get_next(); // RULE 2
        tie(result, tree) = Expr();

        // test result
        if (result == FAIL)
        {
            auto token = lexer.get_current();
            cerr << "Syntax Error: (" << get<string>(token) << ")." << endl;
            return {FAIL, NULL};
        }

        auto next_token = lexer.get_current(); // RULE 3

        if (get<string>(next_token).compare(",") != 0)
        {
            return {SUCCESS, NULL};
        }

        lexer.get_next(); // RULE 2
        tie(result, tree) = Expr();

        // test result
        if (result == FAIL)
        {
            auto token = lexer.get_current();
            cerr << "Syntax Error: (" << get<string>(token) << ")." << endl;
            return {FAIL, NULL};
        }

        return {SUCCESS, NULL}; // RULE 1
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Actual_Params()
{
    // Actual_params ::= <Expr> { ',' <Expr> }

    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Expr(); // RULE 2

    // test result
    if (result == FAIL)
    {
        auto token = lexer.get_current();
        cerr << "Syntax Error: (" << get<string>(token) << ")." << endl;
        return {FAIL, NULL};
    }

    while (true)
    {
        auto next_token = lexer.get_current(); // RULE 3

        if (get<string>(next_token).compare(",") != 0)
        {
            return {SUCCESS, NULL};
        }

        lexer.get_next(); // RULE 1
        tie(result, tree) = Expr();

        // test result
        if (result == FAIL)
        {
            auto token = lexer.get_current();
            cerr << "Syntax Error: (" << get<string>(token) << ")." << endl;
            return {FAIL, NULL};
        }
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_Width()
{
    // Pad_width ::= '__width'

    //  get current token
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    // ‘__height’
    if (current_token_string.compare("__width") == 0)
    {
        lexer.get_next(); // RULE 1
        return {SUCCESS, NULL};
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_Height()
{
    // Pad_height ::= '__height'

    //  get current token
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    // ‘__height’
    if (current_token_string.compare("__height") == 0)
    {
        lexer.get_next(); // RULE 1
        return {SUCCESS, NULL};
    }
    return {FAIL, NULL};
}
