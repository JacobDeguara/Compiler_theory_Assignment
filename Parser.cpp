#ifndef __PARSER_H__
#define __PARSER_H__

#include <tuple>
#include <vector>
#include <memory>

#include "Lexer.cpp"
#include "enums.h"

class Parser
{
private:
    Lexer lexer = Lexer();

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
    // Pixel_Statement ::= '__pixelr' <Expr> ',' <Expr> ',' <Expr> ',' <Expr> ',' <Expr> | '__pixel' <Expr> ',' <Expr> ',' <Expr>
    tuple<AST_token, shared_ptr<ASTree>> Pixel_Statement();
    // Print_Statement ::= '__print' <Expr>
    tuple<AST_token, shared_ptr<ASTree>> Print_Statement();
    // Delay_Statement ::= '__delay' <Expr>
    tuple<AST_token, shared_ptr<ASTree>> Delay_Statement();
    // Variable_Decl ::= 'let' <Identifier> ':' <Type> '=' <Expr>
    tuple<AST_token, shared_ptr<ASTree>> Variable_Decl();
    // Assigment ::= <Identifier> '=' <Expr>
    tuple<AST_token, shared_ptr<ASTree>> Assigment(tuple<std::string, token_type> token);
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
    tuple<AST_token, shared_ptr<ASTree>> Function_Call(tuple<string, token_type> previous_token);
    // Sub_Expr ::= '(' <Expr> ')'
    tuple<AST_token, shared_ptr<ASTree>> Sub_Expr();
    // Unary ::= ( '-' | '+' | 'not' | '!' ) <Expr>
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

public:
    Parser(string file_name);

    // Creates a new node
    shared_ptr<ASTree> ASTree_node_create(AST_token token);

    bool Compile();
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
    return temp;
}

bool Parser::Compile()
{
    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Program();

    if (result == SUCCESS)
    {
        cout << "SUCCESS" << endl;
        return true;
    }
    else if (result == FAIL)
    {
        cout << "FAIL" << endl;
        return false;
    }
    else
    {
        cout << "ERROR?" << endl;
        return false;
    }
    return false;
}

/*
IMPORTANT RULES;
    RULE 1: ALWAYS END BY using get_next() unless you've already used it (since other functions also end get_next() they count)
    RULE 2: ALWAYS BEFORE entering another function use get_next() (unless you havent used get_current())
    RULE 3: ALWAYS START BY using get_current() at the beginning or after a function

Only function that can break this rule is Program since we are gana start with it.
*/

