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

    print_code_to_file();
}

void PixLang_Converter::print_code(shared_ptr<ASTree> start_node)
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

    code.push_back(".main");

    size_t alloc_pos;
    code.push_back("push 1");
    code.push_back("oframe");
    alloc_pos = code.size();
    code.push_back("push ");
    code.push_back("alloc");

    auto node_items = node->Leaf;

    vector<variables> v;

    for (size_t i = 0; i < node_items.size(); i++)
    {
        auto item = node_items.at(i);
        variables temp;

        switch (item->token)
        {
        case VARIABLE_DECL:
            temp = convert_variable_decliration(item, v, 0);
            v.push_back(temp);
            break;
        case ASSIGNMENT:
            convert_assignment(item, v);
            break;
        case DELAY_STATEMENT:
            convert_delay(item, v);
            break;
        case PRINT_STATEMENT:
            convert_print(item, v);
            break;
        case PIXEL_STATEMENT:
            convert_pixel(item, v);
            break;
        case PIXELR_STATEMENT:
            convert_pixelr(item, v);
            break;
        case WHILE_STATEMENT:
            convert_while(item, v, 0);
            break;
        case IF_STATEMENT:
            convert_if(item, v, 0);
            break;
        case FOR_STATEMENT:
            convert_for(item, v, 0);
            break;
        case FILL_CLEAR:
            convert_fill(item, v);
            break;
        case EXIT_PROGRAM:
            convert_exit(item);
            break;
        default:
            break;
        }
    }

    code.at(alloc_pos).append(to_string(amount_of_vaiables_declared));

    code.push_back("cframe");
    code.push_back("halt");

    for (size_t i = 0; i < node_items.size(); i++)
    {
        auto item = node_items.at(i);

        if (item->token == FUNCTION_DECL)
        {
            code.push_back("//");
            convert_function_decl(item);
        }
    }
}

