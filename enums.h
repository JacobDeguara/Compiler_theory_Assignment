#ifndef __ENUMS_H__
#define __ENUMS_H__

enum token_type
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
    SpecialStatementsOp, // ::= '__print' | '__delay' | '__pixelr' | '__pixel'
    PadOp,               // ::= '__width' | '__height' | '__read' | '__randi'
    Declarator,          // ::= 'let'
    Func,                // ::= 'fun'
    Identifier,          // ::=  ( <Letter> | '_' ) { '_' | <Letter> | <Digit>}
    Comment,             // ::=  '//' {<anything>} '\n'
};

enum state
{
    Start,           // Start ; -> '#' -> Hex1 / -> <digit> -> DigitState / -> <letter> -> IdentifierState
    Hex1,            // Hex1 ; -> <hex> -> Hex2
    Hex2,            // Hex2 ; -> <hex> -> Hex3
    Hex3,            // Hex3 ; -> <hex> -> Hex4
    Hex4,            // Hex4 ; -> <hex> -> Hex5
    Hex5,            // Hex5 ; -> <hex> -> Hex6
    Hex6,            // Hex6 ; -> <hex> -> Hex7
    Hex7,            // Hex7 -> <anything> -> Defaultstate
    DigitState,      // DigitState ; -> '.' -> FloatState / -> <Digit> -> DigitState
    FloatState,      // FloatState ; -> <Digit> -> FLoatState
    IdentifierState, // IdentifierState ; -> <digit> | <letter> | '_' -> IdentifierState
    DefaultState,    // anything that isnt what we stated -> DefaultState
};

enum AST_token
{
    AST_ENDOFFILE,    // end of file
    SUCCESS,          // No error in the process
    FAIL,             // An error in the process
    PROGRAM,          // ::= { 〈Statement〉 }
    STATEMENT,        // ::= 〈VariableDecl〉 ‘;’ | 〈Assignment〉 ‘;’ | 〈PrintStatement〉 ‘;’| 〈DelayStatement〉 ‘;’ | 〈PixelStatement〉 ‘;’ | 〈IfStatement〉 | 〈ForStatement〉 | 〈WhileStatement〉 | 〈RtrnStatement〉 ‘;’ | 〈FunctionDecl〉 | 〈Block〉
    BLOCK,            // ::= ‘{’ { 〈Statement〉 } ‘}’
    FUNCTION_DECL,    // ::= ‘fun’ 〈Identifier〉 ‘(’ [ 〈FormalParams〉 ] ‘)’ ‘->’ 〈Type〉〈Block〉
    FORMAL_PARAMS,    // ::= 〈FormalParam〉 { ‘,’ 〈FormalParam〉 }
    FORMAL_PARAMETER, // ::= 〈Identifier 〉 ‘:’ 〈Type〉
    WHILE_STATEMENT,  // ::= ‘while’ ‘(’ 〈Expr〉 ‘)’ 〈Block〉
    FOR_STATEMENT,    // ::= ‘for’ ‘(’ [ 〈VariableDecl〉 ] ’;’ 〈Expr〉 ’;’ [ 〈Assignment〉 ] ‘)’ 〈Block〉
    IF_STATEMENT,     // ::= ‘if’ ‘(’ 〈Expr〉 ‘)’ 〈Block〉 [ ‘else’ 〈Block〉 ]
    RTRN_STATEMENT,   // ::=  ‘return’ 〈Expr〉
    PIXEL_STATEMENT,  // ::= ‘__pixelr’ 〈Expr〉‘,’〈Expr〉‘,’〈Expr〉‘,’〈Expr〉‘,’〈Expr〉| ‘__pixel’ 〈Expr〉‘,’〈Expr〉‘,’〈Expr〉
    PRINT_STATEMENT,  // ::= ‘__print’ 〈Expr
    DELAY_STATEMENT,  // ::= ‘__delay’ 〈Expr〉
    VARIABLE_DECL,    // ::= ‘let’ 〈Identifier〉 ‘:’ 〈Type〉 ‘=’ 〈Expr〉
    ASSIGNMENT,       // ::= 〈Identifier〉 ‘=’ 〈Expr〉
    EXPR,             // ::= 〈SimpleExpr〉 { 〈RelationalOp〉 〈SimpleExpr〉 }
    SIMPLE_EXPR,      // ::= 〈Term〉 { 〈AdditiveOp〉 〈Term〉 }
    TERM,             // ::= 〈Factor〉 { 〈MultiplicativeOp〉 〈Factor〉 }
    FACTOR,           // ::= 〈Literal〉 | 〈Identifier〉 | 〈FunctionCall〉 | 〈SubExpr〉 | 〈Unary〉 | 〈PadRandI〉 | 〈PadWidth〉 | 〈PadHeight〉 | 〈PadRead〉
    LITERAL,          // ::= 〈BooleanLiteral〉 | 〈IntegerLiteral〉 | 〈FloatLiteral〉 | 〈ColourLiteral〉 | 〈PadWidth〉 | 〈PadHeight〉 | 〈PadRead〉
    FUNCTION_CALL,    // ::= 〈Identifier 〉 ‘(’ [ 〈ActualParams〉 ] ‘)’
    SUB_EXPR,         //  ::= ‘(’ 〈Expr 〉 ‘)’
    UNARY,            // ::= ( ‘-’ | ‘not’ | ‘!’ ) 〈Expr〉
    PAD_RANDI,        // :: = ‘__randi’ 〈Expr
    PAD_READ,         //  :: = ‘__read’ 〈Expr〉‘,’〈Expr〉
    ACTUAL_PARAMS,    // ::= 〈Expr 〉 { ‘,’ 〈Expr 〉 }
    PAD_WIDTH,        // ::= ‘__width’
    PAD_HEIGHT,       // ::= ‘__height’
    IDENTIFIER,       // ::= <identifier(token_type)>
    BOOL_LITERAL,     // ::= <BooleanLiteral(token_type)>
    INTEGER_LITERAL,  // ::= <IntegerLiteral(token_type)>
    FLOAT_LITERAL,    // ::= <FloatLiteral(token_type)>
    COLOUR_LITERAL,   // ::= <ColourLiteral(token_type)>
};

struct ASTree
{
    AST_token token;
    string text;
    vector<shared_ptr<ASTree>> Leaf;
};

#endif // __ENUMS_H__