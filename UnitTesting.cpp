#include <iostream>

#include "Lexer.cpp"
#include "FSA.cpp"

#define ASSERT(x)                                                                     \
    {                                                                                 \
        if (!(x))                                                                     \
            std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; \
        else                                                                          \
            std::cout << __FUNCTION__ << " > PASS" << std::endl;                      \
    }

void test_lexer_set_up_succeeds(Lexer lexer)
{

    ASSERT(lexer.set_up_file("test.txt"));
}

void test_lexer_set_up_fails(Lexer lexer)
{

    ASSERT(!lexer.set_up_file("doesntExists.txt"));
}

void test_is_hex_succeeds(FSA lexer)
{
    bool flag = true;
    for (char i = '0'; i <= '9'; i++)
    {
        if (!lexer.is_hex(i))
            flag = false;
    }
    for (char i = 'a'; i <= 'f'; i++)
    {
        if (!lexer.is_hex(i))
            flag = false;
    }
    for (char i = 'A'; i <= 'F'; i++)
    {
        if (!lexer.is_hex(i))
            flag = false;
    }
    ASSERT(flag);
}

void test_is_hex_fails(FSA lexer)
{
    bool flag = true;
    if (lexer.is_hex('f' + 1) || lexer.is_hex('F' + 1) || lexer.is_hex('9' + 1) || lexer.is_hex('a' - 1) || lexer.is_hex('A' - 1) || lexer.is_hex('0' - 1))
        flag = false;
    ASSERT(flag);
}
void test_is_digit_succeeds(FSA lexer)
{
    bool flag = true;
    for (char i = '0'; i <= '9'; i++)
    {
        if (!lexer.is_hex(i))
            flag = false;
    }
    ASSERT(flag);
}

void test_is_digit_fails(FSA lexer)
{
    bool flag = true;
    if (lexer.is_hex('9' + 1) || lexer.is_hex('0' - 1))
        flag = false;
    ASSERT(flag);
}

void test_lexer_unit_tests()
{
    Lexer lexer = Lexer();
    test_lexer_set_up_fails(lexer);
    test_lexer_set_up_succeeds(lexer);
}

void test_NFSA_Hex(FSA fsa)
{
    auto token = fsa.StartNFSA("#ffffff");
    ASSERT(token == ColourLiteral);
    token = fsa.StartNFSA("#000000");
    ASSERT(token == ColourLiteral);
}

void test_NFSA_HexFail(FSA fsa)
{
    bool flag = true;
    // too many hexs
    // to little hexs
    //  no start

    auto token = fsa.StartNFSA("#fffffff");
    if (token != ENDOFFILE)
        flag = false;
    token = fsa.StartNFSA("#fffff");
    if (token != ENDOFFILE)
        flag = false;
    token = fsa.StartNFSA("#");
    if (token != ENDOFFILE)
        flag = false;
    token = fsa.StartNFSA("");
    if (token != ENDOFFILE)
        flag = false;
    ASSERT(flag);
}

void test_NFSA_Integer(FSA fsa)
{
    auto token = fsa.StartNFSA("1234567890");
    ASSERT(token == IntegerLiteral);
    token = fsa.StartNFSA("0");
    ASSERT(token == IntegerLiteral);
}

void test_NFSA_Float(FSA fsa)
{
    auto token = fsa.StartNFSA("1234567890.1234567890");
    ASSERT(token == FloatLiteral);
    token = fsa.StartNFSA("0.");
    ASSERT(token == FloatLiteral);
}

void test_NFSA_IntegerFail(FSA fsa)
{
    auto token = fsa.StartNFSA("01234abcd");
    ASSERT(token == ENDOFFILE);
}

void test_NFSA_FloatFail(FSA fsa)
{
    auto token = fsa.StartNFSA("01234.abcd");
    ASSERT(token == ENDOFFILE);
    token = fsa.StartNFSA("01234.324234abcd");
    ASSERT(token == ENDOFFILE);
}

void test_FSA_unit_tests()
{
    FSA fsa = FSA();
    test_is_hex_fails(fsa);
    test_is_hex_succeeds(fsa);
    test_is_digit_fails(fsa);
    test_is_digit_succeeds(fsa);
    // fsa.printMap();
    test_NFSA_Hex(fsa);
    test_NFSA_HexFail(fsa);
    test_NFSA_Integer(fsa);
    test_NFSA_IntegerFail(fsa);
    test_NFSA_Float(fsa);
    test_NFSA_FloatFail(fsa);
}

int main(void)
{
    test_lexer_unit_tests();
    test_FSA_unit_tests();
}