tuple<AST_token, shared_ptr<ASTree>> Parser::Program()
{
    // Program ::= { <Statement> } 'EOF'

    AST_token result;
    shared_ptr<ASTree> tree;
    auto current_token = lexer.get_next();
    auto current_token_type = get<token_type>(current_token);

    // { <Statment> }
    while (current_token_type != ENDOFFILE)
    {
        tie(result, tree) = Statement();

        if (result == FAIL)
        {
            return {FAIL, NULL};
        }

        current_token = lexer.get_current(); // RULE 3
        current_token_type = get<token_type>(current_token);
    }
    return {SUCCESS, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Statement()
{
    /*Statement ::= <VariableDecl> ';' | <Assignment> ';' |
     *  <PrintStatement> ';'| <DelayStatement> ';' |
     *  <PixelStatement> ';' | <IfStatement> | <ForStatement> |
     *  <WhileStatement> | <RtrnStatement> ';' | <FunctionDecl> |
     *  <Block>
     */
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_type = get<token_type>(current_token);
    auto current_token_string = get<string>(current_token);

    AST_token result;
    shared_ptr<ASTree> tree;

    switch (current_token_type)
    {
    case Declarator: // <VariableDecl> ';'
        tie(result, tree) = Variable_Decl();

        if (result == SUCCESS)
        {
            // ';'
            current_token = lexer.get_current();
            current_token_string = get<string>(current_token);

            if (current_token_string.compare(";") == 0)
            {
                lexer.get_next();
                return {SUCCESS, NULL};
            }
        }

        cerr << "Syntax Error: missing ';' after variable decliration" << endl;
        return {FAIL, NULL};
        break;
    case Identifier: // <Assignment> ';'

        lexer.get_next();
        tie(result, tree) = Assigment(current_token);

        if (result == FAIL)
        {
            return {FAIL, NULL};
        }

        // ';'
        current_token = lexer.get_current();
        current_token_string = get<string>(current_token);

        if (current_token_string.compare(";") == 0)
        {
            lexer.get_next();
            return {SUCCESS, NULL};
        }

        cerr << "Syntax Error: missing ';' after variable decliration" << endl;
        return {FAIL, NULL};
        break;
    case SpecialStatementsOp: // <PrintStatement> ';'| <DelayStatement> ';' | <PixelStatement> ';'

        current_token_string = get<string>(current_token);

        if (current_token_string.compare("__print") == 0) // <PrintStatement> ';'
        {
            tie(result, tree) = Print_Statement();

            if (result == SUCCESS)
            {
                // ';'
                current_token = lexer.get_current();
                current_token_string = get<string>(current_token);

                if (current_token_string.compare(";") == 0)
                {
                    lexer.get_next();
                    return {SUCCESS, NULL};
                }
                break;
            }
            else
            {
                return {FAIL, NULL};
            }
        }
        else if (current_token_string.compare("__delay") == 0) // <DelayStatement> ';'
        {
            tie(result, tree) = Delay_Statement();

            if (result == SUCCESS)
            {
                // ';'
                current_token = lexer.get_current();
                current_token_string = get<string>(current_token);

                if (current_token_string.compare(";") == 0)
                {
                    lexer.get_next();
                    return {SUCCESS, NULL};
                }
                break;
            }
            else
            {
                return {FAIL, NULL};
            }
        }
        else // <PixelStatement> ';'
        {
            tie(result, tree) = Pixel_Statement();

            if (result == SUCCESS)
            {
                // ';'
                current_token = lexer.get_current();
                current_token_string = get<string>(current_token);

                if (current_token_string.compare(";") == 0)
                {
                    lexer.get_next();
                    return {SUCCESS, NULL};
                }
                break;
            }
            else
            {
                return {FAIL, NULL};
            }
        }

        cerr << "Syntax Error: missing ';' after variable decliration" << endl;
        return {FAIL, NULL};
        break;
    case StatementOp: // <IfStatement> | <ForStatement> | <WhileStatement> | <RtrnStatement> ';'

        current_token_string = get<string>(current_token);

        if (current_token_string.compare("if") == 0) // <IfStatement>
        {
            tie(result, tree) = If_Statement();

            if (result == SUCCESS)
            {
                return {SUCCESS, NULL};
            }
        }
        else if (current_token_string.compare("for") == 0) // <ForStatement>
        {
            tie(result, tree) = For_Statement();

            if (result == SUCCESS)
            {
                return {SUCCESS, NULL};
            }
        }
        else if (current_token_string.compare("while") == 0) // <WhileStatement>
        {

            tie(result, tree) = While_Statement();

            if (result == SUCCESS)
            {
                return {SUCCESS, NULL};
            }
        }
        else // <RtrnStatement> ';'
        {
            tie(result, tree) = RTRN_Statement();

            if (result == SUCCESS)
            {
                // ';'
                current_token = lexer.get_current();
                current_token_string = get<string>(current_token);

                if (current_token_string.compare(";") == 0)
                {
                    lexer.get_next();
                    return {SUCCESS, NULL};
                }
                else
                {
                    cerr << "Syntax Error: missing ';' after variable decliration" << endl;
                    return {FAIL, NULL};
                }
                break;
            }
        }
        break;
    case Func: // <FunctionDecl>
        tie(result, tree) = Function_Decl();

        if (result == SUCCESS)
        {
            return {SUCCESS, NULL};
        }
        break;
    case ExceptionCharacter: // <Block>
        tie(result, tree) = Block();

        if (result == SUCCESS)
        {
            return {SUCCESS, NULL};
        }
        break;
    default:
        break;
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Block()
{
    // Block ::= '{' { <Statement> } '}'

    // '{'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("{") != 0)
    {
        cerr << "Syntax Error: missing '{' " << endl;
        return {FAIL, NULL};
    }

    // { <Statement> } '}'
    AST_token result;
    shared_ptr<ASTree> tree;

    auto next_token = lexer.get_next();
    auto next_token_string = get<string>(next_token);

    while (next_token_string.compare("}") != 0)
    {
        tie(result, tree) = Statement();

        if (result == SUCCESS)
        {
        }
        else
        {
            return {FAIL, NULL};
        }

        next_token = lexer.get_current(); // RULE 3
        next_token_string = get<string>(next_token);
    }

    // '}'
    if (next_token_string.compare("}") == 0)
    {
        lexer.get_next();
        return {SUCCESS, NULL};
    }
    cerr << "Syntax Error: missing '}'" << endl;
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Function_Decl()
{
    // Function_Decl ::= 'fun' <Identifier> '(' [ <FormalParams> ] ')' '->' <Type> <Block>

    // 'fun'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_type = get<token_type>(current_token);

    if (current_token_type != Func)
    {
        cerr << "FAILED CODE: Function_Decl() at if(current_token_type != Func)" << endl;
        return {FAIL, NULL};
    }

    // <Identifier>
    auto next_token = lexer.get_next();
    auto next_token_type = get<token_type>(next_token);

    if (next_token_type != Identifier)
    {
        cerr << "Syntax Error: missing Identifier" << endl;
        return {FAIL, NULL};
    }

    // '('
    next_token = lexer.get_next();
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare("(") != 0)
    {
        cerr << "Syntax Error: missing '(' " << endl;
        return {FAIL, NULL};
    }

    // [ <FormalParams> ] | ')'
    AST_token result;
    shared_ptr<ASTree> tree;
    next_token = lexer.get_next();
    next_token_string = get<string>(next_token);

    if (next_token_string.compare(")") != 0)
    {
        tie(result, tree) = Formal_Params();

        if (result == FAIL)
        {
            cerr << "Possible? Syntax Error: missing ')' " << endl;
            return {FAIL, NULL};
        }

        // ')'
        current_token = lexer.get_current();
        auto current_token_string = get<string>(current_token);

        if (current_token_string.compare(")") != 0)
        {
            cerr << "Syntax Error: missing ')' " << endl;
            return {FAIL, NULL};
        }
    }

    // '->'
    next_token = lexer.get_next();
    next_token_string = get<string>(next_token);

    if (next_token_string.compare("->") != 0)
    {
        cerr << "Syntax Error: missing '->' " << endl;
        return {FAIL, NULL};
    }

    // <Type>
    next_token = lexer.get_next();
    next_token_type = get<token_type>(next_token);

    if (next_token_type != Type)
    {
        cerr << "Syntax Error: missing type declaration " << endl;
        return {FAIL, NULL};
    }

    // <Block>
    lexer.get_next(); // RULE 2
    tie(result, tree) = Block();

    if (result == SUCCESS)
    {
        return {SUCCESS, NULL};
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Formal_Params()
{
    // Formal_Params ::= <FormalParam> { ',' <FormalParam> }

    // <FormalParameter>
    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Formal_Parameter();

    if (result == FAIL)
    {
        return {FAIL, NULL};
    }

    // { ','  <FormalParameter>}
    while (1)
    {
        // ','
        auto current_token = lexer.get_current(); // RULE 3
        auto current_token_string = get<string>(current_token);

        if (current_token_string.compare(",") != 0)
        {
            return {SUCCESS, NULL};
        }

        // <FormalParameter>
        lexer.get_next(); // RULE 2
        tie(result, tree) = Formal_Parameter();

        if (result == FAIL)
        {
            return {FAIL, NULL};
        }
    }
    cerr << "CODE FAILED: failed while(1){}" << endl;
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Formal_Parameter()
{
    // Formal_Parameter ::= <Identifier> ':' <Type>

    // <Identifier>
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_type = get<token_type>(current_token);

    if (current_token_type != Identifier)
    {
        cerr << "Syntax Error: missing identifier" << endl;
        return {FAIL, NULL};
    }

    // ':'
    auto next_token = lexer.get_next();
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare(":") != 0)
    {
        cerr << "Syntax Error: missing ':' " << endl;
        return {FAIL, NULL};
    }

    // <Type>
    next_token = lexer.get_next();
    auto next_token_type = get<token_type>(next_token);

    if (next_token_type == Type)
    {
        lexer.get_next(); // RULE 1
        return {SUCCESS, NULL};
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::While_Statement()
{
    // While_Statement ::= 'while' '(' <Expr> ')' <Block>

    // 'while'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("while") != 0)
    {
        return {FAIL, NULL};
    }

    // '('
    auto next_token = lexer.get_next(); // RULE 3
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare("(") != 0)
    {
        cerr << "Syntax Error: missing '(' " << endl;
        return {FAIL, NULL};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == FAIL)
    {
        return {FAIL, NULL};
    }

    // ')'
    current_token = lexer.get_current(); // RULE 3
    current_token_string = get<string>(current_token);

    if (current_token_string.compare(")") != 0)
    {
        cerr << "Syntax Error: missing ')' " << endl;
        return {FAIL, NULL};
    }

    // <Block>
    lexer.get_next(); // RULE 2
    tie(result, tree) = Block();

    if (result == SUCCESS)
    {
        return {SUCCESS, NULL};
    }

    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::For_Statement()
{
    // For_Statement ::= 'for' '(' [ <VariableDecl> ] ';' <Expr> ';' [ <Assignment> ] ')' <Block>

    // 'for'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("for") != 0)
    {
        cerr << "Syntax Error: missing 'for' " << endl;
        return {FAIL, NULL};
    }

    // '('
    auto next_token = lexer.get_next();
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare("(") != 0)
    {
        cerr << "Syntax Error: missing '(' " << endl;
        return {FAIL, NULL};
    }

    // [ <VariableDecl> ]
    AST_token result;
    shared_ptr<ASTree> tree;
    next_token = lexer.get_next();
    next_token_string = get<string>(next_token);

    if (next_token_string.compare(";") != 0)
    {
        tie(result, tree) = Variable_Decl();

        if (result == FAIL)
        {
            return {FAIL, NULL};
        }

        current_token = lexer.get_current(); // RULE 3
        current_token_string = get<string>(current_token);

        if (current_token_string.compare(";") != 0)
        {
            cerr << "Syntax Error: missing ';' " << endl;
            return {FAIL, NULL};
        }
    }

    // <Expr>
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == FAIL)
    {
        return {FAIL, NULL};
    }

    // ';'
    current_token = lexer.get_current(); // RULE 3
    current_token_string = get<string>(current_token);

    if (current_token_string.compare(";") != 0)
    {
        cerr << "Syntax Error: missing ';' " << endl;
        return {FAIL, NULL};
    }

    // <Identifier>
    current_token = lexer.get_next();
    auto current_token_type = get<token_type>(current_token);

    if (current_token_type == Identifier)
    {
        // [ <Assignment> ]
        lexer.get_next(); // RULE 2
        tie(result, tree) = Assigment(current_token);

        if (result == SUCCESS)
        {
        }
    }

    // ')'
    current_token = lexer.get_current(); // RULE 3
    current_token_string = get<string>(current_token);

    if (current_token_string.compare(")") != 0)
    {
        cerr << "Syntax Error: missing ')' " << endl;
        return {FAIL, NULL};
    }

    // <Block>
    lexer.get_next(); // RULE 2
    tie(result, tree) = Block();

    if (result == SUCCESS)
    {
        return {SUCCESS, NULL};
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::If_Statement()
{
    // If_Statement ::= 'if' '(' <Expr> ')' <Block> [ 'else' <Block> ]

    // 'if'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("if") != 0)
    {
        cerr << "Syntax Error: missing 'if' " << endl;
        return {FAIL, NULL};
    }

    // '('
    auto next_token = lexer.get_next();
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare("(") != 0)
    {
        cerr << "Syntax Error: missing '(' " << endl;
        return {FAIL, NULL};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == FAIL)
    {
        return {FAIL, NULL};
    }

    // ')'
    current_token = lexer.get_current(); // RULE 3
    current_token_string = get<string>(current_token);

    if (current_token_string.compare(")") != 0)
    {
        cerr << "Syntax Error: missing ')' " << endl;
        return {FAIL, NULL};
    }

    //<Block>
    lexer.get_next();
    tie(result, tree) = Block();

    if (result == FAIL)
    {
        return {FAIL, NULL};
    }

    // [ 'else' <Block> ]
    next_token = lexer.get_current(); // RULE 3
    next_token_string = get<string>(next_token);

    if (next_token_string.compare("else") != 0)
    {
        return {SUCCESS, NULL};
    }

    lexer.get_next();
    tie(result, tree) = Block();

    if (result == SUCCESS)
    {
        return {SUCCESS, NULL};
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::RTRN_Statement()
{
    // RTRN_Statement ::= 'return' <Expr>

    // 'return'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("return") != 0)
    {
        cerr << "Syntax Error: missing 'return' " << endl;
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

tuple<AST_token, shared_ptr<ASTree>> Parser::Pixel_Statement()
{
    /* Pixel_Statement ::= '__pixelr' <Expr>','<Expr>','<Expr>','<Expr>','<Expr>
     *                   | '__pixel'  <Expr>','<Expr>','<Expr>
     */

    // '__pixelr' | '__pixel'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);
    int count = 0; // amount of looping

    if (current_token_string.compare("__pixel") == 0)
    {
        count = 2;
    }
    else if (current_token_string.compare("__pixelr") == 0)
    {
        count = 4;
    }
    else
    {
        cerr << "Syntax Error: missing '__pixel' or '__pixelr' " << endl;
        return {FAIL, NULL};
    }

    AST_token result;
    shared_ptr<ASTree> tree;

    // {<Expr> ',' } * count
    for (size_t i = 0; i < count; i++)
    {
        // <Expr>
        lexer.get_next(); // RULE 2
        tie(result, tree) = Expr();

        if (result == FAIL)
        {
            return {FAIL, NULL};
        }

        // ','
        auto next_token = lexer.get_current(); // RULE 3
        auto next_token_string = get<string>(next_token);

        if (next_token_string.compare(",") != 0)
        {
            cerr << "Syntax Error: missing ',' " << endl;
            return {FAIL, NULL};
        }
    }

    //<Expr>
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == SUCCESS)
    {
        return {SUCCESS, NULL};
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Print_Statement()
{
    // Print_Statement ::= '__print' <Expr>

    // '__print'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__print") != 0)
    {
        cerr << "Syntax Error: missing '__print' " << endl;
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

tuple<AST_token, shared_ptr<ASTree>> Parser::Delay_Statement()
{
    // Delay_Statement ::= '__delay' <Expr>

    // '__delay'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__delay") != 0)
    {
        cerr << "Syntax Error: missing '__delay' " << endl;
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

tuple<AST_token, shared_ptr<ASTree>> Parser::Variable_Decl()
{
    // Variable_Decl ::= 'let' <Identifier> ':' <Type> '=' <Expr>

    // 'let'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_type = get<token_type>(current_token);

    if (current_token_type != Declarator)
    {
        cerr << "Syntax Error: missing 'let' " << endl;
        return {FAIL, NULL};
    }

    // <Identifier>
    auto next_token = lexer.get_next();
    auto next_token_type = get<token_type>(next_token);

    if (next_token_type != Identifier)
    {
        cerr << "Syntax Error: missing identifier " << endl;
        return {FAIL, NULL};
    }

    // ':'
    next_token = lexer.get_next();
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare(":") != 0)
    {
        cerr << "Syntax Error: missing ':' " << endl;
        return {FAIL, NULL};
    }

    // <Type>
    next_token = lexer.get_next();
    next_token_type = get<token_type>(next_token);

    if (next_token_type != Type)
    {
        cerr << "Syntax Error: missing Type " << endl;
        return {FAIL, NULL};
    }

    // '='
    next_token = lexer.get_next();
    next_token_string = get<string>(next_token);

    if (next_token_string.compare("=") != 0)
    {
        cerr << "Syntax Error: missing '=' " << endl;
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

tuple<AST_token, shared_ptr<ASTree>> Parser::Assigment(tuple<std::string, token_type> token)
{
    // Assigment ::= <Identifier> '=' <Expr>

    //<Identifier>
    /*  <Identifier> was here but it was removed since we already checked for it
     *  in function Satment(). (some changes where done in For_Statment() since it used <Assigment> as well)
     */

    // '='
    auto next_token = lexer.get_current();
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare("=") != 0)
    {
        cerr << "Syntax Error: missing '=' " << endl;
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

    // <SimpleExpr>
    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Simple_Expr();

    if (result == FAIL)
    {
        return {FAIL, NULL};
    }

    // { <RelationalOp> <SimpleExpr> }
    while (true)
    {
        // <RelationalOp>
        auto next_token = lexer.get_current(); // RULE 3
        auto next_token_type = get<token_type>(next_token);

        if (next_token_type != RelationalOp)
        {
            return {SUCCESS, NULL};
        }

        // <SimpleExpr>
        lexer.get_next(); // RULE 2
        tie(result, tree) = Simple_Expr();

        if (result == FAIL)
        {
            return {FAIL, NULL};
        }
    }
    cerr << "CODE FAILED: at while(1){}" << endl;
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Simple_Expr()
{
    // Simple_Expr ::= <Term> { <AdditiveOp> <Term> }

    //<Term>
    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Term();

    if (result == FAIL)
    {
        return {FAIL, NULL};
    }

    // { <AdditiveOp> <Term> }
    while (true)
    {
        // <AdditiveOp>
        auto next_token = lexer.get_current(); // RULE 3
        auto next_token_type = get<token_type>(next_token);

        if (next_token_type != AdditiveOp)
        {
            return {SUCCESS, NULL};
        }

        // <Term>
        lexer.get_next(); // RULE 2
        tie(result, tree) = Term();

        if (result == FAIL)
        {
            return {FAIL, NULL};
        }
    }
    cerr << "CODE FAILED: at while(1){}" << endl;
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Term()
{
    // Term ::= <Factor> { <MultiplicativeOp> <Factor> }

    // <Factor>
    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Factor();

    if (result == FAIL)
    {
        return {FAIL, NULL};
    }

    // { <MultiplicativeOp> <Factor> }
    while (true)
    {
        // <MultiplicativeOp>
        auto next_token = lexer.get_current(); // RULE 3
        auto next_token_type = get<token_type>(next_token);

        if (next_token_type != MultiplicativeOp)
        {
            return {SUCCESS, NULL};
        }

        // <Factor>
        lexer.get_next(); // RULE 2
        tie(result, tree) = Factor();

        if (result == FAIL)
        {
            return {FAIL, NULL};
        }
    }
    cerr << "CODE FAILED: at while(1){}" << endl;
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
    auto current_token_string = get<string>(current_token);

    tuple<std::string, token_type> next_token;
    string next_token_string;

    AST_token result;
    shared_ptr<ASTree> tree;

    switch (current_token_type)
    {
    case AdditiveOp: // ('-' | '+')
    case UnaryOp:    // <Unary>
        tie(result, tree) = Unary();

        if (result == SUCCESS)
        {
            return {SUCCESS, NULL};
        }
        break;

    case PadOp: //<PadRandI> | <PadWidth> | <PadHeight> | <PadRead>

        if (current_token_string.compare("__height") == 0) // <PadHeight>
        {
            tie(result, tree) = Pad_Height();

            if (result == SUCCESS)
            {
                return {SUCCESS, NULL}; // RULE 3
            }
        }
        else if (current_token_string.compare("__width") == 0) // <PadWidth>
        {
            tie(result, tree) = Pad_Width();

            if (result == SUCCESS)
            {
                return {SUCCESS, NULL}; // RULE 3
            }
        }
        else if (current_token_string.compare("__read") == 0) // <PadRead>
        {
            tie(result, tree) = Pad_Read();

            if (result == SUCCESS)
            {
                return {SUCCESS, NULL}; // RULE 3
            }
        }
        else if (current_token_string.compare("__randi") == 0) // <PadRandI>
        {
            tie(result, tree) = Pad_RandI();

            if (result == SUCCESS)
            {
                return {SUCCESS, NULL}; // RULE 3
            }
        }
        break;
    case Identifier: // <Identifier> | <Function_Call>
        // '('
        next_token = lexer.get_next();
        current_token_string = get<string>(next_token);

        if (current_token_string.compare("(") == 0)
        {
            tie(result, tree) = Function_Call(current_token);

            if (result == FAIL)
            {
                return {FAIL, NULL};
            }
        }

        return {SUCCESS, NULL};
        break;

    default: // finally check for <Literal>

        current_token = lexer.get_current();
        current_token_string = get<string>(current_token);

        if (current_token_string.compare("(") == 0)
        {
            tie(result, tree) = Sub_Expr();

            if (result == SUCCESS)
            {
                return {SUCCESS, NULL};
            }
            break;
        }

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

    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_type = get<token_type>(current_token);

    switch (current_token_type)
    {
    case BooleanLiteral:  // <BooleanLiteral>
        lexer.get_next(); // RULE 1
        return {SUCCESS, NULL};
        break;

    case IntegerLiteral:  // <IntegerLiteral>
        lexer.get_next(); // RULE 1
        return {SUCCESS, NULL};
        break;

    case FloatLiteral:    // <FloatLiteral>
        lexer.get_next(); // RULE 1
        return {SUCCESS, NULL};
        break;

    case ColourLiteral:   // <ColourLiteral>
        lexer.get_next(); // RULE 1
        return {SUCCESS, NULL};
        break;

    case PadOp: // <PadWidth> | <PadHeight> | <PadRead>
        AST_token result;
        shared_ptr<ASTree> tree;

        tie(result, tree) = Pad_Height();

        if (result == SUCCESS)
        {
            return {SUCCESS, NULL}; // RULE 3
        }

        tie(result, tree) = Pad_Width();

        if (result == SUCCESS)
        {
            return {SUCCESS, NULL}; // RULE 3
        }

        tie(result, tree) = Pad_Read();

        if (result == SUCCESS)
        {
            return {SUCCESS, NULL}; // RULE 3
        }
        break;
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Function_Call(tuple<string, token_type> previous_token)
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
        cerr << "Syntax Error: missing '(' " << endl;
        return {FAIL, NULL};
    }

    // [ <ActualParams> ] | ')'
    AST_token result;
    shared_ptr<ASTree> tree;
    auto next_token = lexer.get_next();
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare(")") != 0)
    {
        lexer.get_next(); // RULE 2
        tie(result, tree) = Actual_Params();

        if (result == FAIL)
        {
            return {FAIL, NULL};
        }

        // ')'
        current_token = lexer.get_current(); // RULE 3
        current_token_string = get<string>(current_token);

        if (current_token_string.compare(")") != 0)
        {
            cerr << "Syntax Error: missing ')' " << endl;
            return {FAIL, NULL};
        }
    }

    lexer.get_next();
    return {SUCCESS, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Sub_Expr()
{
    // Sub_expr ::= '(' <Expr> ')'

    // '('
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("(") != 0)
    {
        cerr << "Syntax Error: missing '(' " << endl;
        return {FAIL, NULL};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == FAIL)
    {
        return {FAIL, NULL};
    }

    // ')'
    current_token = lexer.get_current(); // RULE 3
    current_token_string = get<string>(current_token);

    if (current_token_string.compare(")") == 0)
    {
        lexer.get_next(); // RULE 1
        return {SUCCESS, NULL};
    }
    cerr << "Syntax Error: missing ')' " << endl;
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Unary()
{
    // Unary ::= ( '-' | 'not' | '!' ) <Expr>

    // '-' | 'not' | '!'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (get<token_type>(current_token) == UnaryOp)
    {
        // <Expr>
        AST_token result;
        shared_ptr<ASTree> tree;
        lexer.get_next(); // RULE 2
        tie(result, tree) = Expr();

        // test result
        if (result == SUCCESS)
        {
            return {SUCCESS, NULL}; // RULE 1
        }
    }
    else if (current_token_string.compare("-") == 0)
    {
        // <Expr>
        AST_token result;
        shared_ptr<ASTree> tree;
        lexer.get_next(); // RULE 2
        tie(result, tree) = Expr();

        // test result
        if (result == SUCCESS)
        {
            return {SUCCESS, NULL}; // RULE 1
        }
    }
    else if (current_token_string.compare("+") == 0)
    {
        // <Expr>
        AST_token result;
        shared_ptr<ASTree> tree;
        lexer.get_next(); // RULE 2
        tie(result, tree) = Expr();

        // test result
        if (result == SUCCESS)
        {
            return {SUCCESS, NULL}; // RULE 1
        }
    }
    cerr << "Syntax Error: Something is missing" << endl;
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_RandI()
{
    // Pad_randI :: = '__randi'

    // '__randi'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__randi") != 0)
    {
        cerr << "Syntax Error: missing '__randi' " << endl;
        return {FAIL, NULL};
    }

    //<Expr>
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

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_Read()
{
    // Pad_read :: = '__read' <Expr> ',' <Expr>

    // '__read'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__read") != 0)
    {
        cerr << "Syntax Error: missing '__read' " << endl;
        return {FAIL, NULL};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == FAIL)
    {
        return {FAIL, NULL};
    }

    // ','
    auto next_token = lexer.get_current(); // RULE 3
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare(",") != 0)
    {
        cerr << "Syntax Error: missing ',' " << endl;
        return {FAIL, NULL};
    }

    // <Expr>
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == SUCCESS)
    {
        return {SUCCESS, NULL}; // RULE 1
    }
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Actual_Params()
{
    // Actual_params ::= <Expr> { ',' <Expr> }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Expr(); // RULE 2

    if (result == FAIL)
    {
        return {FAIL, NULL};
    }

    // { ',' <Expr> }
    while (true)
    {
        // ','
        auto next_token = lexer.get_current(); // RULE 3
        auto next_token_string = get<string>(next_token);

        if (next_token_string.compare(",") != 0)
        {
            return {SUCCESS, NULL};
        }

        // <Expr>
        lexer.get_next(); // RULE 1
        tie(result, tree) = Expr();

        if (result == FAIL)
        {
            return {FAIL, NULL};
        }
    }
    cerr << "CODE FAILED: while(1){}" << endl;
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_Width()
{
    // Pad_width ::= '__width'

    // '__width'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__width") == 0)
    {
        lexer.get_next(); // RULE 1
        return {SUCCESS, NULL};
    }
    cerr << "Syntax Error: missing '__width' " << endl;
    return {FAIL, NULL};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_Height()
{
    // Pad_height ::= '__height'

    // ‘__height’
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__height") == 0)
    {
        lexer.get_next(); // RULE 1
        return {SUCCESS, NULL};
    }
    cerr << "Syntax Error: missing '__height' " << endl;
    return {FAIL, NULL};
}

#endif // __PARSER_H__