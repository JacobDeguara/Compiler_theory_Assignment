#include "PixLang_Converter.h"

PixLang_Converter::PixLang_Converter(string file_out)
{
    this->file_out = file_out;
}

void PixLang_Converter::print_code_to_file()
{
    ofstream file;
    file.open(file_out, std::ofstream::out | std::ofstream::app);

    file << "\n";
    for (size_t i = 0; i < code.size(); i++)
    {
        file << code.at(i) << endl;
        if (file.fail())
        {
            cerr << "Error occured while printing code!" << endl;
            file.clear();
            break;
        }
    }

    file.close();
}

void PixLang_Converter::test()
{
    code.push_back("//printing test;");
    code.push_back(".main");
    code.push_back("print 1");
    code.push_back("halt");
}

void PixLang_Converter::print_tree(shared_ptr<ASTree> start_node)
{

    convert_program(start_node);
    print_code_to_file();
}

void PixLang_Converter::convert_program(shared_ptr<ASTree> node)
{
    if (node->token != PROGRAM)
    {
        cerr << "node token isnt PROGRAM" << endl;
        return;
    }

    auto node_items = node->Leaf;

    for (size_t i = 0; i < node_items.size(); i++)
    {
        auto item = node_items.at(i);

        switch (item->token)
        {
        case FUNCTION_DECL:
            convert_function_decl(item);
            break;
        case VARIABLE_DECL:
            convert_variable_decliration(item);
            break;
        case ASSIGNMENT:
            convert_assignment(item);
            break;
        case DELAY_STATEMENT:
            convert_delay(item);
            break;
        case PRINT_STATEMENT:
            convert_print(item);
            break;
        case PIXEL_STATEMENT:
            convert_pixel(item);
            break;
        case PIXELR_STATEMENT:
            convert_pixelr(item);
            break;
        case WHILE_STATEMENT:
            convert_while(item);
            break;
        case IF_STATEMENT:
            convert_if(item);
            break;
        case FOR_STATEMENT:
            convert_for(item);
            break;
        case RTRN_STATEMENT:
            convert_return(item);
            break;
        default:
            cerr << item->token << " token isnt in block?" << endl;
            return;
            break;
        }
    }
}

void PixLang_Converter::convert_block(shared_ptr<ASTree> node)
{
    if (node->token != BLOCK)
    {
        cerr << "node token isnt BLOCK" << endl;
        return;
    }

    auto node_items = node->Leaf;

    for (size_t i = 0; i < node_items.size(); i++)
    {
        auto item = node_items.at(i);

        switch (item->token)
        {
        case FUNCTION_DECL:
            convert_function_decl(item);
            break;
        case VARIABLE_DECL:
            convert_variable_decliration(item);
            break;
        case ASSIGNMENT:
            convert_assignment(item);
            break;
        case DELAY_STATEMENT:
            convert_delay(item);
            break;
        case PRINT_STATEMENT:
            convert_print(item);
            break;
        case PIXEL_STATEMENT:
            convert_pixel(item);
            break;
        case PIXELR_STATEMENT:
            convert_pixelr(item);
            break;
        case WHILE_STATEMENT:
            convert_while(item);
            break;
        case IF_STATEMENT:
            convert_if(item);
            break;
        case FOR_STATEMENT:
            convert_for(item);
            break;
        case RTRN_STATEMENT:
            convert_return(item);
            break;
        default:
            cerr << item->token << " token isnt in block?" << endl;
            return;
            break;
        }
    }
}
void PixLang_Converter::convert_if(shared_ptr<ASTree> node)
{
    if (node->token != IF_STATEMENT)
    {
        cerr << "node token isnt IF_STATEMENT" << endl;
        return;
    }

    convert_expr(node->Leaf.at(0));

    convert_block(node->Leaf.at(1));

    if (node->Leaf.size() > 2)
    {
        convert_block(node->Leaf.at(1));
    }
}
void PixLang_Converter::convert_while(shared_ptr<ASTree> node)
{
    if (node->token != WHILE_STATEMENT)
    {
        cerr << "node token isnt WHILE_STATEMENT" << endl;
        return;
    }

    convert_expr(node->Leaf.at(0));
}
void PixLang_Converter::convert_for(shared_ptr<ASTree> node)
{
    if (node->token != FOR_STATEMENT)
    {
        cerr << "node token isnt FOR_STATEMENT" << endl;
        return;
    }

    for (size_t i = 0; i < node->Leaf.size(); i++)
    {
        switch (node->Leaf.at(i)->token)
        {
        case VARIABLE_DECL:
            convert_variable_decliration(node->Leaf.at(i));
            break;
        case EXPR:
            convert_expr(node->Leaf.at(i));
            break;
        case ASSIGNMENT:
            convert_assignment(node->Leaf.at(i));
        }
    }
}
void PixLang_Converter::convert_variable_decliration(shared_ptr<ASTree> node)
{
    if (node->token != VARIABLE_DECL)
    {
        cerr << "node token isnt VARIABLE_DECL" << endl;
        return;
    }

    auto identifier = node->Leaf.at(0)->text;

    convert_expr(node->Leaf.at(2));
}
void PixLang_Converter::convert_assignment(shared_ptr<ASTree> node)
{
    if (node->token != ASSIGNMENT)
    {
        cerr << "node token isnt ASSIGNMENT" << endl;
        return;
    }

    auto identifier = node->Leaf.at(0)->text;

    convert_expr(node->Leaf.at(1));
}
void PixLang_Converter::convert_delay(shared_ptr<ASTree> node)
{
    if (node->token != DELAY_STATEMENT)
    {
        cerr << "node token isnt DELAY_STATEMENT" << endl;
        return;
    }

    convert_expr(node->Leaf.at(0));
}
void PixLang_Converter::convert_print(shared_ptr<ASTree> node)
{
    if (node->token != PRINT_STATEMENT)
    {
        cerr << "node token isnt PRINT_STATEMENT" << endl;
        return;
    }

    convert_expr(node->Leaf.at(0));
}
void PixLang_Converter::convert_pixel(shared_ptr<ASTree> node)
{
    if (node->token != PIXEL_STATEMENT)
    {
        cerr << "node token isnt PIXEL_STATEMENT" << endl;
        return;
    }

    convert_expr(node->Leaf.at(0));
    convert_expr(node->Leaf.at(1));
    convert_expr(node->Leaf.at(2));
}
void PixLang_Converter::convert_pixelr(shared_ptr<ASTree> node)
{
    if (node->token != PIXELR_STATEMENT)
    {
        cerr << "node token isnt PIXELR_STATEMENT" << endl;
        return;
    }

    convert_expr(node->Leaf.at(0));
    convert_expr(node->Leaf.at(1));
    convert_expr(node->Leaf.at(2));
    convert_expr(node->Leaf.at(3));
    convert_expr(node->Leaf.at(4));
}
void PixLang_Converter::convert_function_decl(shared_ptr<ASTree> node)
{
    if (node->token != FUNCTION_DECL)
    {
        cerr << "node token isnt FUNCTION_DECL" << endl;
        return;
    }

    auto identifier = node->Leaf.at(0)->text;

    convert_block(node->Leaf.at(node->Leaf.size() - 1));
}
void PixLang_Converter::convert_return(shared_ptr<ASTree> node)
{
    if (node->token != RTRN_STATEMENT)
    {
        cerr << "node token isnt RTRN_STATEMENT" << endl;
        return;
    }

    convert_expr(node->Leaf.at(0));
}