void PixLang_Converter::convert_block(shared_ptr<ASTree> node, vector<variables> v, int scope_value)
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
        variables temp;

        switch (item->token)
        {
        case VARIABLE_DECL:
            temp = convert_variable_decliration(item, v, scope_value);
            v.push_back(temp);
            break;
        case ASSIGNMENT:
            convert_assignment(item, v);
            break;
        case DELAY_STATEMENT:
            convert_delay(item, v);
            break;
        case PRINT_STATEMENT:
            convert_print(item, v);
            break;
        case PIXEL_STATEMENT:
            convert_pixel(item, v);
            break;
        case PIXELR_STATEMENT:
            convert_pixelr(item, v);
            break;
        case WHILE_STATEMENT:
            convert_while(item, v, scope_value);
            break;
        case IF_STATEMENT:
            convert_if(item, v, scope_value);
            break;
        case FOR_STATEMENT:
            convert_for(item, v, scope_value);
            break;
        case RTRN_STATEMENT:
            convert_return(item, v);
            break;
        case FILL_CLEAR:
            convert_fill(item, v);
            break;
        case EXIT_PROGRAM:
            convert_exit(item);
            break;
        default:
            cerr << item->token << " token isnt in block?" << endl;
            return;
            break;
        }
    }
}
void PixLang_Converter::convert_if(shared_ptr<ASTree> node, vector<variables> v, int scope_value)
{
    if (node->token != IF_STATEMENT)
    {
        cerr << "node token isnt IF_STATEMENT" << endl;
        return;
    }
    size_t firstposition, secondposition, thirdposition, firstjumpsize, secondjumpsize, thirdjumpsize;

    firstposition = code.size();
    code.push_back("push #PC+");

    convert_expr(node->Leaf.at(0), v);

    code.push_back("cjmp");
    secondposition = code.size();
    code.push_back("push #PC+");
    code.push_back("jmp");
    firstjumpsize = code.size();

    convert_block(node->Leaf.at(1), v, scope_value);

    if (node->Leaf.size() > 2)
    {
        thirdposition = code.size();
        code.push_back("push #PC+");
        code.push_back("jmp");
        secondjumpsize = code.size();

        convert_block(node->Leaf.at(1), v, scope_value);

        thirdjumpsize = code.size();

        code.at(thirdposition).append(to_string(thirdjumpsize - thirdposition));
    }
    else
    {
        secondjumpsize = code.size();
    }

    code.at(firstposition).append(to_string(firstjumpsize - firstposition));
    code.at(secondposition).append(to_string(secondjumpsize - secondposition));
}
void PixLang_Converter::convert_while(shared_ptr<ASTree> node, vector<variables> v, int scope_value)
{
    if (node->token != WHILE_STATEMENT)
    {
        cerr << "node token isnt WHILE_STATEMENT" << endl;
        return;
    }
    size_t firstposition, secondposition, thirdposition, firstjumpsize, secondjumpsize, thirdjumpsize;

    firstposition = code.size();
    thirdjumpsize = code.size();
    code.push_back("push #PC+");

    convert_expr(node->Leaf.at(0), v);

    code.push_back("cjmp");
    secondposition = code.size();
    code.push_back("push #PC+");
    code.push_back("jmp");
    firstjumpsize = code.size();

    convert_block(node->Leaf.at(1), v, scope_value);

    thirdposition = code.size();
    code.push_back("push #PC-");
    code.push_back("jmp");
    secondjumpsize = code.size();

    code.at(firstposition).append(to_string(firstjumpsize - firstposition));
    code.at(secondposition).append(to_string(secondjumpsize - secondposition));
    code.at(thirdposition).append(to_string(thirdposition - thirdjumpsize));
}
void PixLang_Converter::convert_for(shared_ptr<ASTree> node, vector<variables> v, int scope_value)
{
    if (node->token != FOR_STATEMENT)
    {
        cerr << "node token isnt FOR_STATEMENT" << endl;
        return;
    }

    size_t firstposition, secondposition, thirdposition, firstjumpsize, secondjumpsize, thirdjumpsize;
    variables var_x;
    for (size_t i = 0; i < node->Leaf.size(); i++)
    {

        switch (node->Leaf.at(i)->token)
        {
        case VARIABLE_DECL:
            var_x = convert_variable_decliration(node->Leaf.at(i), v, scope_value);
            v.push_back(var_x);
            break;
        case EXPR:
            firstposition = thirdjumpsize = code.size();
            code.push_back("push #PC+");

            convert_expr(node->Leaf.at(i), v);

            code.push_back("cjmp");
            secondposition = code.size();
            code.push_back("push #PC+");
            code.push_back("jmp");
            firstjumpsize = code.size();
            break;
        case BLOCK:
            convert_block(node->Leaf.at(i), v, scope_value);
            break;
        }
    }

    for (size_t i = 0; i < node->Leaf.size(); i++)
    {
        if (node->Leaf.at(i)->token == ASSIGNMENT)
        {
            convert_assignment(node->Leaf.at(i), v);
            break;
        }
    }
    thirdposition = code.size();
    code.push_back("push #PC-");
    code.push_back("jmp");
    secondjumpsize = code.size();

    code.at(firstposition).append(to_string(firstjumpsize - firstposition));
    code.at(secondposition).append(to_string(secondjumpsize - secondposition));
    code.at(thirdposition).append(to_string(thirdposition - thirdjumpsize));
}
variables PixLang_Converter::convert_variable_decliration(shared_ptr<ASTree> node, vector<variables> v, int scope_value)
{
    if (node->token != VARIABLE_DECL)
    {
        cerr << "node token isnt VARIABLE_DECL" << endl;
        variables var_x;
        var_x.id = " ";
        return var_x;
    }

    auto identifier = node->Leaf.at(0)->text;

    convert_expr(node->Leaf.at(2), v); // value

    code.push_back("push " + to_string(variable_size_based_on_scope(v, scope_value))); // position
    code.push_back("push " + to_string(scope_value));                                  // frame
    code.push_back("st");

    variables var_x;
    var_x.id = identifier;
    var_x.call = "push [" + to_string(variable_size_based_on_scope(v, scope_value)) + ":" + to_string(scope_value) + "]";
    var_x.pos = variable_size_based_on_scope(v, scope_value);
    var_x.frame = scope_value;

    amount_of_vaiables_declared++;
    return var_x;
}

