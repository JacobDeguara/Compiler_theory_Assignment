#include "Parser.h"

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

bool Parser::Compile(bool echo, bool print_tree, bool show_hidden)
{
    lexer.set_echo(echo);

    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Program();
    root = tree;

    if (print_tree)
    {
        AST_XML_gen xml = AST_XML_gen(root);
        xml.gen_XML(!show_hidden);
    }
    return result == SUCCESS;
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
    shared_ptr<ASTree> head_tree = ASTree_node_create(PROGRAM);

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
            return {FAIL, head_tree};
        }

        head_tree->Leaf.push_back(tree);

        current_token = lexer.get_current(); // RULE 3
        current_token_type = get<token_type>(current_token);
    }
    return {SUCCESS, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Statement()
{
    /*Statement ::= <VariableDecl> ';' | <Assignment> ';' |
     *  <PrintStatement> ';'| <DelayStatement> ';' |
     *  <PixelStatement> ';' | <IfStatement> | <ForStatement> |
     *  <WhileStatement> | <RtrnStatement> ';' | <FunctionDecl> |
     *  <Block> | <ExitProgram> ';'| <fill_clear> ';'
     */
    shared_ptr<ASTree> head_tree = ASTree_node_create(STATEMENT);

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
                return {SUCCESS, tree};
            }
        }

        cerr << "Syntax Error: missing ';' after variable decliration" << endl;
        return {FAIL, head_tree};
        break;
    case Identifier: // <Assignment> ';'

        lexer.get_next();
        tie(result, tree) = Assigment(current_token);

        if (result == FAIL)
        {
            return {FAIL, head_tree};
        }

        // ';'
        current_token = lexer.get_current();
        current_token_string = get<string>(current_token);

        if (current_token_string.compare(";") == 0)
        {

            lexer.get_next();
            return {SUCCESS, tree};
        }

        cerr << "Syntax Error: missing ';' after variable decliration" << endl;
        return {FAIL, head_tree};
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
                    return {SUCCESS, tree};
                }
                break;
            }
            else
            {
                return {FAIL, head_tree};
            }
        }
        else if (current_token_string.compare("__fill") == 0 || current_token_string.compare("__clear") == 0)
        {
            tie(result, tree) = Fill_Statement();

            if (result == SUCCESS)
            {
                // ';'
                current_token = lexer.get_current();
                current_token_string = get<string>(current_token);

                if (current_token_string.compare(";") == 0)
                {
                    lexer.get_next();
                    return {SUCCESS, tree};
                }
                break;
            }
            else
            {
                return {FAIL, head_tree};
            }
        }
        else if (current_token_string.compare("__exit") == 0)
        {
            tie(result, tree) = Exit_Statement();

            if (result == SUCCESS)
            {
                // ';'
                current_token = lexer.get_current();
                current_token_string = get<string>(current_token);

                if (current_token_string.compare(";") == 0)
                {
                    lexer.get_next();
                    return {SUCCESS, tree};
                }
                break;
            }
            else
            {
                return {FAIL, head_tree};
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
                    return {SUCCESS, tree};
                }
                break;
            }
            else
            {
                return {FAIL, head_tree};
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
                    return {SUCCESS, tree};
                }
                break;
            }
            else
            {
                return {FAIL, head_tree};
            }
        }

        cerr << "Syntax Error: missing ';' after variable decliration" << endl;
        return {FAIL, head_tree};
        break;
    case StatementOp: // <IfStatement> | <ForStatement> | <WhileStatement> | <RtrnStatement> ';'

        current_token_string = get<string>(current_token);

        if (current_token_string.compare("if") == 0) // <IfStatement>
        {
            tie(result, tree) = If_Statement();

            if (result == SUCCESS)
            {
                return {SUCCESS, tree};
            }
        }
        else if (current_token_string.compare("for") == 0) // <ForStatement>
        {
            tie(result, tree) = For_Statement();

            if (result == SUCCESS)
            {
                return {SUCCESS, tree};
            }
        }
        else if (current_token_string.compare("while") == 0) // <WhileStatement>
        {
            tie(result, tree) = While_Statement();

            if (result == SUCCESS)
            {
                return {SUCCESS, tree};
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
                    return {SUCCESS, tree};
                }
                else
                {
                    cerr << "Syntax Error: missing ';' after variable decliration" << endl;
                    return {FAIL, head_tree};
                }
                break;
            }
        }
        break;
    case Func: // <FunctionDecl>
        tie(result, tree) = Function_Decl();

        if (result == SUCCESS)
        {
            return {SUCCESS, tree};
        }
        break;
    case ExceptionCharacter: // <Block>
        tie(result, tree) = Block();

        if (result == SUCCESS)
        {
            return {SUCCESS, tree};
        }
        break;
    default:
        break;
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Block()
{
    // Block ::= '{' { <Statement> } '}'
    shared_ptr<ASTree> head_tree = ASTree_node_create(BLOCK);

    // '{'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("{") != 0)
    {
        cerr << "Syntax Error: missing '{' " << endl;
        return {FAIL, head_tree};
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
            head_tree->Leaf.push_back(tree);
        }
        else
        {
            return {FAIL, head_tree};
        }

        next_token = lexer.get_current(); // RULE 3
        next_token_string = get<string>(next_token);
    }

    // '}'
    if (next_token_string.compare("}") == 0)
    {
        lexer.get_next();
        return {SUCCESS, head_tree};
    }
    cerr << "Syntax Error: missing '}'" << endl;
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Function_Decl()
{
    // Function_Decl ::= 'fun' <Identifier> '(' [ <FormalParams> ] ')' '->' <Type> <Block>
    shared_ptr<ASTree> head_tree = ASTree_node_create(FUNCTION_DECL);

    // 'fun'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_type = get<token_type>(current_token);

    if (current_token_type != Func)
    {
        cerr << "FAILED CODE: Function_Decl() at if(current_token_type != Func)" << endl;
        return {FAIL, head_tree};
    }

    // <Identifier>
    auto next_token = lexer.get_next();
    auto next_token_type = get<token_type>(next_token);
    auto next_token_string = get<string>(next_token);

    if (next_token_type != Identifier)
    {
        cerr << "Syntax Error: missing Identifier" << endl;
        return {FAIL, head_tree};
    }

    auto Identifier_node = ASTree_node_create(IDENTIFIER);
    Identifier_node->text = next_token_string;
    head_tree->Leaf.push_back(Identifier_node);

    // '('
    next_token = lexer.get_next();
    next_token_string = get<string>(next_token);

    if (next_token_string.compare("(") != 0)
    {
        cerr << "Syntax Error: missing '(' " << endl;
        return {FAIL, head_tree};
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
            return {FAIL, head_tree};
        }

        head_tree->Leaf.push_back(tree);

        // ')'
        current_token = lexer.get_current();
        auto current_token_string = get<string>(current_token);

        if (current_token_string.compare(")") != 0)
        {
            cerr << "Syntax Error: missing ')' " << endl;
            return {FAIL, head_tree};
        }
    }

    // '->'
    next_token = lexer.get_next();
    next_token_string = get<string>(next_token);

    if (next_token_string.compare("->") != 0)
    {
        cerr << "Syntax Error: missing '->' " << endl;
        return {FAIL, head_tree};
    }

    // <Type>
    next_token = lexer.get_next();
    next_token_type = get<token_type>(next_token);
    next_token_string = get<string>(next_token);

    if (next_token_type != Type)
    {
        cerr << "Syntax Error: missing type declaration " << endl;
        return {FAIL, head_tree};
    }

    auto Type_node = ASTree_node_create(TYPE);
    Type_node->text = next_token_string;
    head_tree->Leaf.push_back(Type_node);

    // <Block>
    lexer.get_next(); // RULE 2
    tie(result, tree) = Block();

    if (result == SUCCESS)
    {
        head_tree->Leaf.push_back(tree);
        return {SUCCESS, head_tree};
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Formal_Params()
{
    // Formal_Params ::= <FormalParam> { ',' <FormalParam> }
    shared_ptr<ASTree> head_tree = ASTree_node_create(FORMAL_PARAMS);

    // <FormalParameter>
    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Formal_Parameter();

    if (result == FAIL)
    {
        return {FAIL, head_tree};
    }

    head_tree->Leaf.push_back(tree);

    // { ','  <FormalParameter>}
    while (1)
    {
        // ','
        auto current_token = lexer.get_current(); // RULE 3
        auto current_token_string = get<string>(current_token);

        if (current_token_string.compare(",") != 0)
        {
            return {SUCCESS, head_tree};
        }

        // <FormalParameter>
        lexer.get_next(); // RULE 2
        tie(result, tree) = Formal_Parameter();

        if (result == FAIL)
        {
            return {FAIL, head_tree};
        }
        head_tree->Leaf.push_back(tree);
    }
    cerr << "CODE FAILED: failed while(1){}" << endl;
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Formal_Parameter()
{
    // Formal_Parameter ::= <Identifier> ':' <Type>
    shared_ptr<ASTree> head_tree = ASTree_node_create(FORMAL_PARAMETER);

    // <Identifier>
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_type = get<token_type>(current_token);
    auto current_token_string = get<string>(current_token);

    if (current_token_type != Identifier)
    {
        cerr << "Syntax Error: missing identifier" << endl;
        return {FAIL, head_tree};
    }

    auto Identifier_node = ASTree_node_create(IDENTIFIER);
    Identifier_node->text = current_token_string;
    head_tree->Leaf.push_back(Identifier_node);

    // ':'
    auto next_token = lexer.get_next();
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare(":") != 0)
    {
        cerr << "Syntax Error: missing ':' " << endl;
        return {FAIL, head_tree};
    }

    // <Type>
    next_token = lexer.get_next();
    auto next_token_type = get<token_type>(next_token);
    next_token_string = get<string>(next_token);

    if (next_token_type == Type)
    {
        auto Type_node = ASTree_node_create(TYPE);
        Type_node->text = next_token_string;
        head_tree->Leaf.push_back(Type_node);

        lexer.get_next(); // RULE 1
        return {SUCCESS, head_tree};
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::While_Statement()
{
    // While_Statement ::= 'while' '(' <Expr> ')' <Block>
    shared_ptr<ASTree> head_tree = ASTree_node_create(WHILE_STATEMENT);

    // 'while'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("while") != 0)
    {
        return {FAIL, head_tree};
    }

    // '('
    auto next_token = lexer.get_next(); // RULE 3
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare("(") != 0)
    {
        cerr << "Syntax Error: missing '(' " << endl;
        return {FAIL, head_tree};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == FAIL)
    {
        return {FAIL, head_tree};
    }
    head_tree->Leaf.push_back(tree);

    // ')'
    current_token = lexer.get_current(); // RULE 3
    current_token_string = get<string>(current_token);

    if (current_token_string.compare(")") != 0)
    {
        cerr << "Syntax Error: missing ')' " << endl;
        return {FAIL, head_tree};
    }

    // <Block>
    lexer.get_next(); // RULE 2
    tie(result, tree) = Block();

    if (result == SUCCESS)
    {
        head_tree->Leaf.push_back(tree);
        return {SUCCESS, head_tree};
    }

    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::For_Statement()
{
    // For_Statement ::= 'for' '(' [ <VariableDecl> ] ';' <Expr> ';' [ <Assignment> ] ')' <Block>
    shared_ptr<ASTree> head_tree = ASTree_node_create(FOR_STATEMENT);

    // 'for'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("for") != 0)
    {
        cerr << "Syntax Error: missing 'for' " << endl;
        return {FAIL, head_tree};
    }

    // '('
    auto next_token = lexer.get_next();
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare("(") != 0)
    {
        cerr << "Syntax Error: missing '(' " << endl;
        return {FAIL, head_tree};
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
            return {FAIL, head_tree};
        }
        head_tree->Leaf.push_back(tree);

        current_token = lexer.get_current(); // RULE 3
        current_token_string = get<string>(current_token);

        if (current_token_string.compare(";") != 0)
        {
            cerr << "Syntax Error: missing ';' " << endl;
            return {FAIL, head_tree};
        }
    }

    // <Expr>
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == FAIL)
    {
        return {FAIL, head_tree};
    }
    head_tree->Leaf.push_back(tree);

    // ';'
    current_token = lexer.get_current(); // RULE 3
    current_token_string = get<string>(current_token);

    if (current_token_string.compare(";") != 0)
    {
        cerr << "Syntax Error: missing ';' " << endl;
        return {FAIL, head_tree};
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
            head_tree->Leaf.push_back(tree);
        }
    }

    // ')'
    current_token = lexer.get_current(); // RULE 3
    current_token_string = get<string>(current_token);

    if (current_token_string.compare(")") != 0)
    {
        cerr << "Syntax Error: missing ')' " << endl;
        return {FAIL, head_tree};
    }

    // <Block>
    lexer.get_next(); // RULE 2
    tie(result, tree) = Block();

    if (result == SUCCESS)
    {
        head_tree->Leaf.push_back(tree);
        return {SUCCESS, head_tree};
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::If_Statement()
{
    // If_Statement ::= 'if' '(' <Expr> ')' <Block> [ 'else' <Block> ]
    shared_ptr<ASTree> head_tree = ASTree_node_create(IF_STATEMENT);

    // 'if'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("if") != 0)
    {
        cerr << "Syntax Error: missing 'if' " << endl;
        return {FAIL, head_tree};
    }

    // '('
    auto next_token = lexer.get_next();
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare("(") != 0)
    {
        cerr << "Syntax Error: missing '(' " << endl;
        return {FAIL, head_tree};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == FAIL)
    {
        return {FAIL, head_tree};
    }
    head_tree->Leaf.push_back(tree);

    // ')'
    current_token = lexer.get_current(); // RULE 3
    current_token_string = get<string>(current_token);

    if (current_token_string.compare(")") != 0)
    {
        cerr << "Syntax Error: missing ')' " << endl;
        return {FAIL, head_tree};
    }

    //<Block>
    lexer.get_next();
    tie(result, tree) = Block();

    if (result == FAIL)
    {
        return {FAIL, head_tree};
    }
    head_tree->Leaf.push_back(tree);

    // [ 'else' <Block> ]
    next_token = lexer.get_current(); // RULE 3
    next_token_string = get<string>(next_token);

    if (next_token_string.compare("else") != 0)
    {
        return {SUCCESS, head_tree};
    }

    lexer.get_next();
    tie(result, tree) = Block();

    if (result == SUCCESS)
    {
        head_tree->Leaf.push_back(tree);
        return {SUCCESS, head_tree};
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::RTRN_Statement()
{
    // RTRN_Statement ::= 'return' <Expr>
    shared_ptr<ASTree> head_tree = ASTree_node_create(RTRN_STATEMENT);

    // 'return'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("return") != 0)
    {
        cerr << "Syntax Error: missing 'return' " << endl;
        return {FAIL, head_tree};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == SUCCESS)
    {
        head_tree->Leaf.push_back(tree);
        return {SUCCESS, head_tree};
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pixel_Statement()
{
    /* Pixel_Statement ::= '__pixelr' <Expr>','<Expr>','<Expr>','<Expr>','<Expr>
     *                   | '__pixel'  <Expr>','<Expr>','<Expr>
     */
    shared_ptr<ASTree> head_tree = ASTree_node_create(PIXEL_STATEMENT);

    // '__pixelr' | '__pixel'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);
    int count = 0; // amount of looping

    head_tree->text = current_token_string;

    if (current_token_string.compare("__pixel") == 0)
    {
        count = 2;
    }
    else if (current_token_string.compare("__pixelr") == 0)
    {
        head_tree = ASTree_node_create(PIXELR_STATEMENT);
        count = 4;
    }
    else
    {
        cerr << "Syntax Error: missing '__pixel' or '__pixelr' " << endl;
        return {FAIL, head_tree};
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
            return {FAIL, head_tree};
        }
        head_tree->Leaf.push_back(tree);

        // ','
        auto next_token = lexer.get_current(); // RULE 3
        auto next_token_string = get<string>(next_token);

        if (next_token_string.compare(",") != 0)
        {
            cerr << "Syntax Error: missing ',' " << endl;
            return {FAIL, head_tree};
        }
    }

    //<Expr>
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == SUCCESS)
    {
        head_tree->Leaf.push_back(tree);
        return {SUCCESS, head_tree};
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Print_Statement()
{
    // Print_Statement ::= '__print' <Expr>
    shared_ptr<ASTree> head_tree = ASTree_node_create(PRINT_STATEMENT);

    // '__print'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__print") != 0)
    {
        cerr << "Syntax Error: missing '__print' " << endl;
        return {FAIL, head_tree};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == SUCCESS)
    {
        head_tree->Leaf.push_back(tree);
        return {SUCCESS, head_tree};
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Delay_Statement()
{
    // Delay_Statement ::= '__delay' <Expr>
    shared_ptr<ASTree> head_tree = ASTree_node_create(DELAY_STATEMENT);

    // '__delay'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__delay") != 0)
    {
        cerr << "Syntax Error: missing '__delay' " << endl;
        return {FAIL, head_tree};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == SUCCESS)
    {
        head_tree->Leaf.push_back(tree);
        return {SUCCESS, head_tree};
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Fill_Statement()
{
    // Fill_Statement ::= '__fill' <Expr> | '__clear'
    shared_ptr<ASTree> head_tree = ASTree_node_create(FILL_CLEAR);

    // '__fill' | '__clear'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__clear") == 0)
    {
        lexer.get_next();
        return {SUCCESS, head_tree};
    }
    else if (current_token_string.compare("__fill") != 0)
    {
        cerr << "Syntax Error: missing '__fill' | __clear " << endl;
        return {FAIL, head_tree};
    }

    head_tree->text = current_token_string;

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == SUCCESS)
    {
        head_tree->Leaf.push_back(tree);
        return {SUCCESS, head_tree};
    }
    return {FAIL, head_tree};
}
tuple<AST_token, shared_ptr<ASTree>> Parser::Exit_Statement()
{
    // Exit_statment ::= '__exit'
    shared_ptr<ASTree> head_tree = ASTree_node_create(EXIT_PROGRAM);

    // '__exit'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__exit") == 0)
    {
        lexer.get_next();
        return {SUCCESS, head_tree};
    }

    cerr << "Syntax Error: missing '__exit' " << endl;
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Variable_Decl()
{
    // Variable_Decl ::= 'let' <Identifier> ':' <Type> '=' <Expr>
    shared_ptr<ASTree> head_tree = ASTree_node_create(VARIABLE_DECL);

    // 'let'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_type = get<token_type>(current_token);

    if (current_token_type != Declarator)
    {
        cerr << "Syntax Error: missing 'let' " << endl;
        return {FAIL, head_tree};
    }

    // <Identifier>
    auto next_token = lexer.get_next();
    auto next_token_type = get<token_type>(next_token);
    auto next_token_string = get<string>(next_token);

    if (next_token_type != Identifier)
    {
        cerr << "Syntax Error: missing identifier " << endl;
        return {FAIL, head_tree};
    }

    auto Identifier_node = ASTree_node_create(IDENTIFIER);
    Identifier_node->text = next_token_string;
    head_tree->Leaf.push_back(Identifier_node);

    // ':'
    next_token = lexer.get_next();
    next_token_string = get<string>(next_token);

    if (next_token_string.compare(":") != 0)
    {
        cerr << "Syntax Error: missing ':' " << endl;
        return {FAIL, head_tree};
    }

    // <Type>
    next_token = lexer.get_next();
    next_token_type = get<token_type>(next_token);
    next_token_string = get<string>(next_token);

    if (next_token_type != Type)
    {
        cerr << "Syntax Error: missing Type " << endl;
        return {FAIL, head_tree};
    }

    auto Type_node = ASTree_node_create(TYPE);
    Type_node->text = next_token_string;
    head_tree->Leaf.push_back(Type_node);

    // '='
    next_token = lexer.get_next();
    next_token_string = get<string>(next_token);

    if (next_token_string.compare("=") != 0)
    {
        cerr << "Syntax Error: missing '=' " << endl;
        return {FAIL, head_tree};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == SUCCESS)
    {
        head_tree->Leaf.push_back(tree);
        return {SUCCESS, head_tree};
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Assigment(tuple<std::string, token_type> previous_token)
{
    // Assigment ::= <Identifier> '=' <Expr>
    shared_ptr<ASTree> head_tree = ASTree_node_create(ASSIGNMENT);

    //<Identifier>
    /*  <Identifier> was here but it was removed since we already checked for it
     *  in function Satment(). (some changes where done in For_Statment() since it used <Assigment> as well)
     */

    auto previous_token_string = get<string>(previous_token);
    auto identifier_node = ASTree_node_create(IDENTIFIER);
    identifier_node->text = previous_token_string;

    head_tree->Leaf.push_back(identifier_node);

    // '='
    auto next_token = lexer.get_current();
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare("=") != 0)
    {
        cerr << "Syntax Error: missing '=' " << endl;
        return {FAIL, head_tree};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == SUCCESS)
    {
        head_tree->Leaf.push_back(tree);
        return {SUCCESS, head_tree}; // RULE 1
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Expr()
{
    // Expr ::= <SimpleExpr> { <RelationalOp> <SimpleExpr> }
    shared_ptr<ASTree> head_tree = ASTree_node_create(EXPR);
    auto next_node = head_tree;
    int i = 0;

    // <SimpleExpr>
    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Simple_Expr();

    if (result == FAIL)
    {
        return {FAIL, head_tree};
    }

    head_tree->Leaf.push_back(tree);

    // { <RelationalOp> <SimpleExpr> }
    while (true)
    {
        // <RelationalOp>
        auto next_token = lexer.get_current(); // RULE 3
        auto next_token_type = get<token_type>(next_token);
        auto next_token_string = get<string>(next_token);

        if (next_token_type != RelationalOp)
        {
            return {SUCCESS, head_tree};
        }

        // empty the next_node
        next_node->Leaf.pop_back();

        // create MultiOp_node
        auto RelatOp_node = ASTree_node_create(RELATIONALOP);
        RelatOp_node->text = next_token_string;

        // set RelatOp_node as next_node leaf
        next_node->Leaf.push_back(RelatOp_node);

        // next_node = RelatOp_node
        next_node = next_node->Leaf.at(i);

        // we set <Factor> as 1. Leaf to RelatOp_node
        next_node->Leaf.push_back(tree);

        // <SimpleExpr>
        lexer.get_next(); // RULE 2
        tie(result, tree) = Simple_Expr();

        if (result == FAIL)
        {
            return {FAIL, head_tree};
        }
        next_node->Leaf.push_back(tree);
        i = 1;
    }
    cerr << "CODE FAILED: at while(1){}" << endl;
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Simple_Expr()
{
    // Simple_Expr ::= <Term> { <AdditiveOp> <Term> }
    shared_ptr<ASTree> head_tree = ASTree_node_create(SIMPLE_EXPR);
    auto next_node = head_tree;
    int i = 0;

    //<Term>
    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Term();

    if (result == FAIL)
    {
        return {FAIL, head_tree};
    }

    head_tree->Leaf.push_back(tree);

    // { <AdditiveOp> <Term> }
    while (true)
    {
        // <AdditiveOp>
        auto next_token = lexer.get_current(); // RULE 3
        auto next_token_type = get<token_type>(next_token);
        auto next_token_string = get<string>(next_token);

        if (next_token_type != AdditiveOp)
        {
            // head_tree->Leaf.at(0) to remove the Simple_Expr node part
            return {SUCCESS, head_tree->Leaf.at(0)};
        }

        // empty the last node in next_node
        next_node->Leaf.pop_back();

        // create AdditOp_node
        auto AdditOp_node = ASTree_node_create(ADDITIVEOP);
        AdditOp_node->text = next_token_string;

        // set AdditOp_node as next_node leaf
        next_node->Leaf.push_back(AdditOp_node);

        // next_node = AdditOp_node
        next_node = next_node->Leaf.at(i);

        // we set <Factor> as 1. Leaf to AdditOp_node
        next_node->Leaf.push_back(tree);

        // <Term>
        lexer.get_next(); // RULE 2
        tie(result, tree) = Term();

        if (result == FAIL)
        {
            return {FAIL, head_tree};
        }
        next_node->Leaf.push_back(tree);
        i = 1;
    }
    cerr << "CODE FAILED: at while(1){}" << endl;
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Term()
{
    // Term ::= <Factor> { <MultiplicativeOp> <Factor> }
    shared_ptr<ASTree> head_tree = ASTree_node_create(TERM);
    auto next_node = head_tree;
    int i = 0;

    // <Factor>
    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Factor();

    if (result == FAIL)
    {
        return {FAIL, head_tree};
    }

    // ste head_tree leaf as <Factor>
    head_tree->Leaf.push_back(tree);

    // { <MultiplicativeOp> <Factor> }
    while (true)
    {
        // <MultiplicativeOp>
        auto next_token = lexer.get_current(); // RULE 3
        auto next_token_type = get<token_type>(next_token);
        auto next_token_string = get<string>(next_token);

        if (next_token_type != MultiplicativeOp)
        {
            // head_tree->Leaf.at(0) to remove the Simple_Expr node part
            return {SUCCESS, head_tree->Leaf.at(0)};
        }

        // empty the next_node
        next_node->Leaf.pop_back();

        // create MultiOp_node
        auto MultiOp_node = ASTree_node_create(MULTIPLICATIVEOP);
        MultiOp_node->text = next_token_string;

        // set MulitOp_node as next_node leaf
        next_node->Leaf.push_back(MultiOp_node);

        // next_node = MulitOp_node
        next_node = next_node->Leaf.at(i);

        // we set <Factor> as 1. Leaf to MultiOp_node
        next_node->Leaf.push_back(tree);

        // <Factor>
        lexer.get_next(); // RULE 2
        tie(result, tree) = Factor();

        if (result == FAIL)
        {
            return {FAIL, head_tree};
        }

        // set next <Factor> as 2. leaf
        next_node->Leaf.push_back(tree);
        i = 1;
    }
    cerr << "CODE FAILED: at while(1){}" << endl;
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Factor()
{
    /* Factor ::= <Literal> | <Identifier> | <FunctionCall> |
     * <SubExpr> | <Unary> | <PadRandI> | <PadWidth> | <PadHeight> |
     * <PadRead> | <Min_Max>
     */
    shared_ptr<ASTree> head_tree = ASTree_node_create(FACTOR);

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
            return {SUCCESS, tree};
        }
        break;

    case PadOp: //<PadRandI> | <PadWidth> | <PadHeight> | <PadRead>

        if (current_token_string.compare("__height") == 0) // <PadHeight>
        {
            tie(result, tree) = Pad_Height();

            if (result == SUCCESS)
            {
                return {SUCCESS, tree}; // RULE 3
            }
        }
        else if (current_token_string.compare("__width") == 0) // <PadWidth>
        {
            tie(result, tree) = Pad_Width();

            if (result == SUCCESS)
            {
                return {SUCCESS, tree}; // RULE 3
            }
        }
        else if (current_token_string.compare("__read") == 0) // <PadRead>
        {
            tie(result, tree) = Pad_Read();

            if (result == SUCCESS)
            {
                return {SUCCESS, tree}; // RULE 3
            }
        }
        else if (current_token_string.compare("__randi") == 0) // <PadRandI>
        {
            tie(result, tree) = Pad_RandI();

            if (result == SUCCESS)
            {
                return {SUCCESS, tree}; // RULE 3
            }
        }
        else if (current_token_string.compare("__min") == 0 || current_token_string.compare("__max") == 0)
        {
            tie(result, tree) = Pad_Min_Max();

            if (result == SUCCESS)
            {
                return {SUCCESS, tree}; // RULE 3
            }
        }
        break;
    case Identifier: // <Identifier> | <Function_Call>
        // '('
        next_token = lexer.get_next();
        next_token_string = get<string>(next_token);

        if (next_token_string.compare("(") == 0)
        {
            tie(result, tree) = Function_Call(current_token);

            if (result == FAIL)
            {
                return {FAIL, head_tree};
            }
        }
        else
        {
            auto Identifier_Node = ASTree_node_create(IDENTIFIER);
            Identifier_Node->text = current_token_string;
            tree = Identifier_Node;
        }

        return {SUCCESS, tree};
        break;
    case ExceptionCharacter:
        current_token = lexer.get_current();
        current_token_string = get<string>(current_token);

        if (current_token_string.compare("(") == 0)
        {
            tie(result, tree) = Sub_Expr();

            if (result == SUCCESS)
            {
                return {SUCCESS, tree};
            }
            break;
        }
        break;

    default: // finally check for <Literal>

        tie(result, tree) = Literal();

        if (result == SUCCESS)
        {
            // ++ / -- operator can be added here but it will only work on <Literals>
            return {SUCCESS, tree};
        }
        break;
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Literal()
{
    // Litreal ::= <BooleanLiteral> | <IntegerLiteral> | <FloatLiteral> | <ColourLiteral> | <PadWidth> | <PadHeight> | <PadRead>
    shared_ptr<ASTree> head_tree = ASTree_node_create(LITERAL);
    shared_ptr<ASTree> tree;

    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);
    auto current_token_type = get<token_type>(current_token);

    switch (current_token_type)
    {
    case BooleanLiteral: // <BooleanLiteral>
        tree = ASTree_node_create(BOOL_LITERAL);
        tree->text = current_token_string;

        lexer.get_next(); // RULE 1
        return {SUCCESS, tree};
        break;

    case IntegerLiteral: // <IntegerLiteral>
        tree = ASTree_node_create(INTEGER_LITERAL);
        tree->text = current_token_string;

        lexer.get_next(); // RULE 1
        return {SUCCESS, tree};
        break;

    case FloatLiteral: // <FloatLiteral>
        tree = ASTree_node_create(FLOAT_LITERAL);
        tree->text = current_token_string;

        lexer.get_next(); // RULE 1
        return {SUCCESS, tree};
        break;

    case ColourLiteral: // <ColourLiteral>
        tree = ASTree_node_create(COLOUR_LITERAL);
        tree->text = current_token_string;

        lexer.get_next(); // RULE 1
        return {SUCCESS, tree};
        break;

    case PadOp: // <PadWidth> | <PadHeight> | <PadRead>
        AST_token result;

        tie(result, tree) = Pad_Height();

        if (result == SUCCESS)
        {
            return {SUCCESS, tree}; // RULE 3
        }

        tie(result, tree) = Pad_Width();

        if (result == SUCCESS)
        {
            return {SUCCESS, tree}; // RULE 3
        }

        tie(result, tree) = Pad_Read();

        if (result == SUCCESS)
        {
            return {SUCCESS, tree}; // RULE 3
        }
        break;
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Function_Call(tuple<string, token_type> previous_token)
{
    // Function_call ::= <Identifier> '(' [ <ActualParams> ] ')'
    shared_ptr<ASTree> head_tree = ASTree_node_create(FUNCTION_CALL);

    // <Identifier>
    /*  <Identifier> was here but it was removed since we already checked for it
     *  in function Factor().
     */

    auto previous_token_string = get<string>(previous_token);
    auto identifier_node = ASTree_node_create(IDENTIFIER);
    identifier_node->text = previous_token_string;

    head_tree->Leaf.push_back(identifier_node);

    // '('
    auto current_token = lexer.get_current();
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("(") != 0)
    {
        cerr << "Syntax Error: missing '(' " << endl;
        return {FAIL, head_tree};
    }

    // [ <ActualParams> ] | ')'
    AST_token result;
    shared_ptr<ASTree> tree;
    auto next_token = lexer.get_next();
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare(")") != 0) // ')'
    {
        // <ActualParams>
        tie(result, tree) = Actual_Params();

        if (result == FAIL)
        {
            return {FAIL, head_tree};
        }

        head_tree->Leaf.push_back(tree);

        // ')'
        current_token = lexer.get_current(); // RULE 3
        current_token_string = get<string>(current_token);

        if (current_token_string.compare(")") != 0)
        {
            cerr << "Syntax Error: missing ')' " << endl;
            return {FAIL, head_tree};
        }
    }

    lexer.get_next();
    return {SUCCESS, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Sub_Expr()
{
    // Sub_expr ::= '(' <Expr> ')'
    shared_ptr<ASTree> head_tree = ASTree_node_create(SUB_EXPR);

    // '('
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("(") != 0)
    {
        cerr << "Syntax Error: missing '(' " << endl;
        return {FAIL, head_tree};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == FAIL)
    {
        return {FAIL, head_tree};
    }

    // ')'
    current_token = lexer.get_current(); // RULE 3
    current_token_string = get<string>(current_token);

    if (current_token_string.compare(")") == 0)
    {
        lexer.get_next(); // RULE 1
        return {SUCCESS, tree->Leaf.at(0)};
    }
    cerr << "Syntax Error: missing ')' " << endl;
    return {FAIL, tree};

    // possible code parsing reduction by removing <SUB_EXPR> and just returning <EXPR>
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Unary()
{
    // Unary ::= ( '-' | '+' | 'not' | '!' ) <Expr>
    shared_ptr<ASTree> head_tree = ASTree_node_create(UNARY);

    // '-' | '+' | 'not' | '!'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (get<token_type>(current_token) == UnaryOp) // 'not' | '!'
    {
        head_tree->text = current_token_string;

        // <Expr>
        AST_token result;
        shared_ptr<ASTree> tree;
        lexer.get_next(); // RULE 2
        tie(result, tree) = Expr();

        // test result
        if (result == SUCCESS)
        {
            head_tree->Leaf.push_back(tree);
            return {SUCCESS, head_tree}; // RULE 1
        }
    }
    else if (current_token_string.compare("-") == 0) // '-'
    {
        head_tree->text = current_token_string;

        // <Expr>
        AST_token result;
        shared_ptr<ASTree> tree;
        lexer.get_next(); // RULE 2
        tie(result, tree) = Expr();

        // test result
        if (result == SUCCESS)
        {
            head_tree->Leaf.push_back(tree);
            return {SUCCESS, head_tree}; // RULE 1
        }
    }
    else if (current_token_string.compare("+") == 0) // '+'
    {
        // <Expr>
        AST_token result;
        shared_ptr<ASTree> tree;
        lexer.get_next(); // RULE 2
        tie(result, tree) = Expr();

        // test result
        if (result == SUCCESS)
        {
            return {SUCCESS, tree}; // RULE 1
            // this returns the tree since; '+' <Expr> ::= <Expr>
        }
    }
    cerr << "Syntax Error: Something is missing" << endl;
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_RandI()
{
    // Pad_randI :: = '__randi'
    shared_ptr<ASTree> head_tree = ASTree_node_create(PAD_RANDI);

    // '__randi'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__randi") != 0)
    {
        cerr << "Syntax Error: missing '__randi' " << endl;
        return {FAIL, head_tree};
    }

    head_tree->text = current_token_string;

    //<Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == SUCCESS)
    {
        head_tree->Leaf.push_back(tree);
        return {SUCCESS, head_tree}; // RULE 1
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_Read()
{
    // Pad_read :: = '__read' <Expr> ',' <Expr>
    shared_ptr<ASTree> head_tree = ASTree_node_create(PAD_READ);

    // '__read'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__read") != 0)
    {
        cerr << "Syntax Error: missing '__read' " << endl;
        return {FAIL, head_tree};
    }

    head_tree->text = current_token_string;

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == FAIL)
    {
        return {FAIL, head_tree};
    }

    head_tree->Leaf.push_back(tree);

    // ','
    auto next_token = lexer.get_current(); // RULE 3
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare(",") != 0)
    {
        cerr << "Syntax Error: missing ',' " << endl;
        return {FAIL, head_tree};
    }

    // <Expr>
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == SUCCESS)
    {
        head_tree->Leaf.push_back(tree);
        return {SUCCESS, head_tree}; // RULE 1
    }
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Actual_Params()
{
    // Actual_params ::= <Expr> { ',' <Expr> }
    shared_ptr<ASTree> head_tree = ASTree_node_create(ACTUAL_PARAMS);

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    tie(result, tree) = Expr(); // RULE 2

    if (result == FAIL)
    {
        return {FAIL, head_tree};
    }

    head_tree->Leaf.push_back(tree);

    // { ',' <Expr> }
    while (true)
    {
        // ','
        auto next_token = lexer.get_current(); // RULE 3
        auto next_token_string = get<string>(next_token);

        if (next_token_string.compare(",") != 0)
        {
            return {SUCCESS, head_tree};
        }

        // <Expr>
        lexer.get_next(); // RULE 1
        tie(result, tree) = Expr();

        if (result == FAIL)
        {
            return {FAIL, head_tree};
        }

        head_tree->Leaf.push_back(tree);
    }
    cerr << "CODE FAILED: while(1){}" << endl;
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_Width()
{
    // Pad_width ::= '__width'
    shared_ptr<ASTree> head_tree = ASTree_node_create(PAD_WIDTH);

    // '__width'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__width") == 0)
    {
        head_tree->text = current_token_string;

        lexer.get_next(); // RULE 1
        return {SUCCESS, head_tree};
    }
    cerr << "Syntax Error: missing '__width' " << endl;
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_Height()
{
    // Pad_height ::= '__height'
    shared_ptr<ASTree> head_tree = ASTree_node_create(PAD_HEIGHT);

    // ‘__height’
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__height") == 0)
    {
        head_tree->text = current_token_string;

        lexer.get_next(); // RULE 1
        return {SUCCESS, head_tree};
    }
    cerr << "Syntax Error: missing '__height' " << endl;
    return {FAIL, head_tree};
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_Min_Max()
{
    // Pad_Min_Max ::= '__max' <Expr>','<Expr> , '__min' <Expr>','<Expr>
    shared_ptr<ASTree> head_tree = ASTree_node_create(MAX_MIN);

    // '__min' | '__max'
    auto current_token = lexer.get_current(); // RULE 3
    auto current_token_string = get<string>(current_token);

    if (current_token_string.compare("__min") == 0)
    {
        head_tree->text = current_token_string;
    }
    else if (current_token_string.compare("__max") == 0)
    {
        head_tree->text = current_token_string;
    }
    else
    {
        cerr << "Syntax Error: missing '__min' or '__max' " << endl;
        return {FAIL, head_tree};
    }

    // <Expr>
    AST_token result;
    shared_ptr<ASTree> tree;
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == FAIL)
    {
        return {FAIL, head_tree};
    }

    head_tree->Leaf.push_back(tree);

    // ','
    auto next_token = lexer.get_current(); // RULE 3
    auto next_token_string = get<string>(next_token);

    if (next_token_string.compare(",") != 0)
    {
        cerr << "Syntax Error: missing ',' " << endl;
        return {FAIL, head_tree};
    }

    // <Expr>
    lexer.get_next(); // RULE 2
    tie(result, tree) = Expr();

    if (result == SUCCESS)
    {
        head_tree->Leaf.push_back(tree);
        return {SUCCESS, head_tree}; // RULE 1
    }
    return {FAIL, head_tree};
}
