#include <tuple>
#include <vector>
#include <memory>

#include "Lexer.h"
#include "enums.h"

class Parser
{
private:
    tuple<AST_token, shared_ptr<ASTree>> Program();
    tuple<AST_token, shared_ptr<ASTree>> Statement();
    tuple<AST_token, shared_ptr<ASTree>> Block();
    tuple<AST_token, shared_ptr<ASTree>> Function_Decl();
    tuple<AST_token, shared_ptr<ASTree>> Formal_Params();
    tuple<AST_token, shared_ptr<ASTree>> Formal_Parameter();
    tuple<AST_token, shared_ptr<ASTree>> While_Statement();
    tuple<AST_token, shared_ptr<ASTree>> For_Statement();
    tuple<AST_token, shared_ptr<ASTree>> If_Statement();
    tuple<AST_token, shared_ptr<ASTree>> RTRN_Statement();
    tuple<AST_token, shared_ptr<ASTree>> Pixel_Statement();
    tuple<AST_token, shared_ptr<ASTree>> Print_Statement();
    tuple<AST_token, shared_ptr<ASTree>> Delay_Statement();
    tuple<AST_token, shared_ptr<ASTree>> Variable_Decl();
    tuple<AST_token, shared_ptr<ASTree>> Assigment();
    tuple<AST_token, shared_ptr<ASTree>> Expr();
    tuple<AST_token, shared_ptr<ASTree>> Simple_Expr();
    tuple<AST_token, shared_ptr<ASTree>> Term();
    tuple<AST_token, shared_ptr<ASTree>> Literal();
    tuple<AST_token, shared_ptr<ASTree>> Function_Call();
    tuple<AST_token, shared_ptr<ASTree>> Sub_Expr();
    tuple<AST_token, shared_ptr<ASTree>> Unary();
    tuple<AST_token, shared_ptr<ASTree>> Pad_RandI();
    tuple<AST_token, shared_ptr<ASTree>> Pad_Read();
    tuple<AST_token, shared_ptr<ASTree>> Actual_Params();
    tuple<AST_token, shared_ptr<ASTree>> Pad_Width();
    tuple<AST_token, shared_ptr<ASTree>> Pad_Height();
    Lexer lexer = Lexer();

public:
    Parser(string file_name);
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
IMPORTANT RULE: ALWAYS END BY using get_next() (unless you've already used it) &
                ALWAYS START BY using get_current()

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
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Variable_Decl()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Assigment()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Expr()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Simple_Expr()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Term()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Literal()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Function_Call()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Sub_Expr()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Unary()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_RandI()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_Read()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Actual_Params()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_Width()
{
}

tuple<AST_token, shared_ptr<ASTree>> Parser::Pad_Height()
{
}