void PixLang_Converter::convert_expr(shared_ptr<ASTree> node)
{
    if (node->token != EXPR)
    {
        cerr << "node token isnt EXPR" << endl;
        return;
    }

    convert_expr2(node->Leaf.at(0));
}

void PixLang_Converter::convert_expr2(shared_ptr<ASTree> node)
{
    switch (node->token)
    {
    case INTEGER_LITERAL:
        break;
    case FLOAT_LITERAL:
        break;
    case COLOUR_LITERAL:
        break;
    case BOOL_LITERAL:
        break;
    case IDENTIFIER:
        convert_identifier(node);
        break;
    case PAD_HEIGHT:
        convert_height(node);
        break;
    case PAD_WIDTH:
        convert_width(node);
        break;
    case PAD_READ:
        convert_read(node);
        break;
    case PAD_RANDI:
        convert_randi(node);
        break;
    case FUNCTION_CALL:
        convert_function_call(node);
        break;
    case ADDITIVEOP:
        convert_expr2(node->Leaf.at(0));
        convert_expr2(node->Leaf.at(1));
        break;
    case MULTIPLICATIVEOP:
        convert_expr2(node->Leaf.at(0));
        convert_expr2(node->Leaf.at(1));
        break;
    case RELATIONALOP:
        convert_expr2(node->Leaf.at(0));
        convert_expr2(node->Leaf.at(0));
        break;
    default:
        return;
        break;
    }
}

void PixLang_Converter::convert_function_call(shared_ptr<ASTree> node)
{
    if (node->token != FUNCTION_CALL)
    {
        cerr << "node token isnt FUNCTION_CALL" << endl;
        return;
    }

    auto identifier = node->Leaf.at(0)->text;

    auto parameters = node->Leaf.at(1)->Leaf;

    for (size_t i = 0; i < parameters.size(); i++)
    {
        convert_expr(parameters.at(i));
    }
}
void PixLang_Converter::convert_unary(shared_ptr<ASTree> node)
{
    if (node->token != UNARY)
    {
        cerr << "node token isnt UNARY" << endl;
        return;
    }

    convert_expr(node->Leaf.at(0));
}
void PixLang_Converter::convert_identifier(shared_ptr<ASTree> node)
{
    if (node->token != IDENTIFIER)
    {
        cerr << "node token isnt IDENTIFIER" << endl;
        return;
    }
    auto identifier = node->text;
}
void PixLang_Converter::convert_randi(shared_ptr<ASTree> node)
{
    if (node->token != PAD_RANDI)
    {
        cerr << "node token isnt PAD_RANDI" << endl;
        return;
    }

    convert_expr(node->Leaf.at(0));
}
void PixLang_Converter::convert_width(shared_ptr<ASTree> node)
{
    if (node->token != PAD_WIDTH)
    {
        cerr << "node token isnt PAD_WIDTH" << endl;
        return;
    }
}
void PixLang_Converter::convert_height(shared_ptr<ASTree> node)
{
    if (node->token != PAD_HEIGHT)
    {
        cerr << "node token isnt PAD_HEIGHT" << endl;
        return;
    }
}
void PixLang_Converter::convert_read(shared_ptr<ASTree> node)
{
    if (node->token != PAD_READ)
    {
        cerr << "node token isnt PAD_READ" << endl;
        return;
    }

    convert_expr(node->Leaf.at(0));
    convert_expr(node->Leaf.at(1));
}