size_t PixLang_Converter::variable_size_based_on_scope(vector<variables> v, int scope_value)
{
    size_t count = 0;

    for (size_t i = 0; i < v.size(); i++)
    {
        if (v.at(i).frame == scope_value)
        {
            count++;
        }
    }
    return count;
}

void PixLang_Converter::convert_assignment(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != ASSIGNMENT)
    {
        cerr << "node token isnt ASSIGNMENT" << endl;
        return;
    }

    auto identifier = node->Leaf.at(0)->text;

    convert_expr(node->Leaf.at(1), v);

    string pos;
    string frame;

    for (size_t i = 0; i < v.size(); i++)
    {
        if (v.at(i).id.compare(identifier) == 0)
        {
            pos = to_string(v.at(i).pos);
            frame = to_string(v.at(i).frame);
            break;
        }
    }

    code.push_back("push " + pos);
    code.push_back("push " + frame);
    code.push_back("st");
}
void PixLang_Converter::convert_delay(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != DELAY_STATEMENT)
    {
        cerr << "node token isnt DELAY_STATEMENT" << endl;
        return;
    }

    convert_expr(node->Leaf.at(0), v);

    code.push_back("delay");
}
void PixLang_Converter::convert_print(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != PRINT_STATEMENT)
    {
        cerr << "node token isnt PRINT_STATEMENT" << endl;
        return;
    }

    convert_expr(node->Leaf.at(0), v);

    code.push_back("print");
}
void PixLang_Converter::convert_pixel(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != PIXEL_STATEMENT)
    {
        cerr << "node token isnt PIXEL_STATEMENT" << endl;
        return;
    }

    convert_expr(node->Leaf.at(2), v);
    convert_expr(node->Leaf.at(1), v);
    convert_expr(node->Leaf.at(0), v);

    code.push_back("pixel");
}
void PixLang_Converter::convert_pixelr(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != PIXELR_STATEMENT)
    {
        cerr << "node token isnt PIXELR_STATEMENT" << endl;
        return;
    }

    convert_expr(node->Leaf.at(4), v);
    convert_expr(node->Leaf.at(3), v);
    convert_expr(node->Leaf.at(2), v);
    convert_expr(node->Leaf.at(1), v);
    convert_expr(node->Leaf.at(0), v);

    code.push_back("pixelr");
}
void PixLang_Converter::convert_function_decl(shared_ptr<ASTree> node)
{
    if (node->token != FUNCTION_DECL)
    {
        cerr << "node token isnt FUNCTION_DECL" << endl;
        return;
    }
    amount_of_vaiables_declared = 0;
    auto identifier = node->Leaf.at(0)->text;

    code.push_back("." + identifier);

    size_t alloc_pos;
    alloc_pos = code.size();
    code.push_back("push ");
    code.push_back("alloc");

    vector<shared_ptr<ASTree>> params;
    for (size_t i = 0; i < node->Leaf.size(); i++)
    {
        if (node->Leaf.at(i)->token == FORMAL_PARAMS)
        {
            params = node->Leaf.at(i)->Leaf;
        }
    }

    vector<variables> var_N;

    for (size_t i = 0; i < params.size(); i++)
    {
        variables v;
        v.id = params.at(i)->Leaf.at(0)->text;
        v.call = "push [" + to_string(i) + "]";
        v.pos = i;
        v.frame = 0;
        var_N.push_back(v);
    }

    convert_block(node->Leaf.at(node->Leaf.size() - 1), var_N, 1);

    code.at(alloc_pos).append(to_string(amount_of_vaiables_declared));
}
void PixLang_Converter::convert_return(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != RTRN_STATEMENT)
    {
        cerr << "node token isnt RTRN_STATEMENT" << endl;
        return;
    }

    convert_expr(node->Leaf.at(0), v);
    code.push_back("ret");
}

void PixLang_Converter::convert_expr(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != EXPR)
    {
        cerr << "node token isnt EXPR" << endl;
        return;
    }

    convert_expr2(node->Leaf.at(0), v);
}

