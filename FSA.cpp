#ifndef __FSA_H__
#define __FSA_H__

#include <map>
#include <iostream>

#include "enums.h"

using namespace std;

class FSA
{
private:
    multimap<state, state> NFSAmap;
    state current_state;
    void STATE_DECLARE(state from, state to)
    {
        map<state, state>::iterator it = NFSAmap.begin();
        NFSAmap.insert(it, pair<state, state>(from, to));
    };

    void Toggle(char c);

public:
    FSA();
    token_type StartNFSA(string token);

    bool is_hex(char c)
    {
        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
        {
            return true;
        }
        return false;
    };

    bool is_digit(char c)
    {
        if (c >= '0' && c <= '9')
        {
            return true;
        }
        return false;
    };

    bool is_letter(char c)
    {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        {
            return true;
        }
        return false;
    };

    void printMap();
    ~FSA() = default;
};

FSA::FSA()
{
    STATE_DECLARE(Start, Hex1); // #
    STATE_DECLARE(Hex1, Hex2);  // <hex>
    STATE_DECLARE(Hex2, Hex3);  // <hex>
    STATE_DECLARE(Hex3, Hex4);  // <hex>
    STATE_DECLARE(Hex4, Hex5);  // <hex>
    STATE_DECLARE(Hex5, Hex6);  // <hex>
    STATE_DECLARE(Hex6, Hex7);  // <hex>
    // (hex7) ::= colour

    STATE_DECLARE(Start, DigitState);      // <digit>
    STATE_DECLARE(DigitState, DigitState); // <digit>
    STATE_DECLARE(DigitState, FloatState); // '.'
    STATE_DECLARE(FloatState, FloatState); // <digit>
    // (DigitState) ::= digit | (FloatState) ::= float

    STATE_DECLARE(Start, IdentifierState);           // <letter>
    STATE_DECLARE(IdentifierState, IdentifierState); // '_' | <letter> | <digit>
    // (IdentifierState) ::= its something that is a word

    // Anything that fails will go to default state of which will be considered a wrong token
}

void FSA::Toggle(char c)
{

    switch (current_state)
    {
    case Start:
        // start ; -> # -> Hex1 / -> <digit> -> DigitState / -> <letter> -> IdentifierState
        if (c == '#')
        {
            current_state = Hex1; // next state to Hex1
            break;
        }
        else if (is_digit(c))
        {
            current_state = DigitState; // next state to Hex1
            break;
        }
        else if (is_letter(c))
        {
            current_state = IdentifierState;
            break;
        }
        else if (c == '_')
        {
            current_state = IdentifierState;
            break;
        }
        current_state = DefaultState;
        break;
        // Hex1 - Hex6 ; -> <hex> -> (next hex)
    case Hex1:
    case Hex2:
    case Hex3:
    case Hex4:
    case Hex5:
    case Hex6:
        if (is_hex(c))
        {
            auto ret = NFSAmap.find(current_state);
            current_state = ret->second;
            break;
        }
    case Hex7:
        // Hex7 -> anything -> Defaultstate
        current_state = DefaultState;
        break;
    case DigitState:
        // DigitState ; -> '.' -> FloatState / -> <Digit> -> DigitState
        if (c == '.')
        {
            current_state = FloatState;
            break;
        }
        else if (is_digit(c))
            break;
        current_state = DefaultState;
        break;
    case FloatState:
        // FloatState ; -> <Digit> -> FLoatState
        if (is_digit(c))
            break;

        current_state = DefaultState;
        break;
    case IdentifierState:
        // IdentifierState ; -> <digit> | <letter> | '_' -> IdentifierState
        if ((c == '_') || is_digit(c) || is_letter(c))
            break;

        current_state = DefaultState;
        break;
    default:
        // anything that isnt what we stated -> DefaultState
        current_state = DefaultState;
        break;
    }
}

void FSA::printMap()
{
    multimap<state, state>::iterator it;
    // showing contents:
    std::cout << "NFSAmap contains:\n";
    for (state ch = Start; ch <= DefaultState; ch = state(ch + 1))
    {
        std::pair<multimap<state, state>::iterator, multimap<state, state>::iterator> ret;

        ret = NFSAmap.equal_range(ch);
        std::cout << ch << " =>";

        for (multimap<state, state>::iterator it = ret.first; it != ret.second; ++it)
            std::cout << ' ' << it->second;

        std::cout << '\n';
    }
}

token_type FSA::StartNFSA(string token)
{

    current_state = Start;
    for (size_t i = 0; i < token.size(); i++)
    {
        Toggle(token.at(i));
    }

    switch (current_state)
    {
    case DigitState:
        return IntegerLiteral;
        break;
    case FloatState:
        return FloatLiteral;
        break;
    case Hex7:
        return ColourLiteral;
        break;
    case IdentifierState:
        return Identifier;
        break;
    default:
        return ENDOFFILE;
        break;
    }
}
#endif // __FSA_H__