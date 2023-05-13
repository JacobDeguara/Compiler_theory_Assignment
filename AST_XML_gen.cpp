#ifndef __AST_XML_GEN_H__
#define __AST_XML_GEN_H__

#include <memory>
#include <iostream>
#include "enums.h"

using namespace std;

class AST_XML_gen
{
private:
    shared_ptr<ASTree> root;

    string get_type_to_string(AST_token token);

    void print_XML(int height, shared_ptr<ASTree> root, bool hide);

public:
    AST_XML_gen(shared_ptr<ASTree> root);

    void gen_XML(bool hide_extra_stuff);

    ~AST_XML_gen();
};

AST_XML_gen::AST_XML_gen(shared_ptr<ASTree> root)
{
    this->root = root;
}

string AST_XML_gen::get_type_to_string(AST_token token)
{
    switch (token)
    {
    case PROGRAM:
        return "Program";
        break;
    case STATEMENT:
        return "Statement";
        break;
    case BLOCK:
        return "Block";
        break;
    case FUNCTION_DECL:
        return "Function Declirtation";
        break;
    case FORMAL_PARAMS:
        return "Formal Parameters";
        break;
    case FORMAL_PARAMETER:
        return "Formal Parameter";
        break;
    case WHILE_STATEMENT:
        return "While Statement";
        break;
    case FOR_STATEMENT:
        return "For Statement";
        break;
    case IF_STATEMENT:
        return "If Statement";
        break;
    case RTRN_STATEMENT:
        return "Return Statement";
        break;
    case PIXEL_STATEMENT:
        return "Pixel Statement";
        break;
    case PIXELR_STATEMENT:
        return "Pixelr Statement";
        break;
    case PRINT_STATEMENT:
        return "Print Statment";
        break;
    case DELAY_STATEMENT:
        return "Delay Statement";
        break;
    case VARIABLE_DECL:
        return "Variable Declare";
        break;
    case ASSIGNMENT:
        return "Assignment";
        break;
    case EXPR:
        return "Expr";
        break;
    case LITERAL:
        return "Litreal";
        break;
    case FUNCTION_CALL:
        return "Function Call";
        break;
    case SUB_EXPR:
        return "Sub Expr";
        break;
    case UNARY:
        return "Unary";
        break;
    case PAD_RANDI:
        return "Pad Randi";
        break;
    case PAD_READ:
        return "Pad Read";
        break;
    case ACTUAL_PARAMS:
        return "Actual Params";
        break;
    case PAD_WIDTH:
        return "Pad Width";
        break;
    case PAD_HEIGHT:
        return "Pad Height";
        break;
    case IDENTIFIER:
        return "Identifier";
        break;
    case RELATIONALOP:
        return "Relational Op";
        break;
    case ADDITIVEOP:
        return "Additive Op";
        break;
    case MULTIPLICATIVEOP:
        return "Mulitplicative Op";
        break;
    case TYPE:
        return "Type";
        break;
    case BOOL_LITERAL:
        return "Boolean";
        break;
    case INTEGER_LITERAL:
        return "Integer";
        break;
    case FLOAT_LITERAL:
        return "Float";
        break;
    case COLOUR_LITERAL:
        return "Colour";
        break;
    case FACTOR:
        return "Factor";
    case MAX_MIN:
        return "Min/Max";
    case EXIT_PROGRAM:
        return "Exit";
    case FILL_CLEAR:
        return "Fill";
    }

    return std::to_string(token);
}

void AST_XML_gen::gen_XML(bool hide_extra_stuff)
{
    print_XML(0, this->root, hide_extra_stuff);
}

void AST_XML_gen::print_XML(int height, shared_ptr<ASTree> root, bool hide)
{
    auto token = root->token;
    if (hide == true)
    {
        if (token == BLOCK || token == EXPR)
        {
            // can remove STATMENT , SUB_EXPR, LITREAL from tree
            for (size_t i = 0; i < root->Leaf.size(); i++)
            {
                print_XML(height, root->Leaf.at(i), hide);
            }
            return;
        }
    }

    for (size_t i = 0; i < height; i++)
    {
        cout << "  ";
    }

    switch (token)
    {
    case IDENTIFIER:
    case COLOUR_LITERAL:
    case INTEGER_LITERAL:
    case BOOL_LITERAL:
    case FLOAT_LITERAL:
        cout << "<" << get_type_to_string(token) << ">" << root->text;
        break;
    case MAX_MIN:
    case UNARY:
    case TYPE:
    case RELATIONALOP:
    case MULTIPLICATIVEOP:
    case ADDITIVEOP:
        cout << "<" << get_type_to_string(token) << " = \"" << root->text << "\">";
        break;
    default:
        cout << "<" << get_type_to_string(token) << ">";
        break;
    }

    if (root->Leaf.size() != 0)
    {
        cout << endl;

        for (size_t i = 0; i < root->Leaf.size(); i++)
        {
            print_XML(height + 1, root->Leaf.at(i), hide);
        }

        for (size_t i = 0; i < height; i++)
        {
            cout << "  ";
        }
    }

    cout << "</" << get_type_to_string(token) << ">\n";
    return;
}

AST_XML_gen::~AST_XML_gen()
{
}

#endif // __AST_XML_GEN_H__