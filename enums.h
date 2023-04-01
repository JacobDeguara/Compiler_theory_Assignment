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
    StatementOp,         // ::= 'return' | 'if' | 'for' | 'while'
    SpecialStatementsOp, // ::= '__print' | '__delay' | '__pixelr' | '__pixel'
    PadOp,               // ::= '__width' | '__height' | '__read' | '__randi'
    Declarator,          // ::= 'let'
    Func,                // ::= 'fun'
    Identifier,          // ::=  ( <Letter> | '_' ) { '_' | <Letter> | <Digit>}
    Comment,             // ::=  '//' {<anything>} '\n'
};

enum state
{
    Start,
    Hex1,
    Hex2,
    Hex3,
    Hex4,
    Hex5,
    Hex6,
    Hex7,
    DigitState,
    FloatState,
    IdentifierState,
    DefaultState,
};
#endif // __ENUMS_H__