void PixLang_Converter::convert_expr2(shared_ptr<ASTree> node, vector<variables> v)
{
    switch (node->token)
    {
    case INTEGER_LITERAL:
        code.push_back("push " + node->text);
        break;
    case FLOAT_LITERAL:
        code.push_back("push " + node->text);
        break;
    case COLOUR_LITERAL:
        code.push_back("push " + node->text);
        break;
    case BOOL_LITERAL:
        if (node->text.compare("true") == 0)
        {
            code.push_back("push 1");
        }
        else
        {
            code.push_back("push 0");
        }
        break;
    case IDENTIFIER:
        convert_identifier(node, v);
        break;
    case PAD_HEIGHT:
        convert_height(node);
        break;
    case PAD_WIDTH:
        convert_width(node);
        break;
    case PAD_READ:
        convert_read(node, v);
        break;
    case PAD_RANDI:
        convert_randi(node, v);
        break;
    case FUNCTION_CALL:
        convert_function_call(node, v);
        break;
    case ADDITIVEOP:
        convert_additive(node, v);
        break;
    case MULTIPLICATIVEOP:
        convert_multiplicative(node, v);
        break;
    case RELATIONALOP:
        convert_relational(node, v);
        break;
    case MAX_MIN:
        convert_min_max(node, v);
        break;
    default:
        return;
        break;
    }
}

void PixLang_Converter::convert_additive(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != ADDITIVEOP)
    {
        cerr << "node token isnt ADDITIVEOP" << endl;
        return;
    }

    convert_expr2(node->Leaf.at(1), v);
    convert_expr2(node->Leaf.at(0), v);

    auto text = node->text;
    if (text.compare("+") == 0)
    {
        code.push_back("add");
    }
    else if (text.compare("-") == 0)
    {
        code.push_back("sub");
    }
    else if (text.compare("or") == 0)
    {
        code.push_back("max");
    }
}
void PixLang_Converter::convert_multiplicative(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != MULTIPLICATIVEOP)
    {
        cerr << "node token isnt MULTIPLICATIVEOP" << endl;
        return;
    }

    // fliped for order
    convert_expr2(node->Leaf.at(1), v);
    convert_expr2(node->Leaf.at(0), v);

    auto text = node->text;
    if (text.compare("*") == 0)
    {
        code.push_back("mul");
    }
    else if (text.compare("/") == 0)
    {
        code.push_back("div"); //  to be continued, div possibly isnt available
    }
    else if (text.compare("and") == 0)
    {
        code.push_back("min");
    }
}
void PixLang_Converter::convert_relational(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != RELATIONALOP)
    {
        cerr << "node token isnt RELATIONALOP" << endl;
        return;
    }

    auto text = node->text;
    auto firstposition = code.size(); // #PC
    if (text.compare("!=") == 0)
    {
        code.push_back("push #PC+");
    }

    // flipped since order is important
    convert_expr2(node->Leaf.at(1), v);
    convert_expr2(node->Leaf.at(0), v);

    if (text.compare("<") == 0)
    {
        code.push_back("lt");
    }
    else if (text.compare(">") == 0)
    {
        code.push_back("gt");
    }
    else if (text.compare("<=") == 0)
    {
        code.push_back("le");
    }
    else if (text.compare(">=") == 0)
    {
        code.push_back("ge");
    }
    else if (text.compare("==") == 0)
    {
        code.push_back("eq");
    }
    else if (text.compare("!=") == 0)
    {
        code.push_back("eq");
        code.push_back("cjmp");
        code.push_back("push 1");
        auto secondposition = code.size();
        code.push_back("push #PC+");
        code.push_back("jmp");
        auto firstjumpsize = code.size();
        code.push_back("push 0");
        auto secondjumpsize = code.size();

        code.at(firstposition).append(to_string(firstjumpsize - firstposition));
        code.at(secondposition).append(to_string(secondjumpsize - secondposition));
    }
}

