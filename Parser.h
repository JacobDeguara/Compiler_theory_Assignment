#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>
#include <tuple>
#include <vector>
#include <memory>

#include "Lexer.cpp"
#include "AST_XML_gen.cpp"
#include "enums.h"

class Parser
{
private:
    Lexer lexer = Lexer();
    shared_ptr<ASTree> root;

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
    // Fill_Statement ::= '__fill' <Expr> | '__clear'
    tuple<AST_token, shared_ptr<ASTree>> Fill_Statement();
    // Exit_statment ::= '__exit'
    tuple<AST_token, shared_ptr<ASTree>> Exit_Statement();
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
    // ::= '__max' <Expr>','<Expr> , '__min' <Expr>','<Expr>
    tuple<AST_token, shared_ptr<ASTree>> Pad_Min_Max();

public:
    Parser(string file_name);

    // Creates a new node
    shared_ptr<ASTree> ASTree_node_create(AST_token token);

    shared_ptr<ASTree> get_root()
    {
        return root;
    }

    bool Compile(bool echo, bool print_tree, bool show_hidden);
    ~Parser() = default;
};

#endif // __PARSER_H__