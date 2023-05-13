#ifndef __ENUMS_H__
#define __ENUMS_H__

#include <string>
#include <vector>
#include <memory>

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

// [ ] ::= do once or none / { } ::= do many times or none / '%s' speacific string
enum AST_token
{
    AST_ENDOFFILE,    // end of file
    SUCCESS,          // Regular expression suceeded
    FAIL,             // An error was found in the process
    PROGRAM,          // ::= { <Statement> }
    STATEMENT,        // ::= <VariableDecl> ';' | <Assignment> ';' | <PrintStatement> ';'| <DelayStatement> ';' | <PixelStatement> ';' | <IfStatement> | <ForStatement> | <WhileStatement> | <RtrnStatement> ';' | <Block>
    BLOCK,            // ::= '{' { <Statement> } '}'
    FUNCTION_DECL,    // ::= 'fun' <Identifier> '(' [ <FormalParams> ] ')' '->' <Type><Block>
    FORMAL_PARAMS,    // ::= <FormalParam> { ',' <FormalParam> }
    FORMAL_PARAMETER, // ::= <Identifier > ':' <Type>
    WHILE_STATEMENT,  // ::= 'while' '(' <Expr> ')' <Block>
    FOR_STATEMENT,    // ::= 'for' '(' [ <VariableDecl> ] ';' <Expr> ';' [ <Assignment> ] ')' <Block>
    IF_STATEMENT,     // ::= 'if' '(' <Expr> ')' <Block> [ 'else' <Block> ]
    RTRN_STATEMENT,   // ::= 'return' <Expr>
    PIXEL_STATEMENT,  // ::= '__pixel' <Expr>','<Expr>','<Expr>
    PIXELR_STATEMENT, // ::= '__pixelr' <Expr>','<Expr>','<Expr>','<Expr>','<Expr>
    PRINT_STATEMENT,  // ::= '__print' <Expr>
    DELAY_STATEMENT,  // ::= '__delay' <Expr>
    VARIABLE_DECL,    // ::= 'let' <Identifier> ':' <Type> '=' <Expr>
    ASSIGNMENT,       // ::= <Identifier> '=' <Expr>
    EXPR,             // ::= <SimpleExpr> { <RelationalOp> <SimpleExpr> }
    SIMPLE_EXPR,      // ::= <Term> { <AdditiveOp> <Term> }
    TERM,             // ::= <Factor> { <MultiplicativeOp> <Factor> }
    FACTOR,           // ::= <Literal> | <Identifier> | <FunctionCall> | <SubExpr> | <Unary> | <PadRandI> | <PadWidth> | <PadHeight> | <PadRead> | <Min_Max>
    LITERAL,          // ::= <BooleanLiteral> | <IntegerLiteral> | <FloatLiteral> | <ColourLiteral> | <PadWidth> | <PadHeight> | <PadRead>
    FUNCTION_CALL,    // ::= <Identifier> '(' [ <ActualParams> ] ')'
    SUB_EXPR,         // ::= '(' <Expr> ')'
    UNARY,            // ::= ( '-' | 'not' | '!' ) <Expr>
    PAD_RANDI,        // :: = '__randi' <Expr>
    PAD_READ,         // :: = '__read' <Expr>','<Expr>
    ACTUAL_PARAMS,    // ::= <Expr> { ',' <Expr> }
    PAD_WIDTH,        // ::= '__width'
    PAD_HEIGHT,       // ::= '__height'
    IDENTIFIER,       // ::= <identifier(token_type)>
    BOOL_LITERAL,     // ::= <BooleanLiteral(token_type)>
    INTEGER_LITERAL,  // ::= <IntegerLiteral(token_type)>
    FLOAT_LITERAL,    // ::= <FloatLiteral(token_type)>
    COLOUR_LITERAL,   // ::= <ColourLiteral(token_type)>
    RELATIONALOP,     // ::= '<' | '>' | '==' | '!=' | '<=' | '>='
    ADDITIVEOP,       // ::= '+' | '-' | 'or'
    MULTIPLICATIVEOP, // ::= '*' | '/' | 'and'
    TYPE,             // ::= 'float' | 'int' | 'bool' | 'colour'
    MAX_MIN,          // ::= '__max' <Expr>','<Expr> , '__min' <Expr>','<Expr>
    EXIT_PROGRAM,     // ::= '__exit'
    FILL_CLEAR,       // ::= '__fill' <Expr> | '__clear'
};

/*
    What to add to ^ are;
    max(x,y) & min(x,y)
    exit()
    clear & fill(x)
*/

struct ASTree
{
    AST_token token;
    std::string text;
    std::vector<std::shared_ptr<ASTree>> Leaf;
};

#endif // __ENUMS_H__