void PixLang_Converter::convert_function_call(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != FUNCTION_CALL)
    {
        cerr << "node token isnt FUNCTION_CALL" << endl;
        return;
    }

    auto identifier = node->Leaf.at(0)->text;

    if (node->Leaf.size() > 1)
    {
        auto parameters = node->Leaf.at(1)->Leaf;

        for (int i = (parameters.size() - 1); i > -1; i--)
        {
            convert_expr(parameters.at(i), v);
        }

        code.push_back("push " + to_string(parameters.size()));
    }
    else
    {
        code.push_back("push 0");
    }

    code.push_back("push ." + identifier);
    code.push_back("call");
}
void PixLang_Converter::convert_unary(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != UNARY)
    {
        cerr << "node token isnt UNARY" << endl;
        return;
    }
    auto text = node->text;

    auto firstposition = code.size(); // #PC

    if (text.compare("!") == 0 || text.compare("not") == 0)
    {
        code.push_back("push #PC+");
    }

    convert_expr(node->Leaf.at(0), v);

    if (text.compare("!") == 0 || text.compare("not") == 0)
    {
        code.push_back("cjmp");
        code.push_back("push 1");
        auto secondposition = code.size();
        code.push_back("push #PC+");
        code.push_back("jmp");
        auto firstjumpsize = code.size();
        code.push_back("push 0");
        auto secondjumpsize = code.size();

        code.at(firstposition).append(to_string(firstjumpsize - firstposition));
        code.at(secondposition).append(to_string(secondjumpsize - secondposition));
    }
    else if (text.compare("-") == 0)
    {
        code.push_back("push 0");
        code.push_back("sub");
    }
}
void PixLang_Converter::convert_identifier(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != IDENTIFIER)
    {
        cerr << "node token isnt IDENTIFIER" << endl;
        return;
    }
    auto identifier = node->text;

    for (size_t i = 0; i < v.size(); i++)
    {
        if (v.at(i).id.compare(identifier) == 0)
        {
            code.push_back(v.at(i).call);
        }
    }
}
void PixLang_Converter::convert_randi(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != PAD_RANDI)
    {
        cerr << "node token isnt PAD_RANDI" << endl;
        return;
    }

    convert_expr(node->Leaf.at(0), v);
    code.push_back("irnd");
}
void PixLang_Converter::convert_width(shared_ptr<ASTree> node)
{
    if (node->token != PAD_WIDTH)
    {
        cerr << "node token isnt PAD_WIDTH" << endl;
        return;
    }

    code.push_back("width");
}
void PixLang_Converter::convert_height(shared_ptr<ASTree> node)
{
    if (node->token != PAD_HEIGHT)
    {
        cerr << "node token isnt PAD_HEIGHT" << endl;
        return;
    }

    code.push_back("height");
}
void PixLang_Converter::convert_read(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != PAD_READ)
    {
        cerr << "node token isnt PAD_READ" << endl;
        return;
    }

    convert_expr(node->Leaf.at(1), v); // y
    convert_expr(node->Leaf.at(0), v); // x
    code.push_back("read");
}

void PixLang_Converter::convert_exit(shared_ptr<ASTree> node)
{
    if (node->token != EXIT_PROGRAM)
    {
        cerr << "node token isnt EXIT_PROGRAM" << endl;
        return;
    }

    code.push_back("halt");
}

void PixLang_Converter::convert_min_max(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != MAX_MIN)
    {
        cerr << "node token isnt MAX_MIN" << endl;
        return;
    }

    convert_expr(node->Leaf.at(1), v); // y
    convert_expr(node->Leaf.at(0), v); // x

    if (node->text.compare("__min") == 0)
    {
        code.push_back("min");
    }
    else if (node->text.compare("__max") == 0)
    {
        code.push_back("max");
    }
}

void PixLang_Converter::convert_fill(shared_ptr<ASTree> node, vector<variables> v)
{
    if (node->token != FILL_CLEAR)
    {
        cerr << "node token isnt FILL_CLEAR" << endl;
        return;
    }

    if (node->Leaf.size() == 0)
    {
        code.push_back("push #f9f9f9");
        code.push_back("clear");
    }
    else
    {
        convert_expr(node->Leaf.at(0), v);
        code.push_back("clear");
    }
}